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
	void UpdateTime();													//!< Update lastUpdate and dt
	void Pause();
	void UnPause();
	void Reset();
	float GetDt() const;

private:
	TimeManager();														//!< Private constructor so that no objects can be created (disallow instantiation outside of the class)
	TimeManager(const TimeManager&) = delete; 							//!< Delete copy constructor 
	TimeManager& operator=(const TimeManager&) = delete; 				//!< Delete copy assignment operator
	TimeManager(TimeManager &&) = delete; 								//!< Delete move constructor
	TimeManager& operator=(TimeManager &&) = delete; 					//!< Delete move assignment operator


	enum State { PLAY, PAUSE };
	int timeState;
	Uint32 lastUpdate;													/*!< Last time we checked how many ticks we were up to */
	float dt;															/*!< Delta time, in seconds (1= 1 second, 0.5 is half a second). Time since the last frame was rendered onto screen */
};