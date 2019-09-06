#include "bullet.h"

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
	changeAnimation(0, true);
	updateCollisionBox();
	invincibleTimer = 0;
}

void Bullet::update() {
	updateCollisionBox();
	move(angle);
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();
	updateAnimation();

}

void Bullet::changeAnimation(int newState, bool resetFrameToBeginning) {
	state = newState;
	currentAnim = animSet->getAnimation(BULLET_ANIM_BULLET);

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void Bullet::updateAnimation() {
	if (currentFrame == NULL || currentAnim == NULL) return;

	// if got frames, update frameTimer and animation
	frameTimer += TimeController::timeController.dT;

	if (frameTimer >= currentFrame->duration) {
		// if its the end of the animation, restart it
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber())
			currentFrame = currentAnim->getFrame(0);
		else
			currentFrame = currentAnim->getNextFrame(currentFrame);

		frameTimer = 0;
	}
}

void Bullet::hitLanded(LivingEntity * entity) {
	//we crashed into an entity and damaged them, time to destroy bullet
	//TODO USE OBJECT POOLING
	active = false;

	//TODO create hit effect
}

void Bullet::crashOntoSolid() {
	//for bullets, when they hit walls, destroy them
	//TODO object pooling
	active = false;
	//TODO create hit effect
}
