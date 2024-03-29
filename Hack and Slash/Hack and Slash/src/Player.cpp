#include "Player.h"
#include "Math.h"
#include "InputManager.h"
#include "Window.h"
using namespace math;

#define ANIM_UP "up"
#define ANIM_DOWN "down"
#define ANIM_LEFT "left"
#define ANIM_RIGHT "right"
#define ANIM_IDLE_UP "idleUp"
#define ANIM_IDLE_DOWN "idleDown"
#define ANIM_IDLE_LEFT "idleLeft"
#define ANIM_IDLE_RIGHT "idleRight"
#define ANIM_SLASH_UP "slashUp"
#define ANIM_SLASH_DOWN "slashDown"
#define ANIM_SLASH_LEFT "slashLeft"
#define ANIM_SLASH_RIGHT "slashRight"
#define ANIM_DASH_UP "dashUp"
#define ANIM_DASH_DOWN "dashDown"
#define ANIM_DASH_LEFT "dashLeft"
#define ANIM_DASH_RIGHT "dashRight"
#define ANIM_DIE "die"

Player::Player(AnimationSet *animSet) : Creature(animSet) {
	type = PLAYER;
	moveSpeedMax = 60; // default 80
	hp = maxHp = 30;
	collisionBoxW = 20;
	collisionBoxH = 24;
	collisionBoxYOffset = -20;
	ChangeAnimation(IDLE, true);
}

void Player::Update() {
	CheckIfDead(DEAD);
	UpdateCollisionBox();
	UpdateMovement();
	UpdateCollisions();
	UpdateHitBox();
	UpdateDamages();
	UpdateAnimation();
	UpdateInvincibleTimer();
}

void Player::Slash() {
	if (hp > 0 && (state == MOVE || state == IDLE)) {
		moving = false;
		frameTimer = 0;
		ChangeAnimation(SLASH, true);
		SM.PlaySound("swing");
	}
}

void Player::Dash() {
	if (hp > 0 && (state == MOVE || state == IDLE)) {
		moving = false;
		frameTimer = 0;

		// Push the player in the direction he is travelling in
		slideAngle = angle;
		slideAmount = 300;
		invincibleTimer = 0.1;

		ChangeAnimation(DASH, true);
		SM.PlaySound("dash");
	}
}

void Player::Die() {
	moving = false;
	ChangeAnimation(DEAD, true);
}

void Player::Revive() {
	hp = maxHp;
	ChangeAnimation(IDLE, true);
	moving = false;
	x = WINDOW.SCREEN_WIDTH / 2;
	y = WINDOW.SCREEN_HEIGHT / 2;
	slideAmount = 0;
}

void Player::ChangeAnimation(int newState, bool resetAnim) {
	state = newState;
	switch (state) {
	default: case IDLE: switch (direction) {
	case UP: currentAnim = animSet->GetAnimation(ANIM_IDLE_UP); break;
	case DOWN: currentAnim = animSet->GetAnimation(ANIM_IDLE_DOWN); break;
	case LEFT: currentAnim = animSet->GetAnimation(ANIM_IDLE_LEFT); break;
	case RIGHT: default: currentAnim = animSet->GetAnimation(ANIM_IDLE_RIGHT); break;
	}; break;

	case SLASH: switch (direction) {
	case UP: currentAnim = animSet->GetAnimation(ANIM_SLASH_UP); break;
	case DOWN: currentAnim = animSet->GetAnimation(ANIM_SLASH_DOWN); break;
	case LEFT: currentAnim = animSet->GetAnimation(ANIM_SLASH_LEFT); break;
	case RIGHT: default: currentAnim = animSet->GetAnimation(ANIM_SLASH_RIGHT); break;
	}; break;

	case DASH: switch (direction) {
	case UP: currentAnim = animSet->GetAnimation(ANIM_DASH_UP); break;
	case DOWN: currentAnim = animSet->GetAnimation(ANIM_DASH_DOWN); break;
	case LEFT: currentAnim = animSet->GetAnimation(ANIM_DASH_LEFT); break;
	case RIGHT: default: currentAnim = animSet->GetAnimation(ANIM_DASH_RIGHT); break;
	}; break;

	case MOVE: switch (direction) {
	case UP: currentAnim = animSet->GetAnimation(ANIM_UP); break;
	case DOWN: currentAnim = animSet->GetAnimation(ANIM_DOWN); break;
	case LEFT: currentAnim = animSet->GetAnimation(ANIM_LEFT); break;
	case RIGHT: default: currentAnim = animSet->GetAnimation(ANIM_RIGHT); break;
	}; break;

	case DEAD: currentAnim = animSet->GetAnimation(ANIM_DIE); break;
	}

	if (resetAnim)
		currentFrame = currentAnim->GetFrame(0);
	else
		currentFrame = currentAnim->GetFrame(currentFrame->GetFrameNumber());
}

void Player::UpdateAnimation() {
	if (currentFrame == nullptr || currentAnim == nullptr)
		return; // Can't do much with animations without pointers pointing to them 

	// If state says moving, but we're not, then change state/anim to idle
	if (state == MOVE && !moving) {
		ChangeAnimation(IDLE, true);
	}
	// If should be showing running animation, lets change state properly
	if (state != MOVE && moving) {
		ChangeAnimation(MOVE, true);
	}

	frameTimer += TM.GetDt();
	// Time to change frames 
	if (frameTimer >= currentFrame->GetDuration()) {
		// If we're at the end of an animation?
		if (currentFrame->GetFrameNumber() == currentAnim->GetEndFrameNumber()) {
			if (state == SLASH || state == DASH) {
				// Change back to moving state/anim
				ChangeAnimation(MOVE, true);
			}
			else if (state == DEAD && hp > 0) {
				// Was dead, but now have more hp, get back up (move state)
				ChangeAnimation(MOVE, true);
			}
			else {
				// Just reset animation (loops it back round)
				currentFrame = currentAnim->GetFrame(0);
			}
		}
		else {
			// Just move onto the next frame in this animation
			currentFrame = currentAnim->GetNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}

void Player::UpdateDamages() {
	Creature::UpdateDamages(ENEMY, "hit", 0.3, 200);
}