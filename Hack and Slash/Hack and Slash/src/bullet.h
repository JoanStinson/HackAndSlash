#pragma once
#include <cstdlib>
#include "Creature.h"
#include "SoundManager.h"

class Bullet : public Creature {
public:
	Bullet(AnimationSet *animSet, int x, int y);
	void Update();
	void Die() {};
	void ChangeAnimation(int newState, bool resetFrameToBeginning);
	void UpdateAnimation();
	void UpdateDamages() { ; }
	void HitLanded(Creature *entity);
	virtual void CrashOntoSolid();

	static const string BULLET_ANIM_BULLET;
};