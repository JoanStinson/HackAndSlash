#ifndef TIMECONTROLLER
#define TIMECONTROLLER

#include <iostream>
#include "SDL.h"

class TimeController{
public:
	//reference values
	//static const int PLAY_STATE = 0, PAUSE_STATE = 1;
	enum State { PLAY, PAUSE };

	int timeState;
	Uint32 lastUpdate; //last time we checked how many ticks we were up to
	float dT; //delta time, in seconds (1= 1 second, 0.5 is half a second). Time since the last frame was rendered onto screen

	TimeController();
	void updateTime();//update lastUpdate and dT
	void pause();
	void resume();
	void reset();

	static TimeController timeController;
};


#endif