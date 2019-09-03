#pragma once
#include "SDL.h"

class TimeController {
public:
	TimeController();
	~TimeController();

	void Play(); // update m_lastUpdate and m_dt
	void Pause();
	void UnPause();
	void Reset();

	// builds a global TimeController object for use anywhere
	
	static const int PLAY_STATE = 0;
	static const int PAUSE_STATE = 1;
	int timeState;
	Uint32 lastUpdate; // last time we checked how many ticks we were up to
	 float dt; // delta time, in seconds (1=1 second, 0.5 is half a second). Time since the last frame was rendered onto screen
};

