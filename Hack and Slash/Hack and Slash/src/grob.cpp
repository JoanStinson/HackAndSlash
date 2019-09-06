#include "grob.h"

const string Grob::GROB_ANIM_UP = "up";
const string Grob::GROB_ANIM_DOWN = "down";
const string Grob::GROB_ANIM_LEFT = "left";
const string Grob::GROB_ANIM_RIGHT = "right";
const string Grob::GROB_ANIM_IDLE_UP = "idleUp";
const string Grob::GROB_ANIM_IDLE_DOWN = "idleDown";
const string Grob::GROB_ANIM_IDLE_LEFT = "idleLeft";
const string Grob::GROB_ANIM_IDLE_RIGHT = "idleRight";
const string Grob::GROB_ANIM_DIE = "die";

const int Grob::GROB_STATE_IDLE = 0;
const int Grob::GROB_STATE_MOVE = 1;
const int Grob::GROB_STATE_DEAD = 2;

int Grob::grobsKilled = 0;

//main functions
Grob::Grob(AnimationSet *animSet) {

	this->animSet = animSet;

	type = "enemy";

	//setup defaults
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 1;
	damage = 0;
	collisionBoxW = 18;
	collisionBoxH = 16;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;

	collisionBoxYOffset = -14;

	direction = DIR_DOWN;

	changeAnimation(GROB_STATE_IDLE, true);

	updateCollisionBox();
}
void Grob::update() {
	//check if died
	if (hp < 1 && state != GROB_STATE_DEAD) {
		changeAnimation(GROB_STATE_DEAD, true);
		moving = false;
		die();
	}

	think();

	//update collision boxes
	updateCollisionBox();
	//update movement/input
	updateMovement();

	//bump into stuff
	updateCollisions();

	//only care about damage hitboxes after we're landed on a final spot in our code
	updateHitBox();
	updateDamages();

	//update animations
	updateAnimation();
	//update timers and things
	updateInvincibleTimer();
}

void Grob::think() {
	if (state == GROB_STATE_IDLE || state == GROB_STATE_MOVE) {
		thinkTimer -= TimeController::timeController.dT;

		if (thinkTimer <= 0) {
			//reset timer
			thinkTimer = 0.5;
			int action = rand() % 10;
			//be idle
			if (action < 3) {

				//otherwise move towards player to get into range
				angle = rand() % 360; //random angle to move in
				moving = true;
				changeAnimation(GROB_STATE_MOVE, state != GROB_STATE_MOVE);

			}
			else {
				//otherwise just be happy and idle :/
				moving = false;
				changeAnimation(GROB_STATE_IDLE, true);
			}

		}
	}

	if (state == GROB_STATE_MOVE)
		move(angle);
}

void Grob::die() {
	moving = false;
	state = GROB_STATE_DEAD;
	changeAnimation(state, true);
	SoundManager::soundManager.playSound("enemyDie");

	//add to our score count
	Grob::grobsKilled++;
}

void Grob::changeAnimation(int newState, bool resetFrameToBegging) {
	state = newState;
	if (state == GROB_STATE_IDLE) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(GROB_ANIM_IDLE_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(GROB_ANIM_IDLE_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(GROB_ANIM_IDLE_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(GROB_ANIM_IDLE_RIGHT);
	}
	else if (state == GROB_STATE_MOVE) {
		if (direction == DIR_DOWN)
			currentAnim = animSet->getAnimation(GROB_ANIM_DOWN);
		if (direction == DIR_UP)
			currentAnim = animSet->getAnimation(GROB_ANIM_UP);
		if (direction == DIR_LEFT)
			currentAnim = animSet->getAnimation(GROB_ANIM_LEFT);
		if (direction == DIR_RIGHT)
			currentAnim = animSet->getAnimation(GROB_ANIM_RIGHT);
	}
	else if (state == GROB_STATE_DEAD) {
		//always faces the person watching. If directional death, do the same as above
		currentAnim = animSet->getAnimation(GROB_ANIM_DIE);
	}

	if (resetFrameToBegging)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber); //change direction for example, wanna change animation, but not what frame we were on

}

void Grob::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL)
		return; //cant do much with no frame or no animation

	//if in a moving state, but not actually moving, return to idle 
	if (state == GROB_STATE_MOVE && !moving) {
		changeAnimation(GROB_STATE_IDLE, true);
	}
	//if should show running animation, lets change the state properly
	if (state != GROB_STATE_MOVE && moving) {
		changeAnimation(GROB_STATE_MOVE, true);
	}

	frameTimer += TimeController::timeController.dT;
	//time to change frames
	if (frameTimer >= currentFrame->duration) {
		//if at the end of the animation
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) {
			//depends on current animation and whats going on a bit
			if (state == GROB_STATE_DEAD) {
				frameTimer = 0;
				//if some how alive again, then change state back to moving
				if (hp > 0)
					changeAnimation(GROB_STATE_MOVE, true);
				else
					active = false;
			}
			else {
				//just reset the animation
				currentFrame = currentAnim->getFrame(0);
			}

		}
		else {
			//just move to the next frame in the animation
			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0; //crucial step. If we miss this, the next frame skips real quick
	}
}

void Grob::updateHitBox() {
	//grobs are a constantly walking pain box
	hitBox.w = collisionBox.w + 2;
	hitBox.h = collisionBox.h + 2;
	hitBox.x = collisionBox.x - 1;
	hitBox.y = collisionBox.y - 1;
	if (hp > 0)
		damage = 10;
	else
		damage = 0;
}

void Grob::updateDamages() {
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "hero") {
				//reference as livingEntity, so we can access enemies hit box
				LivingEntity* enemy = (LivingEntity*)(*entity);

				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)) {
					enemy->hitLanded(this); //let attacker know they hit
					hp -= enemy->damage;

					if (hp > 0) {
						SoundManager::soundManager.playSound("enemyHit");
						invincibleTimer = 0.1;
					}
					//angle from other entity, to towards this entity
					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 300;
				}
			}
		}
	}
}