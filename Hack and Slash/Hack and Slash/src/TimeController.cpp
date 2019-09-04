#include "timeController.h"
//builds a global timeController object for use anywhere
TimeController TimeController::timeController;

TimeController::TimeController(){
	dT = 0;
	lastUpdate = 0;
	timeState = PLAY_STATE;
}

void TimeController::updateTime(){
	if (timeState == PAUSE_STATE){
		dT = 0;
	}
	else{
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		dT = timeDiff / 1000.0; //move these milliseconds into the decimal place area e.g 104 ticks = 0.104 seconds
	}
	lastUpdate = SDL_GetTicks();
}
void TimeController::pause(){
	timeState = PAUSE_STATE;
}
void TimeController::resume(){
	timeState = PLAY_STATE;
}
void TimeController::reset(){
	dT = 0;
	lastUpdate = SDL_GetTicks();
}