#include "hero.h"

const string Hero::HERO_ANIM_UP = "up";
const string Hero::HERO_ANIM_DOWN = "down";
const string Hero::HERO_ANIM_LEFT = "left";
const string Hero::HERO_ANIM_RIGHT = "right";
const string Hero::HERO_ANIM_IDLE_UP = "idleUp";
const string Hero::HERO_ANIM_IDLE_DOWN = "idleDown";
const string Hero::HERO_ANIM_IDLE_LEFT = "idleLeft";
const string Hero::HERO_ANIM_IDLE_RIGHT = "idleRight";
const string Hero::HERO_ANIM_SLASH_UP = "slashUp";
const string Hero::HERO_ANIM_SLASH_DOWN = "slashDown";
const string Hero::HERO_ANIM_SLASH_LEFT = "slashLeft";
const string Hero::HERO_ANIM_SLASH_RIGHT = "slashRight";
const string Hero::HERO_ANIM_DASH_UP = "dashUp";
const string Hero::HERO_ANIM_DASH_DOWN = "dashDown";
const string Hero::HERO_ANIM_DASH_LEFT = "dashLeft";
const string Hero::HERO_ANIM_DASH_RIGHT = "dashRight";
const string Hero::HERO_ANIM_DIE = "die";

const int Hero::HERO_STATE_IDLE = 0;
const int Hero::HERO_STATE_MOVE = 1;
const int Hero::HERO_STATE_SLASH = 2;
const int Hero::HERO_STATE_DASH = 3;
const int Hero::HERO_STATE_DEAD = 4;

Hero::Hero(AnimationSet *animSet) {
	this->animSet = animSet;
	type = "hero";

	//setup default hero values
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 80;
	hp = hpMax = 20;
	damage = 0;
	collisionBoxW = 20;
	collisionBoxH = 24;
	collisionBoxYOffset = -20;

	direction = DIR_DOWN;

	changeAnimation(HERO_STATE_IDLE, true);

	updateCollisionBox();
}
void Hero::update() {
	//check if dead
	if (hp < 1 && state != HERO_STATE_DEAD) {
		changeAnimation(HERO_STATE_DEAD, true);
		moving = false;
		die();
	}

	updateCollisionBox();
	updateMovement();
	updateCollisions();

	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}
void Hero::slash() {
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE)) {
		moving = false;
		frameTimer = 0;
		changeAnimation(HERO_STATE_SLASH, true);
		SoundManager::soundManager.playSound("swing");
	}
}
void Hero::dash() {
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE)) {
		moving = false;
		frameTimer = 0;

		//push the hero in the direction they are travelling
		slideAngle = angle;
		slideAmount = 300;
		invincibleTimer = 0.1;

		changeAnimation(HERO_STATE_DASH, true);
		SoundManager::soundManager.playSound("dash");
	}
}
void Hero::die() {
	moving = false;
	changeAnimation(HERO_STATE_DEAD, true);
}
void Hero::revive() {
	hp = hpMax;
	changeAnimation(HERO_STATE_IDLE, true);
	moving = false;
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	slideAmount = 0;
}
void Hero::changeAnimation(int newState, bool resetFrameToBeginning) {
	//if (state == newState) return;
	state = newState;
	switch (state) {
	default: case HERO_STATE_IDLE: switch (direction) {
	case DIR_UP: currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_UP); break;
	case DIR_DOWN: currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_DOWN); break;
	case DIR_LEFT: currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_LEFT); break;
	case DIR_RIGHT: default: currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_RIGHT); break;
	}; break;
	case HERO_STATE_SLASH: switch (direction) {
	case DIR_UP: currentAnim = animSet->getAnimation(HERO_ANIM_SLASH_UP); break;
	case DIR_DOWN: currentAnim = animSet->getAnimation(HERO_ANIM_SLASH_DOWN); break;
	case DIR_LEFT: currentAnim = animSet->getAnimation(HERO_ANIM_SLASH_LEFT); break;
	case DIR_RIGHT: default: currentAnim = animSet->getAnimation(HERO_ANIM_SLASH_RIGHT); break;
	}; break;
	case HERO_STATE_DASH: switch (direction) {
	case DIR_UP: currentAnim = animSet->getAnimation(HERO_ANIM_DASH_UP); break;
	case DIR_DOWN: currentAnim = animSet->getAnimation(HERO_ANIM_DASH_DOWN); break;
	case DIR_LEFT: currentAnim = animSet->getAnimation(HERO_ANIM_DASH_LEFT); break;
	case DIR_RIGHT: default: currentAnim = animSet->getAnimation(HERO_ANIM_DASH_RIGHT); break;
	}; break;
	case HERO_STATE_MOVE: switch (direction) {
	case DIR_UP: currentAnim = animSet->getAnimation(HERO_ANIM_UP); break;
	case DIR_DOWN: currentAnim = animSet->getAnimation(HERO_ANIM_DOWN); break;
	case DIR_LEFT: currentAnim = animSet->getAnimation(HERO_ANIM_LEFT); break;
	case DIR_RIGHT: default: currentAnim = animSet->getAnimation(HERO_ANIM_RIGHT); break;
	}; break;
	case HERO_STATE_DEAD: currentAnim = animSet->getAnimation(HERO_ANIM_DIE); break;
	}
	if (resetFrameToBeginning) {
		currentFrame = currentAnim->getFrame(0);
	}
	else {
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
	}
}
void Hero::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL)
		return; //cant do much with animations without pointers pointing to them :S

	//if state says moving, but we're not, then change state/anim to idle
	if (state == HERO_STATE_MOVE && !moving) {
		changeAnimation(HERO_STATE_IDLE, true);
	}
	//if should be showing running animation, lets change state properly
	if (state != HERO_STATE_MOVE && moving) {
		changeAnimation(HERO_STATE_MOVE, true);
	}


	frameTimer += TimeController::timeController.dT;
	//time to change frames :D
	if (frameTimer >= currentFrame->duration) {
		//if we're at the end of an animation?
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber()) {
			if (state == HERO_STATE_SLASH || state == HERO_STATE_DASH) {
				//change back to moving state/anim
				changeAnimation(HERO_STATE_MOVE, true);
			}
			else if (state == HERO_STATE_DEAD && hp > 0) {
				//was dead, but now have more hp, get back up (move state)
				changeAnimation(HERO_STATE_MOVE, true);
			}
			else {
				//just reset animation (loops it back round)
				currentFrame = currentAnim->getFrame(0);
			}
		}
		else {
			//just move onto the next frame in this animation
			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}
void Hero::updateDamages() {
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "enemy") {
				//we know enemies are living entities, so cast it to that
				LivingEntity* enemy = (LivingEntity*)(*entity);

				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox)) {
					enemy->hitLanded(this); //let attacker know they hit
					hp -= enemy->damage;

					//still alive!!
					if (hp > 0) {
						invincibleTimer = 0.3;
						SoundManager::soundManager.playSound("hit");
					}

					slideAngle = Entity::angleBetweenTwoEntities((*entity), this);
					slideAmount = 200;
				}
			}
		}
	}
}