#pragma once
#include <cstdlib> //need it for random numbers
#include "Creature.h"
#include "SoundManager.h"
#include "Enemy.h"

class Grob : public Enemy {
public:
	//main functions
	Grob(AnimationSet *animSet);
	void Update();
	void Think();
	void Die();
	void ChangeAnimation(int newState, bool resetFrameToBegging);

	void UpdateAnimation();
	void UpdateHitBox();
	void UpdateDamages();

	enum State { IDLE, MOVE, DEAD };

	static int grobsKilled;

};