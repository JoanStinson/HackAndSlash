#ifndef LIVINGENTITY
#define LIVINGENTITY

#include "entity.h"

//abstract class also
class LivingEntity : public Entity{
public:
	int hp, hpMax;
	int damage = 0;
	SDL_Rect hitBox; //similar to collisionBox, but describes damaging area

	float invincibleTimer = 0; // if > 0, then cant be hurt

	virtual void updateHitBox();
	virtual void updateDamages() = 0; //how we get damaged by other things (up to each class to define)
	virtual void die() = 0; //abstract. up to other classes to decide how they die
	virtual void updateInvincibleTimer();

	//TODO put override to all overrided methods
	//TODO put const references and pointers in all functions
	//TODO refactor names uppercamelcase
	//TODO NULLS to nullptr and C++ 11, auto for based loops, namespaces, etc.
	void draw() override; //overriding entities draw
};

#endif