#include "Grob.h"
#include "Math.h"
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

Grob::Grob(AnimationSet *animSet) {

	this->animSet = animSet;

	type = ENEMY;

	//setup defaults
	x = globals::ScreenWidth / 2;
	y = globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 1;
	damage = 0;
	collisionBox.w = collisionBoxW = 18;
	collisionBox.h = collisionBoxH = 16;

	collisionBoxYOffset = -14;

	direction = DOWN;

	ChangeAnimation(IDLE, true);

	UpdateCollisionBox();
}

void Grob::Update() {
	//check if dead
	CheckIfDead(DEAD);

	Think();

	//update collision boxes
	UpdateCollisionBox();
	//update movement/input
	UpdateMovement();

	//bump into stuff
	UpdateCollisions();

	//only care about damage hitboxes after we're landed on a final spot in our code
	UpdateHitBox();
	UpdateDamages();

	//update animations
	UpdateAnimation();
	//update timers and things
	UpdateInvincibleTimer();
}

void Grob::Think() {
	if (state == IDLE || state == MOVE) {
		thinkTimer -= TimeManager::timeController.dT;

		if (thinkTimer <= 0) {
			//reset timer
			thinkTimer = 0.5;
			int action = rand() % 10;
			//be idle
			if (action < 3) {

				//otherwise move towards player to get into range
				angle = rand() % 360; //random angle to move in
				moving = true;
				ChangeAnimation(MOVE, state != MOVE);

			}
			else {
				//otherwise just be happy and idle :/
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
	SoundManager::soundManager.PlaySound("enemyDie");

	//add to our score count
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
		currentFrame = currentAnim->GetFrame(currentFrame->frameNumber); //change direction for example, wanna change animation, but not what frame we were on

}

void Grob::UpdateAnimation() {
	if (currentFrame == nullptr || currentAnim == nullptr)
		return; //cant do much with no frame or no animation

	//if in a moving state, but not actually moving, return to idle 
	if (state == MOVE && !moving) {
		ChangeAnimation(IDLE, true);
	}
	//if should show running animation, lets change the state properly
	if (state != MOVE && moving) {
		ChangeAnimation(MOVE, true);
	}

	frameTimer += TimeManager::timeController.dT;
	//time to change frames
	if (frameTimer >= currentFrame->duration) {
		//if at the end of the animation
		if (currentFrame->frameNumber == currentAnim->GetEndFrameNumber()) {
			//depends on current animation and whats going on a bit
			if (state == DEAD) {
				frameTimer = 0;
				//if some how alive again, then change state back to moving
				if (hp > 0)
					ChangeAnimation(MOVE, true);
				else
					active = false;
			}
			else {
				//just reset the animation
				currentFrame = currentAnim->GetFrame(0);
			}

		}
		else {
			//just move to the next frame in the animation
			currentFrame = currentAnim->GetNextFrame(currentFrame);
		}
		frameTimer = 0; //crucial step. If we miss this, the next frame skips real quick
	}
}

void Grob::UpdateHitBox() {
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

void Grob::UpdateDamages() {
	Creature::UpdateDamages(PLAYER, "enemyHit", 0.1, 300);
}