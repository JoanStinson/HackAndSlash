#include "Boss.h"
#include "Math.h"
using namespace math;

#define ANIM_IDLE "idle"
#define ANIM_CHARGE "charge"
#define ANIM_SHOOT "shoot"
#define ANIM_SLAM "slam"
#define ANIM_JUMP_TELEGRAPH "jumpTelegraph"
#define ANIM_JUMP "jump"
#define ANIM_DIE "die"

int Boss::roundKingsKilled = 0;

Boss::Boss(AnimationSet * animSet, AnimationSet * bulletAnimSet) {
	this->animSet = animSet;
	this->bulletAnimSet = bulletAnimSet;

	aiState = NORMAL;

	type = ENEMY;
	x = globals::ScreenWidth / 2;
	y = globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 100;//default = 500
	damage = 0;
	collisionBox.w = collisionBoxW = 50;
	collisionBox.h = collisionBoxH = 30;

	collisionBoxYOffset = -14;

	ChangeAnimation(IDLE, true);

	UpdateCollisionBox();
}

void Boss::Update() {
	//check if dead
	CheckIfDead(DEAD);

	//ai thinks what to do
	Think();

	UpdateShoot();

	UpdateCollisionBox();

	UpdateMovement();

	UpdateCollisions();

	UpdateHitBox();
	UpdateDamages();
	UpdateAnimation();
	UpdateInvincibleTimer();
}

void Boss::UpdateShoot() {
	//update all our shooting stuff
	if (state == SHOOT) {
		shootTimer -= TimeManager::timeController.dT;
		shotTimer -= TimeManager::timeController.dT;

		//if shooting time is over, stop shooting
		if (shootTimer <= 0) {
			ChangeAnimation(IDLE, true);
		}
		else if (shotTimer <= 0) { //otherwise if still shooting and its time to take a shot
			shotTimer = 0.5;
			Bullet *bullet = new Bullet(bulletAnimSet, x, y);
			SoundManager::soundManager.PlaySound("shoot");
			bullet->angle = angle;
			Entity::entities.push_back(bullet);
		}
	}
}

void Boss::Think() {
	//finds closest hero
	FindPlayer();

	//if there is a hero we can target, do the thinking
	if (target != nullptr) {
		//only tick down think timer if in IDLE state
		if (state == IDLE) {
			thinkTimer -= TimeManager::timeController.dT;
		}

		// keep setting angle to point towards target
		angle = Entity::AngleBetweenTwoEntities(this, target);

		//CHECK  WHICH PHASE WE ARE IN MY FRIEND
		if (hp > 250) {
			aiState = NORMAL;
		}
		else if (hp > 100) {
			aiState = DAMAGED;
		}
		else {
			aiState = FRENETIC;
		}

		//if thinkTimer is up, work out what to do next
		if (thinkTimer <= 0 && state == IDLE) {
			//reset animations, we're about to do something new
			frameTimer = 0;

			//in normal phase do:
			if (aiState == NORMAL) {
				//reset timer
				thinkTimer = 2;

				//randomly select iehter slam or charge
				//peniseeeeeeeeeeeeeeeeeeeeees
				int action = randomNumber(4);
				if (action % 2 == 0) {
					//eat it bitch!
					Slam();
				}
				else
					Charge();
			}
			else if (aiState == DAMAGED) {
				//reset timer
				thinkTimer = 1.5;

				//randomly select iehter slam or charge
				//peniseeeeeeeeeeeeeeeeeeeeees
				int action = randomNumber(6);
				if (action < 2) {
					//eat it bitch!
					Slam();
				}
				else if (action < 4)
					Charge();
				else
					JumpTelegraph();
			}
			else { //assuming we are in frantic phase
				//reset timer
				thinkTimer = 1;
				int action = randomNumber(4);
				if (action % 2 == 0)
					JumpTelegraph();
				else
					Charge();

			}
		}
	}
	else {
		//targeting no one
		moving = 0;
		ChangeAnimation(IDLE, (state != IDLE));
	}
}

void Boss::Charge() {
	moving = false;
	SoundManager::soundManager.PlaySound("laugh");
	ChangeAnimation(CHARGE, true);
}

void Boss::Shoot() {
	moving = false;
	//5s of shooting at the player
	shootTimer = 5;
	//take first shot immediately during shooting time
	shotTimer = 0;
	//if damaged or frantic, have a shorter shooting time
	if (aiState != NORMAL) {
		shootTimer = 3;
	}

	ChangeAnimation(SHOOT, true);

}

void Boss::Slam() {
	moving = false;
	ChangeAnimation(SLAM, true);
}

void Boss::JumpTelegraph() {
	moving = false;
	frameTimer = 0;
	ChangeAnimation(JUMP_TELEGRAPH, true);
}

void Boss::Jump() {
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	ChangeAnimation(JUMP, true);
}

void Boss::Die() {
	moving = false;
	ChangeAnimation(DEAD, true);
	SoundManager::soundManager.PlaySound("enemyDie");

	roundKingsKilled++;
}

void Boss::ChangeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;
	switch (state) {
	default: case IDLE: currentAnim = animSet->GetAnimation(ANIM_IDLE); break;
	case CHARGE: currentAnim = animSet->GetAnimation(ANIM_CHARGE); break;
	case SHOOT: currentAnim = animSet->GetAnimation(ANIM_SHOOT); break;
	case SLAM: currentAnim = animSet->GetAnimation(ANIM_SLAM); break;
	case JUMP_TELEGRAPH: currentAnim = animSet->GetAnimation(ANIM_JUMP_TELEGRAPH); break;
	case JUMP: currentAnim = animSet->GetAnimation(ANIM_JUMP); break;
	case DEAD: currentAnim = animSet->GetAnimation(ANIM_DIE); break;
	}

	if (resetFrameToBeginning) {
		currentFrame = currentAnim->GetFrame(0);
	}
	else {
		currentFrame = currentAnim->GetFrame(currentFrame->frameNumber);
	}
}

void Boss::UpdateAnimation() {
	if (currentFrame == nullptr || currentAnim == nullptr) {
		return;
	}

	frameTimer += TimeManager::timeController.dT;
	//if frameTimer greater then frame duration:
	if (frameTimer >= currentFrame->duration) {
		// if at the end of animation
		if (currentFrame->frameNumber == currentAnim->GetEndFrameNumber()) {
			if (state == CHARGE)
				Shoot();
			else if (state == JUMP_TELEGRAPH)
				Jump();
			else if (state == SLAM || state == JUMP)
				ChangeAnimation(IDLE, true);
			else if (state == DEAD) {
				frameTimer = 0;
				//if somehow alive again
				if (hp > 0)
					ChangeAnimation(IDLE, true);
				else
					active = false;
			}
		}
		else { //not end of animation, so move to next frame normally
			currentFrame = currentAnim->GetNextFrame(currentFrame);
			//IF THE NEW FRAME IS DEALING DAMAGE, we might want to make sounds
			GroupType<float> *damages = dynamic_cast<GroupType<float>*>(GroupBuilder::FindGroupByName("damage", currentFrame->frameData));
			//if does damage
			if (damages != nullptr && damages->GetGroupSize() > 0) {
				// and if its Slam state
				if (state == SLAM) {
					SoundManager::soundManager.PlaySound("smash");
				}
				else if (state == JUMP) {
					SoundManager::soundManager.PlaySound("crash");
				}
			}
		}
		frameTimer = 0;
	}
}

void Boss::UpdateDamages() {
	Creature::UpdateDamages(PLAYER, "enemyHit", 0.1);
}