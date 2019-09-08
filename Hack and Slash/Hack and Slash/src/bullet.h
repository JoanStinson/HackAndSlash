#pragma once
#include <cstdlib>
#include "Creature.h"
#include "SoundManager.h"

class Bullet : public Creature {
public:
	Bullet(AnimationSet *animSet, int x, int y);
	void Update();
	void Die() {};
	void ChangeAnimation(int newState, bool resetAnim = true);
	void UpdateAnimation();
	void UpdateDamages() { ; }
	void HitLanded(Creature *entity);
	virtual void CrashOntoSolid();
	void Reset(int x, int y);
};