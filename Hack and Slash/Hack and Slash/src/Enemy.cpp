#include "Enemy.h"

Enemy::Enemy(AnimationSet *animSet) : Creature(animSet) {
	type = ENEMY;
}

void Enemy::FindPlayer() {
	target = nullptr;
	//find closest target
	for each (auto *entity in Entity::entities) {
		if (entity->type == PLAYER && entity->active) {
			//if found first hero in list, just set them as the target for now
			if (target == nullptr) {
				target = dynamic_cast<Creature*>(entity); //if cant cast to LivingEntity, throw casting exception
			}
			else {
				//otherwise, is this other hero closer then the previous target
				if (Entity::DistBetweenTwoEntities(this, entity) < Entity::DistBetweenTwoEntities(this, target)) {
					target = dynamic_cast<Creature*>(entity);
				}
			}
		}
	}
}
