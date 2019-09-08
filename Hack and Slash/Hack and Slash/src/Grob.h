#pragma once
#include <cstdlib> 
#include "Creature.h"
#include "SoundManager.h"
#include "Enemy.h"

class Grob : public Enemy {
public:
	Grob(AnimationSet *animSet);
	void Update() override;
	static int grobsKilled;

private:
	void Think();
	void Die() override;
	void ChangeAnimation(int newState, bool resetAnim = true) override;
	void UpdateAnimation();
	void UpdateHitBox();
	void UpdateDamages();

	enum State { IDLE, MOVE, DEAD };
};
