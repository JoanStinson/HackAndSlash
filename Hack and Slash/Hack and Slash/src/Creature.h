#pragma once
#include "Entity.h"

//abstract class also
class Creature : public Entity {
public:
	Creature() = default;
	Creature(AnimationSet *animSet);
	void Draw() override; //overriding entities draw

	int hp;
	int maxHp;
	int damage;
	float invincibleTimer = 0; // if > 0, then cant be hurt
	SDL_Rect hitBox; //similar to collisionBox, but describes damaging area

protected:
	void CheckIfDead(int deadState);
	virtual void UpdateHitBox();
	virtual void UpdateDamages(const string &type, const string &hitSFX, const float invincibleTimer, const int slideAmount = 0); //how we get damaged by other things (up to each class to define)
	virtual void Die() = 0; //abstract. up to other classes to decide how they die
	virtual void UpdateInvincibleTimer();
	virtual void HitLanded(Creature *entity) {}
};