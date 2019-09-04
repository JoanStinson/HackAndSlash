#include "wall.h"

Wall::Wall(AnimationSet *animSet){
	this->animSet = animSet;

	//basic setup
	solid = true;

	collisionBoxW = 32;
	collisionBoxH = 32;
	collisionBoxYOffset = -16;

	updateCollisionBox();
	changeAnimation(0, false);

}
void Wall::update(){
	updateCollisionBox();

	if (currentFrame == NULL || currentAnim == NULL)
		return;

	frameTimer += TimeController::timeController.dT;

	if (frameTimer >= currentFrame->duration){
		currentFrame = currentAnim->getNextFrame(currentFrame);

		frameTimer = 0;
	}
}
void Wall::changeAnimation(int newState, bool resetFrameToBeginning){
	currentAnim = animSet->getAnimation("wall");
	currentFrame = currentAnim->getFrame(0);
}