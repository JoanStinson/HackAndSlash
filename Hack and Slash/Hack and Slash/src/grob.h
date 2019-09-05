#pragma once
#ifndef GROB
#define GROB

#include <cstdlib> //need it for random numbers
#include "livingEntity.h"
#include "soundManager.h"

class Grob : public LivingEntity {
public:
	static const string GROB_ANIM_UP;
	static const string GROB_ANIM_DOWN;
	static const string GROB_ANIM_LEFT;
	static const string GROB_ANIM_RIGHT;
	static const string GROB_ANIM_IDLE_UP;
	static const string GROB_ANIM_IDLE_DOWN;
	static const string GROB_ANIM_IDLE_LEFT;
	static const string GROB_ANIM_IDLE_RIGHT;
	static const string GROB_ANIM_DIE;

	static const int GROB_STATE_IDLE;
	static const int GROB_STATE_MOVE;
	static const int GROB_STATE_DEAD;


	static int grobsKilled;

	//when this is 0, think of something new to do (used for AI)
	float thinkTimer = 0;

	//main functions
	Grob(AnimationSet *animSet);
	void update();
	void think();
	void die();
	void changeAnimation(int newState, bool resetFrameToBegging);

	void updateAnimation();
	void updateHitBox();
	void updateDamages();
};

#endif