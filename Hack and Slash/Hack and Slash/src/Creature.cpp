#include "Creature.h"

void Creature::UpdateHitBox() {
	//assume damage is 0 for now
	damage = 0;

	GroupType<SDL_Rect>* hitBoxes = dynamic_cast<GroupType<SDL_Rect>*>(GroupBuilder::FindGroupByName("hitBox", currentFrame->frameData));
	if (hitBoxes != NULL && hitBoxes->GetGroupSize() > 0) {
		//update hitbox
		SDL_Rect hb = hitBoxes->GetBoxData().front();
		hitBox.x = x - currentFrame->offSet.x + hb.x;
		hitBox.y = y - currentFrame->offSet.y + hb.y;
		hitBox.w = hb.w;
		hitBox.h = hb.h;

		//update damage
		GroupType<float>* damages = dynamic_cast<GroupType<float>*>(GroupBuilder::FindGroupByName("damage", currentFrame->frameData));
		if (damages != NULL && damages->GetGroupSize() > 0) {
			damage = damages->GetNumData().front();
		}
	}
}

void Creature::UpdateInvincibleTimer() {
	if (invincibleTimer > 0) {
		invincibleTimer -= TimeManager::timeController.dT;
	}
}

void Creature::Draw() {
	if (currentFrame != NULL && active) {
		if (invincibleTimer > 0 && animSet->whiteSpriteSheet != NULL) {
			currentFrame->Draw(animSet->whiteSpriteSheet, x - globals::camera.x, y - globals::camera.y);
		}
		else {
			currentFrame->Draw(animSet->spriteSheet, x - globals::camera.x, y - globals::camera.y);
		}

	}
	//draw collsionBox
	if (solid && globals::debugging) {
		//sets the current drawing colour (Doesn't affect textures and what not)
		SDL_SetRenderDrawColor(globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(globals::renderer, &lastCollisionBox);

		SDL_SetRenderDrawColor(globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(globals::renderer, &collisionBox);

		SDL_SetRenderDrawColor(globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(globals::renderer, &hitBox);
	}
}