#include "Boss.h"
#include "Math.h"
using namespace math;

int Boss::roundKingsKilled = 0;

// animations
const string Boss::ROUND_KING_ANIM_IDLE = "idle";
const string Boss::ROUND_KING_ANIM_CHARGE = "charge";
const string Boss::ROUND_KING_ANIM_SHOOT = "shoot";
const string Boss::ROUND_KING_ANIM_SLAM = "slam";
const string Boss::ROUND_KING_ANIM_JUMP_TELEGRAPH = "jumpTelegraph";
const string Boss::ROUND_KING_ANIM_JUMP = "jump";
const string Boss::ROUND_KING_ANIM_DIE = "die";

// states 
const int Boss::ROUND_KING_STATE_IDLE = 1;
const int Boss::ROUND_KING_STATE_CHARGE = 2;
const int Boss::ROUND_KING_STATE_SHOOT = 3;
const int Boss::ROUND_KING_STATE_SLAM = 4;
const int Boss::ROUND_KING_STATE_JUMP_TELEGRAPH = 5;
const int Boss::ROUND_KING_STATE_JUMP = 6;
const int Boss::ROUND_KING_STATE_DEAD = 7;

//phases
const int Boss::ROUND_KING_PHASE_NORMAL = 1;
const int Boss::ROUND_KING_PHASE_DAMAGED = 2;
const int Boss::ROUND_KING_PHASE_FRENETIC = 3;

Boss::Boss(AnimationSet * animSet, AnimationSet * bulletAnimSet) {
	this->animSet = animSet;
	this->bulletAnimSet = bulletAnimSet;

	type = "enemy";
	x = globals::ScreenWidth / 2;
	y = globals::ScreenHeight / 2;
	moveSpeed = 0;
	moveSpeedMax = 20;
	hp = hpMax = 100;//default = 500
	damage = 0;
	collisionBoxW = 50;
	collisionBoxH = 30;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;

	collisionBoxYOffset = -14;

	ChangeAnimation(ROUND_KING_STATE_IDLE, true);

	UpdateCollisionBox();
}

void Boss::Update() {
	//check if dead
	if (hp < 1 && state != ROUND_KING_STATE_DEAD) {
		ChangeAnimation(ROUND_KING_STATE_DEAD, true);
		moving = false;
		Die();
	}

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
	if (state == ROUND_KING_STATE_SHOOT) {
		shootTimer -= TimeManager::timeController.dT;
		shotTimer -= TimeManager::timeController.dT;

		//if shooting time is over, stop shooting
		if (shootTimer <= 0) {
			ChangeAnimation(ROUND_KING_STATE_IDLE, true);
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
	FindNearestTarget();

	//if there is a hero we can target, do the thinking
	if (target != NULL) {
		//only tick down think timer if in IDLE state
		if (state == ROUND_KING_STATE_IDLE) {
			thinkTimer -= TimeManager::timeController.dT;
		}

		// keep setting angle to point towards target
		angle = Entity::AngleBetweenTwoEntities(this, target);

		//CHECK  WHICH PHASE WE ARE IN MY FRIEND
		if (hp > 250) {
			aiState = ROUND_KING_PHASE_NORMAL;
		}
		else if (hp > 100) {
			aiState = ROUND_KING_PHASE_DAMAGED;
		}
		else {
			aiState = ROUND_KING_PHASE_FRENETIC;
		}

		//if thinkTimer is up, work out what to do next
		if (thinkTimer <= 0 && state == ROUND_KING_STATE_IDLE) {
			//reset animations, we're about to do something new
			frameTimer = 0;

			//in normal phase do:
			if (aiState == ROUND_KING_PHASE_NORMAL) {
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
			else if (aiState == ROUND_KING_PHASE_DAMAGED) {
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
		ChangeAnimation(ROUND_KING_STATE_IDLE, (state != ROUND_KING_STATE_IDLE));
	}
}

void Boss::Charge() {
	moving = false;
	SoundManager::soundManager.PlaySound("laugh");
	ChangeAnimation(ROUND_KING_STATE_CHARGE, true);
}

void Boss::Shoot() {
	moving = false;
	//5s of shooting at the player
	shootTimer = 5;
	//take first shot immediately during shooting time
	shotTimer = 0;
	//if damaged or frantic, have a shorter shooting time
	if (aiState != ROUND_KING_PHASE_NORMAL) {
		shootTimer = 3;
	}

	ChangeAnimation(ROUND_KING_STATE_SHOOT, true);

}

void Boss::Slam() {
	moving = false;
	ChangeAnimation(ROUND_KING_STATE_SLAM, true);
}

void Boss::JumpTelegraph() {
	moving = false;
	frameTimer = 0;
	ChangeAnimation(ROUND_KING_STATE_JUMP_TELEGRAPH, true);
}

void Boss::Jump() {
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	ChangeAnimation(ROUND_KING_STATE_JUMP, true);
}

void Boss::Die() {
	moving = false;
	ChangeAnimation(ROUND_KING_STATE_DEAD, true);
	SoundManager::soundManager.PlaySound("enemyDie");

	roundKingsKilled++;
}

void Boss::FindNearestTarget() {
	//TODO cleaner solution, same function is used in glob
	//Make an enemy class! and make grob, glob and king inherit from it
	target = NULL;
	//find closest target
	for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
		if ((*entity)->type == "hero" && (*entity)->active) {
			//if found first hero in list, just set them as the target for now
			if (target == NULL) {
				target = (Creature*)(*entity); //if cant cast to LivingEntity, throw casting exception
			}
			else {
				//otherwise, is this other hero closer then the previous target
				if (Entity::DistBetweenTwoEntities(this, (*entity)) < Entity::DistBetweenTwoEntities(this, target)) {
					target = (Creature*)(*entity);
				}
			}
		}
	}
}

void Boss::ChangeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;

	if (state == ROUND_KING_STATE_IDLE)
		currentAnim = animSet->GetAnimation(ROUND_KING_ANIM_IDLE);
	if (state == ROUND_KING_STATE_CHARGE)
		currentAnim = animSet->GetAnimation(ROUND_KING_ANIM_CHARGE);
	if (state == ROUND_KING_STATE_SHOOT)
		currentAnim = animSet->GetAnimation(ROUND_KING_ANIM_SHOOT);
	if (state == ROUND_KING_STATE_SLAM)
		currentAnim = animSet->GetAnimation(ROUND_KING_ANIM_SLAM);
	if (state == ROUND_KING_STATE_JUMP_TELEGRAPH)
		currentAnim = animSet->GetAnimation(ROUND_KING_ANIM_JUMP_TELEGRAPH);
	if (state == ROUND_KING_STATE_JUMP)
		currentAnim = animSet->GetAnimation(ROUND_KING_ANIM_JUMP);
	if (state == ROUND_KING_STATE_DEAD)
		currentAnim = animSet->GetAnimation(ROUND_KING_ANIM_DIE);

	if (resetFrameToBeginning)
		currentFrame = currentAnim->GetFrame(0);
	else
		currentFrame = currentAnim->GetFrame(currentFrame->frameNumber);
}

void Boss::UpdateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL) {
		return;
	}

	frameTimer += TimeManager::timeController.dT;
	//if frameTimer greater then frame duration:
	if (frameTimer >= currentFrame->duration) {
		// if at the end of animation
		if (currentFrame->frameNumber == currentAnim->GetEndFrameNumber()) {
			if (state == ROUND_KING_STATE_CHARGE)
				Shoot();
			else if (state == ROUND_KING_STATE_JUMP_TELEGRAPH)
				Jump();
			else if (state == ROUND_KING_STATE_SLAM || state == ROUND_KING_STATE_JUMP)
				ChangeAnimation(ROUND_KING_STATE_IDLE, true);
			else if (state == ROUND_KING_STATE_DEAD) {
				frameTimer = 0;
				//if somehow alive again
				if (hp > 0)
					ChangeAnimation(ROUND_KING_STATE_IDLE, true);
				else
					active = false;
			}
		}
		else { //not end of animation, so move to next frame normally
			currentFrame = currentAnim->GetNextFrame(currentFrame);
			//IF THE NEW FRAME IS DEALING DAMAGE, we might want to make sounds
			GroupType<float> *damages = dynamic_cast<GroupType<float>*>(GroupBuilder::FindGroupByName("damage", currentFrame->frameData));
			//if does damage
			if (damages != NULL && damages->GetGroupSize() > 0) {
				// and if its Slam state
				if (state == ROUND_KING_STATE_SLAM) {
					SoundManager::soundManager.PlaySound("smash");
				}
				else if (state == ROUND_KING_STATE_JUMP) {
					SoundManager::soundManager.PlaySound("crash");
				}
			}
		}
		frameTimer = 0;
	}
}

void Boss::UpdateDamages() {
	//am I hitable
	if (active && hp > 0 && invincibleTimer <= 0) {
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			if ((*entity)->active && (*entity)->type == "hero") {
				//cast entity to livingentity
				Creature *enemy = dynamic_cast<Creature*>((*entity));

				//if enemy is hitting me
				if (enemy->damage > 0 && collBetweenTwoRects(collisionBox, enemy->hitBox)) {
					enemy->HitLanded(this); //let attacker know they hit
					hp -= enemy->damage;

					if (hp > 0) {
						SoundManager::soundManager.PlaySound("enemyHit");
						invincibleTimer = 0.1;
					}
				}
			}
		}
	}
}
