#include "Bullet.h"

const string Bullet::BULLET_ANIM_BULLET = "bullet";

Bullet::Bullet(AnimationSet * animSet, int x, int y) {
	this->animSet = animSet;
	solid = false;
	collideWithSolids = true;
	dieOnSolids = true;
	moving = true;
	type = "enemy";
	this->x = x;
	this->y = y;
	moveSpeed = 40;
	moveSpeedMax = 40;
	hp = hpMax = 1;
	damage = 5;
	collisionBoxW = 5;
	collisionBoxH = 5;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
	collisionBoxYOffset = 0;

	direction = DIR_DOWN;
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

void Bullet::ChangeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;
	currentAnim = animSet->GetAnimation(BULLET_ANIM_BULLET);

	if (resetFrameToBeginning)
		currentFrame = currentAnim->GetFrame(0);
	else
		currentFrame = currentAnim->GetFrame(currentFrame->frameNumber);
}

void Bullet::UpdateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL) return;

	// if got frames, update frameTimer and animation
	frameTimer += TimeManager::timeController.dT;

	if (frameTimer >= currentFrame->duration) {
		// if its the end of the animation, restart it
		if (currentFrame->frameNumber == currentAnim->GetEndFrameNumber())
			currentFrame = currentAnim->GetFrame(0);
		else
			currentFrame = currentAnim->GetNextFrame(currentFrame);

		frameTimer = 0;
	}
}

void Bullet::HitLanded(Creature *entity) {
	//we crashed into an entity and damaged them, time to destroy bullet
	//TODO USE OBJECT POOLING
	active = false;

	//TODO create hit effect
}

void Bullet::CrashOntoSolid() {
	//for bullets, when they hit walls, destroy them
	//TODO object pooling
	active = false;
	//TODO create hit effect
}
