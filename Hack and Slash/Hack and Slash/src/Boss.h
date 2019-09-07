#pragma once
#include <cstdlib>
#include "Creature.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "Enemy.h"

class Boss : public Enemy {
public:

	Boss(AnimationSet *animSet, AnimationSet *bulletAnimSet);
	void Update();
	void UpdateShoot();
	void Think();
	void Charge();
	void Shoot();
	void Slam();
	void JumpTelegraph();
	void Jump();
	void Die();
	void ChangeAnimation(int newState, bool resetFrameToBeginning);
	void UpdateAnimation();
	void UpdateDamages();

	enum State { IDLE, CHARGE, SHOOT, SLAM, JUMP_TELEGRAPH, JUMP, DEAD };
	enum Phase { NORMAL, DAMAGED, FRENETIC };

	//how long to shoot for
	float shootTimer = 0;
	//how long between bullets
	float shotTimer = 0;
	//reference to bullet animations
	AnimationSet *bulletAnimSet;
	static int roundKingsKilled;
};