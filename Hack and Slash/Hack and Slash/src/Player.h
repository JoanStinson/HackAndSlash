#pragma once
#include "Globals.h"
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

	//refernce variables
	static const string HERO_ANIM_UP;
	static const string HERO_ANIM_DOWN;
	static const string HERO_ANIM_LEFT;
	static const string HERO_ANIM_RIGHT;
	static const string HERO_ANIM_IDLE_UP;
	static const string HERO_ANIM_IDLE_DOWN;
	static const string HERO_ANIM_IDLE_LEFT;
	static const string HERO_ANIM_IDLE_RIGHT;
	static const string HERO_ANIM_SLASH_UP;
	static const string HERO_ANIM_SLASH_DOWN;
	static const string HERO_ANIM_SLASH_LEFT;
	static const string HERO_ANIM_SLASH_RIGHT;
	static const string HERO_ANIM_DASH_UP;
	static const string HERO_ANIM_DASH_DOWN;
	static const string HERO_ANIM_DASH_LEFT;
	static const string HERO_ANIM_DASH_RIGHT;
	static const string HERO_ANIM_DIE;

	static const int HERO_STATE_IDLE;
	static const int HERO_STATE_MOVE;
	static const int HERO_STATE_SLASH;
	static const int HERO_STATE_DASH;
	static const int HERO_STATE_DEAD;
};