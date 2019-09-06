#include "TimeManager.h"

//builds a global timeController object for use anywhere
TimeManager TimeManager::timeController;

TimeManager::TimeManager(){
	dT = 0;
	lastUpdate = 0;
	timeState = PLAY;
}

void TimeManager::UpdateTime(){
	if (timeState == PAUSE){
		dT = 0;
	}
	else{
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		dT = timeDiff / 1000.0; //move these milliseconds into the decimal place area e.g 104 ticks = 0.104 seconds
	}
	lastUpdate = SDL_GetTicks();
}

void TimeManager::Pause(){
	timeState = PAUSE;
}

void TimeManager::UnPause(){
	timeState = PLAY;
}

void TimeManager::Reset(){
	dT = 0;
	lastUpdate = SDL_GetTicks();
}