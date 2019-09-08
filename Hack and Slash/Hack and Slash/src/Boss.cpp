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
	//update all our shooting stuff
	if (state == SHOOT) {
		shootTimer -= TM.GetDt();
		shotTimer -= TM.GetDt();

		//if shooting time is over, stop shooting
		if (shootTimer <= 0) {
			ChangeAnimation(IDLE, true);
		}
		else if (shotTimer <= 0) { //otherwise if still shooting and its time to take a shot
			shotTimer = 0.5;
			//find an inactive bullet and shoot it, instead of instantiating and deleting
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
	//finds closest hero
	FindPlayer();

	//if there is a hero we can target, do the thinking
	if (target != nullptr) {
		//only tick down think timer if in IDLE state
		if (state == IDLE) {
			thinkTimer -= TM.GetDt();
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
	SM.PlaySound("laugh");
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
	//if frameTimer greater then frame duration:
	if (frameTimer >= currentFrame->GetDuration()) {
		// if at the end of animation
		if (currentFrame->GetFrameNumber() == currentAnim->GetEndFrameNumber()) {
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
			GroupType<float> *damages = dynamic_cast<GroupType<float>*>(GroupBuilder::FindGroupByName("damage", currentFrame->GetFrameData()));
			//if does damage
			if (damages != nullptr && damages->GetGroupSize() > 0) {
				// and if its Slam state
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
