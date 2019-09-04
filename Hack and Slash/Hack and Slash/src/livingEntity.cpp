#include "livingEntity.h"

void LivingEntity::updateHitBox(){
	//assume damage is 0 for now
	damage = 0;

	GroupBox* hitBoxes = (GroupBox*)GroupBuilder::findGroupByName("hitBox", currentFrame->frameData);
	if (hitBoxes != NULL && hitBoxes->numberOfDataInGroup() > 0){
		//update hitbox
		SDL_Rect hb = hitBoxes->data.front();
		hitBox.x = x - currentFrame->offSet.x + hb.x;
		hitBox.y = y - currentFrame->offSet.y + hb.y;
		hitBox.w = hb.w;
		hitBox.h = hb.h;

		//update damage
		GroupNumber* damages = (GroupNumber*)GroupBuilder::findGroupByName("damage", currentFrame->frameData);
		if (damages != NULL && damages->numberOfDataInGroup() > 0){
			damage = damages->data.front();
		}
	}
}
void LivingEntity::updateInvincibleTimer(){
	if (invincibleTimer > 0){
		invincibleTimer -= TimeController::timeController.dT;
	}
}
void LivingEntity::draw(){
	if (currentFrame != NULL && active){
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	//draw collsionBox
	if (solid && Globals::debugging){
		//sets the current drawing colour (Doesn't affect textures and what not)
		SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);

		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &hitBox);
	}
}