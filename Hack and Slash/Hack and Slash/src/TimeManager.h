#pragma once
#include <iostream>
#include "SDL.h"

class TimeManager{
public:
	TimeManager();
	void UpdateTime();//update lastUpdate and dT
	void Pause();
	void UnPause();
	void Reset();

	//reference values
	//static const int PLAY_STATE = 0, PAUSE_STATE = 1;
	enum State { PLAY, PAUSE };

	int timeState;
	Uint32 lastUpdate; //last time we checked how many ticks we were up to
	float dT; //delta time, in seconds (1= 1 second, 0.5 is half a second). Time since the last frame was rendered onto screen

	static TimeManager timeController;
};