#include "Wall.h"

Wall::Wall(AnimationSet *animSet) {
	this->animSet = animSet;

	//basic setup
	solid = true;

	collisionBoxW = 32;
	collisionBoxH = 32;
	collisionBoxYOffset = -16;

	UpdateCollisionBox();
	ChangeAnimation(0, false);

}

void Wall::Update() {
	UpdateCollisionBox();

	if (currentFrame == nullptr || currentAnim == nullptr)
		return;

	frameTimer += TM.GetDt();

	if (frameTimer >= currentFrame->GetDuration()) {
		currentFrame = currentAnim->GetNextFrame(currentFrame);

		frameTimer = 0;
	}
}

void Wall::ChangeAnimation(int newState, bool resetAnim) {
	currentAnim = animSet->GetAnimation("wall");
	currentFrame = currentAnim->GetFrame(0);
}