#pragma once
#include "Creature.h"

class Enemy : public Creature {
public:
	Enemy() {}
	Enemy(AnimationSet *animSet);
	void FindPlayer();

protected:
	//who should I case
	Creature *target = nullptr;
	int aiState;
	//when this is 0, think of something new to do (used for AI)
	float thinkTimer = 0;
};