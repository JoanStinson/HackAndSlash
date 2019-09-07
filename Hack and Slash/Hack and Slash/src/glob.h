#pragma once
#include <cstdlib>
#include "Creature.h"
#include "SoundManager.h"
#include "Enemy.h"

class Glob : public Enemy {
public:
	Glob(AnimationSet *animSet);
	void Update();
	void Think();
	void Telegraph();
	void Attack();
	void Die();
	void ChangeAnimation(int newState, bool resetFrameToBeginning);
	void UpdateAnimation();
	void UpdateDamages();

	enum State { IDLE, MOVE, ATTACK, TELEGRAPH, DEAD };
	enum Phase { NORMAL, CHASE };

	static int globsKilled;

};