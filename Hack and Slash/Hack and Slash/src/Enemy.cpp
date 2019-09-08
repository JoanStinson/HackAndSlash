#include "Enemy.h"

Enemy::Enemy(AnimationSet *animSet) : Creature(animSet) {
	type = ENEMY;
}

void Enemy::FindPlayer() {
	target = nullptr;
	// Find closest target
	for each (auto *entity in Entity::entities) {
		if (entity->type == PLAYER && entity->active) {
			// If found first in list, just set them as the target for now
			if (target == nullptr) {
				target = dynamic_cast<Creature*>(entity); // If cant cast to LivingEntity, throw casting exception
			}
			else {
				// Otherwise, is this other hero closer then the previous target
				if (Entity::DistBetweenTwoEntities(this, entity) < Entity::DistBetweenTwoEntities(this, target)) {
					target = dynamic_cast<Creature*>(entity);
				}
			}
		}
	}
}
