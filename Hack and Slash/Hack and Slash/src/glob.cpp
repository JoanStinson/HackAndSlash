#include "glob.h"
#include "Math.h"
using namespace math;

const string Glob::GLOB_ANIM_UP = "up";
const string Glob::GLOB_ANIM_DOWN = "down";
const string Glob::GLOB_ANIM_LEFT = "left";
const string Glob::GLOB_ANIM_RIGHT = "right";
const string Glob::GLOB_ANIM_IDLE_UP = "idleUp";
const string Glob::GLOB_ANIM_IDLE_DOWN = "idleDown";
const string Glob::GLOB_ANIM_IDLE_LEFT = "idleLeft";
const string Glob::GLOB_ANIM_IDLE_RIGHT = "idleRight";
const string Glob::GLOB_ANIM_ATTACK_UP = "attackUp";
const string Glob::GLOB_ANIM_ATTACK_DOWN = "attackDown";
const string Glob::GLOB_ANIM_ATTACK_LEFT = "attackLeft";
const string Glob::GLOB_ANIM_ATTACK_RIGHT = "attackRight";
const string Glob::GLOB_ANIM_TELEGRAPH_UP = "telegraphUp";
const string Glob::GLOB_ANIM_TELEGRAPH_DOWN = "telegraphDown";
const string Glob::GLOB_ANIM_TELEGRAPH_LEFT = "telegraphLeft";
const string Glob::GLOB_ANIM_TELEGRAPH_RIGHT = "telegraphRight";
const string Glob::GLOB_ANIM_DIE = "die";

const int Glob::GLOB_STATE_IDLE = 0;
const int Glob::GLOB_STATE_MOVE = 1;
const int Glob::GLOB_STATE_ATTACK = 2;
const int Glob::GLOB_STATE_TELEGRAPH = 3;
const int Glob::GLOB_STATE_DEAD = 4;

const int Glob::GLOB_AI_NORMAL = 0;
const int Glob::GLOB_AI_CHASE = 1;

int Glob::globsKilled = 0;

Glob::Glob(AnimationSet *animSet) {
	this->animSet = animSet;

	type = "enemy";

	//defaults
	x = globals::ScreenWidth / 2;
	y = globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 10 + (rand() % 20); //10-29
	damage = 0;
	collisionBoxW = 18;
	collisionBoxH = 20;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;

	collisionBoxYOffset = -14;

	direction = DIR_DOWN;
	changeAnimation(GLOB_STATE_IDLE, true);

	updateCollisionBox();
}
void Glob::update() {
	//check if died
	if (hp < 1 && state != GLOB_STATE_DEAD) {
		changeAnimation(GLOB_STATE_DEAD, true);
		moving = false;
		die();
	}

	think();

	updateCollisionBox();
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}
void Glob::think() {
	if (state == GLOB_STATE_IDLE || state == GLOB_STATE_MOVE) {
		thinkTimer -= TimeController::timeController.dT;
		//time to choose an action
		if (thinkTimer <= 0) {
			//reset the timer
			thinkTimer = rand() % 5;//0-5seconds
			int action = rand() % 10; //0-9

			if (action < 3) {
				moving = false;
				aiState = GLOB_AI_NORMAL;
				changeAnimation(GLOB_STATE_IDLE, true);
			}
			else {
				findNearestTarget();
				//found a target and its alive, lets get 'em
				if (target != NULL && target->hp > 0) {
					float dist = Entity::distanceBetweenTwoEntities(this, target);
					//if in range, ATTACK!
					if (dist < 100) {
						telegraph(); //telegraph our attack first so players have a chance to dodge
						aiState = GLOB_AI_NORMAL;

					}
					else {
						//otherwise move up to the player/target
						aiState = GLOB_AI_CHASE;
						moving = true;
						changeAnimation(GLOB_STATE_MOVE, state != GLOB_STATE_MOVE);
					}
				}
				else {
					//no targets, go idle
					moving = false;
					aiState = GLOB_AI_NORMAL;
					changeAnimation(GLOB_STATE_IDLE, true);
				}
			}

		}
	}
	//if chasing a target, then do that
	if (aiState == GLOB_AI_CHASE && hp > 0 && active) {
		//get the angle between me and the target
		angle = Entity::angleBetweenTwoEntities(this, target);
		//move that way
		move(angle);
	}
}
void Glob::telegraph() {
	moving = false;
	frameTimer = 0;
	changeAnimation(GLOB_STATE_TELEGRAPH, true);
}
void Glob::attack() {
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(GLOB_STATE_ATTACK, true);
	//SoundManager::soundManager.playSound("growl");
}
void Glob::die() {
	moving = false;
	state = GLOB_STATE_DEAD;
	changeAnimation(state, true);
	SoundManager::soundManager.playSound("enemyDie");

	//add to our score count
	Glob::globsKilled++;
}
void Glob::findNearestTarget() {
	target = NULL;
	//find closest target
	for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
		if ((*entity)->type == "hero" && (*entity)->active) {
			//if found first hero in list, just set them as the target for now
			if (target == NULL) {
				target = (LivingEntity*)(*entity); //if cant cast to LivingEntity, throw casting exception
			}
			else {
				//otherwise, is this other hero closer then the previous target
				if (Entity::distanceBetweenTwoEntities(this, (*entity)) < Entity::distanceBetweenTwoEntities(this, target)) {
					target = (LivingEntity*)(*entity);
				}
			}
		}
	}

}
void Glob::changeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;

	if (state == GLOB_STATE_IDLE) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_RIGHT);
	}
	else if (state == GLOB_STATE_MOVE) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_RIGHT);
	}
	else if (state == GLOB_STATE_ATTACK) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_RIGHT);
	}
	else if (state == GLOB_STATE_TELEGRAPH) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_RIGHT);
	}
	else if (state == GLOB_STATE_DEAD) {
		currentAnim = animSet->getAnimation(GLOB_ANIM_DIE);
	}

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}
void Glob::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	//if we're in moveState but not actually going anywhere then
	if (state == GLOB_STATE_MOVE && !moving) {
		changeAnimation(GLOB_STATE_IDLE, true);
	}
	//if we're idle but we're actually moving, then
	if (state != GLOB_STATE_MOVE && moving) {
		changeAnimation(GLOB_STATE_MOVE, true);
	}

	frameTimer += TimeController::timeController.dT;
	//time to change frames
	if (frameTimer >= currentFrame->duration) {
		//if at the end of the animation
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) {
			if (state == GLOB_STATE_TELEGRAPH) {
				//done telegraphing, now attack
				attack();
			}
			else if (state == GLOB_STATE_ATTACK) {
				changeAnimation(GLOB_STATE_MOVE, true);
			}
			else if (state == GLOB_STATE_DEAD) {
				frameTimer = 0;
				//if some how alive again, then change state back to moving
				if (hp > 0)
					changeAnimation(GLOB_STATE_MOVE, true);
				else
					active = false;
			}
			else {
				//loop animation
				currentFrame = currentAnim->getFrame(0);
			}
		}
		else {
			//otherwise just move to the next frame
			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}
void Glob::updateDamages() {
	if (active && hp > 0 && invincibleTimer <= 0) {
		//TODO do a function
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "hero") {
				//reference as LivingEntity, so we can access hitBox/damage
				LivingEntity* enemy = (LivingEntity*)(*entity);
				//if enemy hits me, thennnnnnnnn
				if (enemy->damage > 0 && collBetweenTwoRects(collisionBox, enemy->hitBox)) {
					enemy->hitLanded(this); //let attacker know they hit
					hp -= enemy->damage;

					if (hp > 0) {
						SoundManager::soundManager.playSound("enemyHit");
						invincibleTimer = 0.1;
					}
					//angle from other entity, towards this entity (get thrown backwards)
					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 300;
				}
			}
		}
	}
}