#include "Bullet.h"

#define ANIM_BULLET "bullet"

Bullet::Bullet(AnimationSet *animSet, int x, int y) {
	active = false;
	this->animSet = animSet;
	solid = false;
	collideWithSolids = true;
	dieOnSolids = true;
	moving = true;
	type = ENEMY;
	this->x = x;
	this->y = y;
	moveSpeed = 40;
	moveSpeedMax = 40;
	hp = hpMax = 1;
	damage = 5;
	collisionBox.w = collisionBoxW = 5;
	collisionBox.h = collisionBoxH = 5;
	collisionBoxYOffset = 0;

	direction = DOWN;
	ChangeAnimation(0, true);
	UpdateCollisionBox();
	invincibleTimer = 0;
}

void Bullet::Update() {
	UpdateCollisionBox();
	Move(angle);
	UpdateMovement();
	UpdateCollisions();
	UpdateHitBox();
	UpdateDamages();
	UpdateAnimation();

}

void Bullet::ChangeAnimation(int newState, bool resetAnim) {
	state = newState;
	currentAnim = animSet->GetAnimation(ANIM_BULLET);

	if (resetAnim)
		currentFrame = currentAnim->GetFrame(0);
	else
		currentFrame = currentAnim->GetFrame(currentFrame->GetFrameNumber());
}

void Bullet::UpdateAnimation() {
	if (currentFrame == nullptr || currentAnim == nullptr) return;

	// if got frames, update frameTimer and animation
	frameTimer += TM.GetDt();

	if (frameTimer >= currentFrame->GetDuration()) {
		// if its the end of the animation, restart it
		if (currentFrame->GetFrameNumber() == currentAnim->GetEndFrameNumber())
			currentFrame = currentAnim->GetFrame(0);
		else
			currentFrame = currentAnim->GetNextFrame(currentFrame);

		frameTimer = 0;
	}
}

void Bullet::HitLanded(Creature *entity) {
	//we crashed into an entity and damaged them, time to destroy bullet
	active = false;
}

void Bullet::CrashOntoSolid() {
	//for bullets, when they hit walls, destroy them
	//TODO object pooling
	active = false;
}

void Bullet::Reset(int x, int y) {
	active = true;
	this->x = x;
	this->y = y;
	moveSpeed = 40;
	moveSpeedMax = 40;
	hp = hpMax = 1;
	damage = 5;
	collisionBox.w = collisionBoxW = 5;
	collisionBox.h = collisionBoxH = 5;
	collisionBoxYOffset = 0;

	direction = DOWN;
	ChangeAnimation(0, true);
	UpdateCollisionBox();
	invincibleTimer = 0;
}
