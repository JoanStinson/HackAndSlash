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
	//this->animSet = animSet;

	aiState = NORMAL;


	//type = ENEMY;

	//defaults
	//x = WINDOW.SCREEN_WIDTH / 2;
	//y = WINDOW.SCREEN_HEIGHT / 2;
	//moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 10 + (rand() % 20); //10-29
	//damage = 0;
	collisionBox.w = collisionBoxW = 18;
	collisionBox.h = collisionBoxH = 20;

	collisionBoxYOffset = -14;

	//direction = DOWN;
	ChangeAnimation(IDLE, true);

	//UpdateCollisionBox();
}

void Glob::Update() {
	//check if dead
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
		//time to choose an action
		if (thinkTimer <= 0) {
			//reset the timer
			thinkTimer = rand() % 5;//0-5seconds
			int action = rand() % 10; //0-9

			if (action < 3) {
				moving = false;
				aiState = NORMAL;
				ChangeAnimation(IDLE, true);
			}
			else {
				FindPlayer();
				//found a target and its alive, lets get 'em
				if (target != nullptr && target->hp > 0) {
					float dist = Entity::DistBetweenTwoEntities(this, target);
					//if in range, ATTACK!
					if (dist < 100) {
						Telegraph(); //telegraph our attack first so players have a chance to dodge
						aiState = NORMAL;

					}
					else {
						//otherwise move up to the player/target
						aiState = CHASE;
						moving = true;
						ChangeAnimation(MOVE, state != MOVE);
					}
				}
				else {
					//no targets, go idle
					moving = false;
					aiState = NORMAL;
					ChangeAnimation(IDLE, true);
				}
			}

		}
	}
	//if chasing a target, then do that
	if (aiState == CHASE && hp > 0 && active) {
		//get the angle between me and the target
		angle = Entity::AngleBetweenTwoEntities(this, target);
		//move that way
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

	//add to our score count
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

	//if we're in moveState but not actually going anywhere then
	if (state == MOVE && !moving) {
		ChangeAnimation(IDLE, true);
	}
	//if we're idle but we're actually moving, then
	if (state != MOVE && moving) {
		ChangeAnimation(MOVE, true);
	}

	frameTimer += TM.GetDt();
	//time to change frames
	if (frameTimer >= currentFrame->GetDuration()) {
		//if at the end of the animation
		if (currentFrame->GetFrameNumber() == currentAnim->GetEndFrameNumber()) {
			if (state == TELEGRAPH) {
				//done telegraphing, now attack
				Attack();
			}
			else if (state == ATTACK) {
				ChangeAnimation(MOVE, true);
			}
			else if (state == DEAD) {
				frameTimer = 0;
				//if some how alive again, then change state back to moving
				if (hp > 0)
					ChangeAnimation(MOVE, true);
				else
					active = false;
			}
			else {
				//loop animation
				currentFrame = currentAnim->GetFrame(0);
			}
		}
		else {
			//otherwise just move to the next frame
			currentFrame = currentAnim->GetNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}

void Glob::UpdateDamages() {
	Creature::UpdateDamages(PLAYER, "enemyHit", 0.1, 300);
}