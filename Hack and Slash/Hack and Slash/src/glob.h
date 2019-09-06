#pragma once
#include <cstdlib>
#include "Creature.h"
#include "SoundManager.h"

class Glob : public Creature {
public:
	Glob(AnimationSet *animSet);
	void Update();
	void Think();
	void Telegraph();
	void Attack();
	void Die();
	void FindNearestTarget();
	void ChangeAnimation(int newState, bool resetFrameToBeginning);
	void UpdateAnimation();
	void UpdateDamages();

	static const string GLOB_ANIM_UP;
	static const string GLOB_ANIM_DOWN;
	static const string GLOB_ANIM_LEFT;
	static const string GLOB_ANIM_RIGHT;
	static const string GLOB_ANIM_IDLE_UP;
	static const string GLOB_ANIM_IDLE_DOWN;
	static const string GLOB_ANIM_IDLE_LEFT;
	static const string GLOB_ANIM_IDLE_RIGHT;
	static const string GLOB_ANIM_ATTACK_UP;
	static const string GLOB_ANIM_ATTACK_DOWN;
	static const string GLOB_ANIM_ATTACK_LEFT;
	static const string GLOB_ANIM_ATTACK_RIGHT;
	static const string GLOB_ANIM_TELEGRAPH_UP;
	static const string GLOB_ANIM_TELEGRAPH_DOWN;
	static const string GLOB_ANIM_TELEGRAPH_LEFT;
	static const string GLOB_ANIM_TELEGRAPH_RIGHT;
	static const string GLOB_ANIM_DIE;

	static const int GLOB_STATE_IDLE;
	static const int GLOB_STATE_MOVE;
	static const int GLOB_STATE_ATTACK;
	static const int GLOB_STATE_TELEGRAPH;
	static const int GLOB_STATE_DEAD;

	static const int GLOB_AI_NORMAL;
	static const int GLOB_AI_CHASE;

	static int globsKilled;

	//when this is 0, choose to do an action (AI)
	float thinkTimer = 0;
	//who should I chase
	Creature* target = NULL;

	int aiState = GLOB_AI_NORMAL;
};