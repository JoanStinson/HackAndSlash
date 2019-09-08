#include "Grob.h"
#include "Math.h"
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
#define ANIM_DIE "die"

int Grob::grobsKilled = 0;

Grob::Grob(AnimationSet *animSet) : Enemy(animSet) {
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = maxHp = 1;
	collisionBox.w = collisionBoxW = 18;
	collisionBox.h = collisionBoxH = 16;
	collisionBoxYOffset = -14;
	ChangeAnimation(IDLE, true);
}

void Grob::Update() {
	CheckIfDead(DEAD);
	Think();
	UpdateCollisionBox();
	UpdateMovement();
	UpdateCollisions();
	UpdateHitBox();
	UpdateDamages();
	UpdateAnimation();
	UpdateInvincibleTimer();
}

void Grob::Think() {
	if (state == IDLE || state == MOVE) {
		thinkTimer -= TM.GetDt();

		if (thinkTimer <= 0) {
			// Reset timer
			thinkTimer = 0.5;
			int action = rand() % 10;
			// Be idle
			if (action < 3) {

				// Otherwise move towards player to get into range
				angle = rand() % 360; // Random angle to move in
				moving = true;
				ChangeAnimation(MOVE, state != MOVE);

			}
			else {
				// Otherwise just be happy and idle :/
				moving = false;
				ChangeAnimation(IDLE, true);
			}

		}
	}

	if (state == MOVE)
		Move(angle);
}

void Grob::Die() {
	moving = false;
	state = DEAD;
	ChangeAnimation(state, true);
	SM.PlaySound("enemyDie");
	Grob::grobsKilled++;
}

void Grob::ChangeAnimation(int newState, bool resetFrameToBegging) {
	state = newState;
	switch (state) {
	default: case IDLE: switch (direction) {
	case UP: currentAnim = animSet->GetAnimation(ANIM_IDLE_UP); break;
	case DOWN: currentAnim = animSet->GetAnimation(ANIM_IDLE_DOWN); break;
	case LEFT: currentAnim = animSet->GetAnimation(ANIM_IDLE_LEFT); break;
	case RIGHT: default: currentAnim = animSet->GetAnimation(ANIM_IDLE_RIGHT); break;
	}; break;


	case MOVE: switch (direction) {
	case UP: currentAnim = animSet->GetAnimation(ANIM_UP); break;
	case DOWN: currentAnim = animSet->GetAnimation(ANIM_DOWN); break;
	case LEFT: currentAnim = animSet->GetAnimation(ANIM_LEFT); break;
	case RIGHT: default: currentAnim = animSet->GetAnimation(ANIM_RIGHT); break;
	}; break;

	case DEAD: currentAnim = animSet->GetAnimation(ANIM_DIE); break;
	}

	if (resetFrameToBegging)
		currentFrame = currentAnim->GetFrame(0);
	else
		currentFrame = currentAnim->GetFrame(currentFrame->GetFrameNumber()); // Change direction for example, wanna change animation, but not what frame we were on

}

void Grob::UpdateAnimation() {
	if (currentFrame == nullptr || currentAnim == nullptr)
		return; // Can't do much with no frame or no animation

	// If in a moving state, but not actually moving, return to idle 
	if (state == MOVE && !moving) {
		ChangeAnimation(IDLE, true);
	}
	// If should show running animation, lets change the state properly
	if (state != MOVE && moving) {
		ChangeAnimation(MOVE, true);
	}

	frameTimer += TM.GetDt();
	// Time to change frames
	if (frameTimer >= currentFrame->GetDuration()) {
		// If at the end of the animation
		if (currentFrame->GetFrameNumber() == currentAnim->GetEndFrameNumber()) {
			// Depends on current animation and whats going on a bit
			if (state == DEAD) {
				frameTimer = 0;
				// If some how alive again, then change state back to moving
				if (hp > 0)
					ChangeAnimation(MOVE, true);
				else
					active = false;
			}
			else {
				// Just reset the animation
				currentFrame = currentAnim->GetFrame(0);
			}

		}
		else {
			// Just move to the next frame in the animation
			currentFrame = currentAnim->GetNextFrame(currentFrame);
		}
		frameTimer = 0; // Crucial step. If we miss this, the next frame skips real quick
	}
}

void Grob::UpdateHitBox() {
	// Grobs are a constantly walking pain box
	hitBox.w = collisionBox.w + 2;
	hitBox.h = collisionBox.h + 2;
	hitBox.x = collisionBox.x - 1;
	hitBox.y = collisionBox.y - 1;
	if (hp > 0)
		damage = 10;
	else
		damage = 0;
}

void Grob::UpdateDamages() {
	Creature::UpdateDamages(PLAYER, "enemyHit", 0.1, 300);
}