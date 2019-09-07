#pragma once
#include <iostream>
#include "SDL.h"

#define TM TimeManager::Instance()

class TimeManager {
public:
	inline static TimeManager& Instance() {
		static TimeManager instance;
		return instance;
	}
	void UpdateTime();//update lastUpdate and dT
	void Pause();
	void UnPause();
	void Reset();
	float GetDt();

private:
	// private constructor so that no objects can be created (disallow instantiation outside of the class)
	TimeManager();

	// delete copy constructor 
	TimeManager(const TimeManager&) = delete;
	// delete copy assignment operator
	TimeManager& operator=(const TimeManager&) = delete;
	// delete move constructor
	TimeManager(TimeManager &&) = delete;
	// delete move assignment operator
	TimeManager& operator=(TimeManager &&) = delete;

	//reference values
	//static const int PLAY_STATE = 0, PAUSE_STATE = 1;
	enum State { PLAY, PAUSE };
	int timeState;
	Uint32 lastUpdate; //last time we checked how many ticks we were up to
	float dt; //delta time, in seconds (1= 1 second, 0.5 is half a second). Time since the last frame was rendered onto screen
};