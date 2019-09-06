#include "Player.h"
#include "Math.h"
using namespace math;

const string Player::HERO_ANIM_UP = "up";
const string Player::HERO_ANIM_DOWN = "down";
const string Player::HERO_ANIM_LEFT = "left";
const string Player::HERO_ANIM_RIGHT = "right";
const string Player::HERO_ANIM_IDLE_UP = "idleUp";
const string Player::HERO_ANIM_IDLE_DOWN = "idleDown";
const string Player::HERO_ANIM_IDLE_LEFT = "idleLeft";
const string Player::HERO_ANIM_IDLE_RIGHT = "idleRight";
const string Player::HERO_ANIM_SLASH_UP = "slashUp";
const string Player::HERO_ANIM_SLASH_DOWN = "slashDown";
const string Player::HERO_ANIM_SLASH_LEFT = "slashLeft";
const string Player::HERO_ANIM_SLASH_RIGHT = "slashRight";
const string Player::HERO_ANIM_DASH_UP = "dashUp";
const string Player::HERO_ANIM_DASH_DOWN = "dashDown";
const string Player::HERO_ANIM_DASH_LEFT = "dashLeft";
const string Player::HERO_ANIM_DASH_RIGHT = "dashRight";
const string Player::HERO_ANIM_DIE = "die";

const int Player::HERO_STATE_IDLE = 0;
const int Player::HERO_STATE_MOVE = 1;
const int Player::HERO_STATE_SLASH = 2;
const int Player::HERO_STATE_DASH = 3;
const int Player::HERO_STATE_DEAD = 4;

Player::Player(AnimationSet *animSet) {
	this->animSet = animSet;
	type = "hero";

	//setup default hero values
	x = globals::ScreenWidth / 2;
	y = globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 80;
	hp = hpMax = 20;
	damage = 0;
	collisionBoxW = 20;
	collisionBoxH = 24;
	collisionBoxYOffset = -20;

	direction = DIR_DOWN;

	ChangeAnimation(HERO_STATE_IDLE, true);

	UpdateCollisionBox();
}

void Player::Update() {
	//check if dead
	if (hp < 1 && state != HERO_STATE_DEAD) {
		ChangeAnimation(HERO_STATE_DEAD, true);
		moving = false;
		Die();
	}

	UpdateCollisionBox();
	UpdateMovement();
	UpdateCollisions();

	UpdateHitBox();
	UpdateDamages();

	UpdateAnimation();
	UpdateInvincibleTimer();
}

void Player::Slash() {
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE)) {
		moving = false;
		frameTimer = 0;
		ChangeAnimation(HERO_STATE_SLASH, true);
		SoundManager::soundManager.PlaySound("swing");
	}
}

void Player::Dash() {
	if (hp > 0 && (state == HERO_STATE_MOVE || state == HERO_STATE_IDLE)) {
		moving = false;
		frameTimer = 0;

		//push the hero in the direction they are travelling
		slideAngle = angle;
		slideAmount = 300;
		invincibleTimer = 0.1;

		ChangeAnimation(HERO_STATE_DASH, true);
		SoundManager::soundManager.PlaySound("dash");
	}
}

void Player::Die() {
	moving = false;
	ChangeAnimation(HERO_STATE_DEAD, true);
}

void Player::Revive() {
	hp = hpMax;
	ChangeAnimation(HERO_STATE_IDLE, true);
	moving = false;
	x = globals::ScreenWidth / 2;
	y = globals::ScreenHeight / 2;
	slideAmount = 0;
}

void Player::ChangeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;
	switch (state) {
	default: case HERO_STATE_IDLE: switch (direction) {
	case DIR_UP: currentAnim = animSet->GetAnimation(HERO_ANIM_IDLE_UP); break;
	case DIR_DOWN: currentAnim = animSet->GetAnimation(HERO_ANIM_IDLE_DOWN); break;
	case DIR_LEFT: currentAnim = animSet->GetAnimation(HERO_ANIM_IDLE_LEFT); break;
	case DIR_RIGHT: default: currentAnim = animSet->GetAnimation(HERO_ANIM_IDLE_RIGHT); break;
	}; break;

	case HERO_STATE_SLASH: switch (direction) {
	case DIR_UP: currentAnim = animSet->GetAnimation(HERO_ANIM_SLASH_UP); break;
	case DIR_DOWN: currentAnim = animSet->GetAnimation(HERO_ANIM_SLASH_DOWN); break;
	case DIR_LEFT: currentAnim = animSet->GetAnimation(HERO_ANIM_SLASH_LEFT); break;
	case DIR_RIGHT: default: currentAnim = animSet->GetAnimation(HERO_ANIM_SLASH_RIGHT); break;
	}; break;

	case HERO_STATE_DASH: switch (direction) {
	case DIR_UP: currentAnim = animSet->GetAnimation(HERO_ANIM_DASH_UP); break;
	case DIR_DOWN: currentAnim = animSet->GetAnimation(HERO_ANIM_DASH_DOWN); break;
	case DIR_LEFT: currentAnim = animSet->GetAnimation(HERO_ANIM_DASH_LEFT); break;
	case DIR_RIGHT: default: currentAnim = animSet->GetAnimation(HERO_ANIM_DASH_RIGHT); break;
	}; break;

	case HERO_STATE_MOVE: switch (direction) {
	case DIR_UP: currentAnim = animSet->GetAnimation(HERO_ANIM_UP); break;
	case DIR_DOWN: currentAnim = animSet->GetAnimation(HERO_ANIM_DOWN); break;
	case DIR_LEFT: currentAnim = animSet->GetAnimation(HERO_ANIM_LEFT); break;
	case DIR_RIGHT: default: currentAnim = animSet->GetAnimation(HERO_ANIM_RIGHT); break;
	}; break;

	case HERO_STATE_DEAD: currentAnim = animSet->GetAnimation(HERO_ANIM_DIE); break;
	}
	if (resetFrameToBeginning) {
		currentFrame = currentAnim->GetFrame(0);
	}
	else {
		currentFrame = currentAnim->GetFrame(currentFrame->frameNumber);
	}
}

void Player::UpdateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL)
		return; //cant do much with animations without pointers pointing to them :S

	//if state says moving, but we're not, then change state/anim to idle
	if (state == HERO_STATE_MOVE && !moving) {
		ChangeAnimation(HERO_STATE_IDLE, true);
	}
	//if should be showing running animation, lets change state properly
	if (state != HERO_STATE_MOVE && moving) {
		ChangeAnimation(HERO_STATE_MOVE, true);
	}


	frameTimer += TimeManager::timeController.dT;
	//time to change frames :D
	if (frameTimer >= currentFrame->duration) {
		//if we're at the end of an animation?
		if (currentFrame->frameNumber == currentAnim->GetEndFrameNumber()) {
			if (state == HERO_STATE_SLASH || state == HERO_STATE_DASH) {
				//change back to moving state/anim
				ChangeAnimation(HERO_STATE_MOVE, true);
			}
			else if (state == HERO_STATE_DEAD && hp > 0) {
				//was dead, but now have more hp, get back up (move state)
				ChangeAnimation(HERO_STATE_MOVE, true);
			}
			else {
				//just reset animation (loops it back round)
				currentFrame = currentAnim->GetFrame(0);
			}
		}
		else {
			//just move onto the next frame in this animation
			currentFrame = currentAnim->GetNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}

void Player::UpdateDamages() {
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "enemy") {
				//we know enemies are living entities, so cast it to that
				Creature* enemy = (Creature*)(*entity);

				if (enemy->damage > 0 && collBetweenTwoRects(collisionBox, enemy->hitBox)) {
					enemy->HitLanded(this); //let attacker know they hit
					hp -= enemy->damage;

					//still alive!!
					if (hp > 0) {
						invincibleTimer = 0.3;
						SoundManager::soundManager.PlaySound("hit");
					}

					slideAngle = Entity::AngleBetweenTwoEntities((*entity), this);
					slideAmount = 200;
				}
			}
		}
	}
}