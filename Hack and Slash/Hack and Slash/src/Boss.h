#pragma once
#include <cstdlib>
#include "Creature.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "Enemy.h"

class Boss : public Enemy {
public:
	Boss(AnimationSet *animSet, AnimationSet *bulletAnimSet);
	~Boss();
	void Update();

	static int roundKingsKilled;

private:
	void UpdateShoot();
	void Think();
	void Charge();
	void Shoot();
	void Slam();
	void JumpTelegraph();
	void Jump();
	void Die();
	void ChangeAnimation(int newState, bool resetAnim = true);
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
	list<Bullet*> bullets;
	const int TOTAL_BULLETS = 10;
};
