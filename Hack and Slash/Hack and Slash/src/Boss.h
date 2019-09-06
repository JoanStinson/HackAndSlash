#pragma once
#include <cstdlib>
#include "Creature.h"
#include "Bullet.h"
#include "SoundManager.h"

class Boss : public Creature {
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
	void FindNearestTarget();//TODO move find nearest target to somewhere better, maybe static
	void ChangeAnimation(int newState, bool resetFrameToBeginning);
	void UpdateAnimation();
	void UpdateDamages();

	// animations
	static const string ROUND_KING_ANIM_IDLE;
	static const string ROUND_KING_ANIM_CHARGE;
	static const string ROUND_KING_ANIM_SHOOT;
	static const string ROUND_KING_ANIM_SLAM;
	static const string ROUND_KING_ANIM_JUMP_TELEGRAPH;
	static const string ROUND_KING_ANIM_JUMP;
	static const string ROUND_KING_ANIM_DIE;

	// states 
	static const int ROUND_KING_STATE_IDLE;
	static const int ROUND_KING_STATE_CHARGE;
	static const int ROUND_KING_STATE_SHOOT;
	static const int ROUND_KING_STATE_SLAM;
	static const int ROUND_KING_STATE_JUMP_TELEGRAPH;
	static const int ROUND_KING_STATE_JUMP;
	static const int ROUND_KING_STATE_DEAD;

	//phases
	static const int ROUND_KING_PHASE_NORMAL;
	static const int ROUND_KING_PHASE_DAMAGED;
	static const int ROUND_KING_PHASE_FRENETIC;

	//when zero, do new action
	float thinkTimer = 0;
	//how long to shoot for
	float shootTimer = 0;
	//how long between bullets
	float shotTimer = 0;
	//who should I case
	Creature *target = NULL;

	//current phase for boss
	int aiState = ROUND_KING_PHASE_NORMAL;
	//reference to bullet animations
	AnimationSet *bulletAnimSet;

	static int roundKingsKilled;


};
