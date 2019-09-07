#include "Creature.h"
#include "Math.h"
#include "SoundManager.h"

void Creature::UpdateHitBox() {
	//assume damage is 0 for now
	damage = 0;

	GroupType<SDL_Rect>* hitBoxes = dynamic_cast<GroupType<SDL_Rect>*>(GroupBuilder::FindGroupByName("hitBox", currentFrame->frameData));
	if (hitBoxes != nullptr && hitBoxes->GetGroupSize() > 0) {
		//update hitbox
		SDL_Rect hb = hitBoxes->GetBoxData().front();
		hitBox.x = x - currentFrame->offSet.x + hb.x;
		hitBox.y = y - currentFrame->offSet.y + hb.y;
		hitBox.w = hb.w;
		hitBox.h = hb.h;

		//update damage
		GroupType<float>* damages = dynamic_cast<GroupType<float>*>(GroupBuilder::FindGroupByName("damage", currentFrame->frameData));
		if (damages != nullptr && damages->GetGroupSize() > 0) {
			damage = damages->GetNumData().front();
		}
	}
}

void Creature::UpdateDamages(const string &type, const string &hitSFX, const float invincibleTimer, const int slideAmount) {
	if (active && hp > 0 && this->invincibleTimer <= 0) {
		for each (auto *entity in Entity::entities) {
			if (entity->active && entity->type == type) {
				//we know enemies are living entities, so cast it to that
				Creature* enemy = dynamic_cast<Creature*>(entity);

				if (enemy->damage > 0 && math::collBetweenTwoRects(collisionBox, enemy->hitBox)) {
					enemy->HitLanded(this); //let attacker know they hit
					hp -= enemy->damage;

					//still alive!!
					if (hp > 0) {
						SM.PlaySound(hitSFX);
						this->invincibleTimer = invincibleTimer;
					}

					if (slideAmount != 0) {
						slideAngle = Entity::AngleBetweenTwoEntities(entity, this);
						this->slideAmount = slideAmount;
					}
				}
			}
		}
	}
}

void Creature::UpdateInvincibleTimer() {
	if (invincibleTimer > 0) {
		invincibleTimer -= TM.GetDt();
	}
}

void Creature::CheckIfDead(int deadState) {
	if (hp < 1 && state != deadState) {
		ChangeAnimation(deadState, true);
		moving = false;
		Die();
	}
}

void Creature::Draw() {
	if (currentFrame != nullptr && active) {
		if (invincibleTimer > 0 && animSet->whiteSpriteSheet != nullptr) {
			currentFrame->Draw(animSet->whiteSpriteSheet, x - globals::camera.x, y - globals::camera.y);
		}
		else {
			currentFrame->Draw(animSet->spriteSheet, x - globals::camera.x, y - globals::camera.y);
		}

	}
	//draw collsionBox
	if (solid && globals::debugging) {
		//sets the current drawing colour (Doesn't affect textures and what not)
		SDL_SetRenderDrawColor(RENDERER(), 0, 0, 255, 255);
		SDL_RenderDrawRect(RENDERER(), &lastCollisionBox);

		SDL_SetRenderDrawColor(RENDERER(), 255, 0, 0, 255);
		SDL_RenderDrawRect(RENDERER(), &collisionBox);

		SDL_SetRenderDrawColor(RENDERER(), 255, 0, 255, 255);
		SDL_RenderDrawRect(RENDERER(), &hitBox);
	}
}