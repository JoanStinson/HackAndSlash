#include "TimeManager.h"

TimeManager::TimeManager() {
	dt = 0;
	lastUpdate = 0;
	timeState = PLAY;
}

void TimeManager::UpdateTime() {
	if (timeState == PAUSE) {
		dt = 0;
	}
	else {
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		dt = timeDiff / 1000.0; // Move these milliseconds into the decimal place area e.g 104 ticks = 0.104 seconds
	}
	lastUpdate = SDL_GetTicks();
}

void TimeManager::Pause() {
	timeState = PAUSE;
}

void TimeManager::UnPause() {
	timeState = PLAY;
}

void TimeManager::Reset() {
	dt = 0;
	lastUpdate = SDL_GetTicks();
}

float TimeManager::GetDt() const {
	return dt;
}
