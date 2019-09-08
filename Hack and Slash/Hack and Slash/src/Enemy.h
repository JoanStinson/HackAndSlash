#pragma once
#include "Creature.h"

class Enemy : public Creature {
public:
	Enemy() {}
	Enemy(AnimationSet *animSet);
	void FindPlayer();

protected:
	Creature *target = nullptr;										/*!< Who should I chase */
	int aiState;													/*!< The current state of the AI */
	float thinkTimer = 0;											/*!< When this is 0, think of something new to do (used for AI) */
};