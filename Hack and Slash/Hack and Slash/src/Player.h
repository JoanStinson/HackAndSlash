#pragma once
#include "Creature.h"
#include "SoundManager.h"

class Player : public Creature {
public:
	Player(AnimationSet *animSet);

	void ChangeAnimation(int newState, bool resetAnim = true) override;
	void Update() override;
	void Die() override;

	void Slash();
	void Dash();
	void Revive();
	void UpdateAnimation();
	void UpdateDamages();

	enum State { IDLE, MOVE, SLASH, DASH, DEAD };
};