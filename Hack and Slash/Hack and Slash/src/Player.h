#pragma once
#include "Creature.h"
#include "SoundManager.h"

class Player : public Creature {
public:
	Player(AnimationSet *animSet);
	void Update() override;
	void Slash();
	void Dash();
	void Revive();

	enum State { IDLE, MOVE, SLASH, DASH, DEAD };

private:
	void ChangeAnimation(int newState, bool resetAnim = true) override;
	void Die() override;
	void UpdateAnimation();
	void UpdateDamages();
};