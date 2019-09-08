#include "Glob.h"
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
#define ANIM_ATTACK_UP "attackUp"
#define ANIM_ATTACK_DOWN "attackDown"
#define ANIM_ATTACK_LEFT "attackLeft"
#define ANIM_ATTACK_RIGHT "attackRight"
#define ANIM_TELEGRAPH_UP "telegraphUp"
#define ANIM_TELEGRAPH_DOWN "telegraphDown"
#define ANIM_TELEGRAPH_LEFT "telegraphLeft"
#define ANIM_TELEGRAPH_RIGHT "telegraphRight"
#define ANIM_DIE "die"

int Glob::globsKilled = 0;

Glob::Glob(AnimationSet *animSet) : Enemy(animSet) {
	aiState = NORMAL;
	moveSpeedMax = 20;
	hp = maxHp = 10 + (rand() % 20); // 10-29
	collisionBox.w = collisionBoxW = 18;
	collisionBox.h = collisionBoxH = 20;
	collisionBoxYOffset = -14;
	ChangeAnimation(IDLE, true);
}

void Glob::Update() {
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

void Glob::Think() {
	if (state == IDLE || state == MOVE) {
		thinkTimer -= TM.GetDt();
		// Time to choose an action
		if (thinkTimer <= 0) {
			// Reset the timer
			thinkTimer = rand() % 5; // 0-5
			int action = rand() % 10; // 0-9

			if (action < 3) {
				moving = false;
				aiState = NORMAL;
				ChangeAnimation(IDLE, true);
			}
			else {
				FindPlayer();
				// Found a target and its alive, lets get 'em
				if (target != nullptr && target->hp > 0) {
					float dist = Entity::DistBetweenTwoEntities(this, target);
					// If in range, attack!
					if (dist < 100) {
						Telegraph(); // Telegraph our attack first so players have a chance to dodge
						aiState = NORMAL;

					}
					else {
						// Otherwise move up to the player/target
						aiState = CHASE;
						moving = true;
						ChangeAnimation(MOVE, state != MOVE);
					}
				}
				else {
					// No targets, go idle
					moving = false;
					aiState = NORMAL;
					ChangeAnimation(IDLE, true);
				}
			}

		}
	}
	// If chasing a target, then do that
	if (aiState == CHASE && hp > 0 && active) {
		// Get the angle between me and the target
		angle = Entity::AngleBetweenTwoEntities(this, target);
		// Move that way
		Move(angle);
	}
}

void Glob::Telegraph() {
	moving = false;
	frameTimer = 0;
	ChangeAnimation(TELEGRAPH, true);
}

void Glob::Attack() {
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	ChangeAnimation(ATTACK, true);
}

void Glob::Die() {
	moving = false;
	state = DEAD;
	ChangeAnimation(state, true);
	SM.PlaySound("enemyDie");
	Glob::globsKilled++;
}

void Glob::ChangeAnimation(int newState, bool resetAnim) {
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

	case ATTACK: switch (direction) {
	case UP: currentAnim = animSet->GetAnimation(ANIM_ATTACK_UP); break;
	case DOWN: currentAnim = animSet->GetAnimation(ANIM_ATTACK_DOWN); break;
	case LEFT: currentAnim = animSet->GetAnimation(ANIM_ATTACK_LEFT); break;
	case RIGHT: default: currentAnim = animSet->GetAnimation(ANIM_ATTACK_RIGHT); break;
	}; break;

	case TELEGRAPH: switch (direction) {
	case UP: currentAnim = animSet->GetAnimation(ANIM_TELEGRAPH_UP); break;
	case DOWN: currentAnim = animSet->GetAnimation(ANIM_TELEGRAPH_DOWN); break;
	case LEFT: currentAnim = animSet->GetAnimation(ANIM_TELEGRAPH_LEFT); break;
	case RIGHT: default: currentAnim = animSet->GetAnimation(ANIM_TELEGRAPH_RIGHT); break;
	}; break;

	case DEAD: currentAnim = animSet->GetAnimation(ANIM_DIE); break;
	}

	if (resetAnim)
		currentFrame = currentAnim->GetFrame(0);
	else
		currentFrame = currentAnim->GetFrame(currentFrame->GetFrameNumber());
}

void Glob::UpdateAnimation() {
	if (currentFrame == nullptr || currentAnim == nullptr)
		return;

	// If we're in moveState but not actually going anywhere then
	if (state == MOVE && !moving) {
		ChangeAnimation(IDLE, true);
	}
	// If we're idle but we're actually moving, then
	if (state != MOVE && moving) {
		ChangeAnimation(MOVE, true);
	}

	frameTimer += TM.GetDt();
	// Time to change frames
	if (frameTimer >= currentFrame->GetDuration()) {
		// If at the end of the animation
		if (currentFrame->GetFrameNumber() == currentAnim->GetEndFrameNumber()) {
			if (state == TELEGRAPH) {
				// Done telegraphing, now attack
				Attack();
			}
			else if (state == ATTACK) {
				ChangeAnimation(MOVE, true);
			}
			else if (state == DEAD) {
				frameTimer = 0;
				// If some how alive again, then change state back to moving
				if (hp > 0)
					ChangeAnimation(MOVE, true);
				else
					active = false;
			}
			else {
				// Loop animation
				currentFrame = currentAnim->GetFrame(0);
			}
		}
		else {
			// Otherwise just move to the next frame
			currentFrame = currentAnim->GetNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}

void Glob::UpdateDamages() {
	Creature::UpdateDamages(PLAYER, "enemyHit", 0.1, 300);
}
