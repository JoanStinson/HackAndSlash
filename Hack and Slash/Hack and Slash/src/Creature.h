#pragma once
#include "Entity.h"

/*! Abstract class for entities that move */
class Creature : public Entity {
public:
	Creature() = default;
	Creature(AnimationSet *animSet);
	void Draw() override;												//!< Overriding entities draw

	int hp;
	int maxHp;
	int damage;
	float invincibleTimer = 0;											/*!< if > 0, then can't be hurt */
	SDL_Rect hitBox;													/*!< Similar to collisionBox, but describes damaging area */

protected:
	void CheckIfDead(int deadState);
	virtual void UpdateHitBox();
	virtual void UpdateDamages(const string &type, const string &hitSFX,//!< How we get damaged by other things (up to each class to define) 
		const float invincibleTimer, const int slideAmount = 0); 
	virtual void Die() = 0;												//!< Abstract, up to other classes to decide how they die
	virtual void UpdateInvincibleTimer();
	virtual void HitLanded(Creature *entity) {}
};