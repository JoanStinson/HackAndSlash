#include "Boss.h"
#include "Math.h"
#include "Window.h"
#include "Utils.h"
using namespace math;

#define ANIM_IDLE "idle"
#define ANIM_CHARGE "charge"
#define ANIM_SHOOT "shoot"
#define ANIM_SLAM "slam"
#define ANIM_JUMP_TELEGRAPH "jumpTelegraph"
#define ANIM_JUMP "jump"
#define ANIM_DIE "die"

int Boss::roundKingsKilled = 0;

Boss::Boss(AnimationSet *animSet, AnimationSet *bulletAnimSet) : Enemy(animSet) {
	this->bulletAnimSet = bulletAnimSet;
	aiState = NORMAL;
	moveSpeedMax = 20;
	hp = maxHp = 500;
	collisionBox.w = collisionBoxW = 50;
	collisionBox.h = collisionBoxH = 30;
	collisionBoxYOffset = -14;
	ChangeAnimation(IDLE, true);
	for (int i = 0; i < TOTAL_BULLETS; i++)
		bullets.push_back(new Bullet(bulletAnimSet, x, y));
}

Boss::~Boss() {
	delete bulletAnimSet;
	for each (auto *bullet in bullets) {
		bullets.remove(bullet), delete bullet;
	}
}

void Boss::Update() {
	CheckIfDead(DEAD);
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
	// Update all our shooting stuff
	if (state == SHOOT) {
		shootTimer -= TM.GetDt();
		delayShoot -= TM.GetDt();

		// If shooting time is over, stop shooting
		if (shootTimer <= 0) {
			ChangeAnimation(IDLE, true);
		}
		else if (delayShoot <= 0) { //Otherwise if still shooting and its time to take a shot
			delayShoot = 0.5;
			// Find an inactive bullet and shoot it, instead of instantiating and deleting
			for each (auto *bullet in bullets) {
				if (!bullet->active) {
					bullet->Reset(x, y);
					SM.PlaySound("shoot");
					bullet->angle = angle;
					Entity::entities.push_back(bullet);
					break;
				}
			}
		}


	}

	if (utils::debugging) {
		int activeBullets = 0;
		for each (auto *bullet in bullets) {
			if (bullet->active)
				activeBullets++;
		}
		cout << "Current active number of bullets: " << activeBullets << endl;
	}
}

void Boss::Think() {
	// Finds closest hero
	FindPlayer();

	// If there is a hero we can target, do the thinking
	if (target != nullptr) {
		// Only tick down think timer if in IDLE state
		if (state == IDLE) {
			thinkTimer -= TM.GetDt();
		}

		// Keep setting angle to point towards target
		angle = Entity::AngleBetweenTwoEntities(this, target);

		// Check in which phase we are 
		if (hp > 250) {
			aiState = NORMAL;
		}
		else if (hp > 100) {
			aiState = DAMAGED;
		}
		else {
			aiState = FRENETIC;
		}

		// If thinkTimer is up, work out what to do next
		if (thinkTimer <= 0 && state == IDLE) {
			// Reset animations, we're about to do something new
			frameTimer = 0;

			// In normal phase do:
			if (aiState == NORMAL) {
				//reset timer
				thinkTimer = 2;

				// Randomly select iehter slam or charge
				int action = randomNumber(4);
				if (action % 2 == 0) {
					Slam();
				}
				else
					Charge();
			}
			else if (aiState == DAMAGED) {
				// Reset timer
				thinkTimer = 1.5;

				// Randomly select iehter slam or charge
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
			else { // Assuming we are in frantic phase
				// Reset timer
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
		// Targeting no one
		moving = 0;
		ChangeAnimation(IDLE, (state != IDLE));
	}
}

void Boss::Charge() {
	moving = false;
	SM.PlaySound("laugh");
	ChangeAnimation(CHARGE, true);
}

void Boss::Shoot() {
	moving = false;
	// 5s of shooting at the player
	shootTimer = 5;
	// Take first shot immediately during shooting time
	delayShoot = 0;
	// If damaged or frantic, have a shorter shooting time
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
	SM.PlaySound("enemyDie");
	roundKingsKilled++;
}

void Boss::ChangeAnimation(int newState, bool resetAnim) {
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

	if (resetAnim)
		currentFrame = currentAnim->GetFrame(0);
	else
		currentFrame = currentAnim->GetFrame(currentFrame->GetFrameNumber());
}

void Boss::UpdateAnimation() {
	if (currentFrame == nullptr || currentAnim == nullptr) {
		return;
	}

	frameTimer += TM.GetDt();
	// If frameTimer greater then frame duration:
	if (frameTimer >= currentFrame->GetDuration()) {
		// If at the end of animation
		if (currentFrame->GetFrameNumber() == currentAnim->GetEndFrameNumber()) {
			if (state == CHARGE)
				Shoot();
			else if (state == JUMP_TELEGRAPH)
				Jump();
			else if (state == SLAM || state == JUMP)
				ChangeAnimation(IDLE, true);
			else if (state == DEAD) {
				frameTimer = 0;
				// If somehow alive again
				if (hp > 0)
					ChangeAnimation(IDLE, true);
				else
					active = false;
			}
		}
		else { // Not end of animation, so move to next frame normally
			currentFrame = currentAnim->GetNextFrame(currentFrame);
			// If the new frame is dealing damage, play sounds 
			GroupType<float> *damages = dynamic_cast<GroupType<float>*>(GroupBuilder::FindGroupByName("damage", currentFrame->GetFrameData()));
			// If does damage
			if (damages != nullptr && damages->GetGroupSize() > 0) {
				// And if its slam state
				if (state == SLAM) {
					SM.PlaySound("smash");
				}
				else if (state == JUMP) {
					SM.PlaySound("crash");
				}
			}
		}
		frameTimer = 0;
	}
}

void Boss::UpdateDamages() {
	Creature::UpdateDamages(PLAYER, "enemyHit", 0.1);
}
