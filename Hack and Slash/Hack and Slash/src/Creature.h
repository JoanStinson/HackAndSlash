#pragma once
#include "Entity.h"

//abstract class also
class Creature : public Entity {
public:
	virtual void UpdateHitBox();
	virtual void UpdateDamages() = 0; //how we get damaged by other things (up to each class to define)
	virtual void Die() = 0; //abstract. up to other classes to decide how they die
	virtual void UpdateInvincibleTimer();
	virtual void HitLanded(Creature *entity) { ; }

	//TODO put override to all overrided methods
	//TODO put const references and pointers in all functions
	//TODO refactor names uppercamelcase
	//TODO NULLS to nullptr and C++ 11, auto for based loops, namespaces, etc.
	void Draw() override; //overriding entities draw


	int hp, hpMax;
	int damage = 0;
	SDL_Rect hitBox; //similar to collisionBox, but describes damaging area

	float invincibleTimer = 0; // if > 0, then cant be hurt


};