#pragma once
#include "Creature.h"
#include "SoundManager.h"

class Player : public Creature {
public:
	Player(AnimationSet *animSet);
	void Update();
	void Slash();
	void Dash();
	void Die();
	void Revive();
	void ChangeAnimation(int newState, bool resetFrameToBeginning);
	void UpdateAnimation();
	void UpdateDamages();
	enum State { IDLE, MOVE, SLASH, DASH, DEAD };

private:
};