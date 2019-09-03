#include "TimeController.h"

TimeController::TimeController(): /*dt(0),*/ lastUpdate(0), timeState(PLAY_STATE) {

}

TimeController::~TimeController() {

}

void TimeController::Play() {
	if (timeState == PAUSE_STATE) {
		dt = 0;
	}
	else {
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		dt = timeDiff / 1000; // move these ms into the decimal place are e.g 104ticks = 0.104s
	}
	lastUpdate = SDL_GetTicks();
}

void TimeController::Pause() {
	timeState = PAUSE_STATE;
}

void TimeController::UnPause() {
	timeState = PLAY_STATE;
}

void TimeController::Reset() {
	dt = 0;
	lastUpdate = SDL_GetTicks();
}

static float dt;