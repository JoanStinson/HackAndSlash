#include "TimeController.h"

TimeController::TimeController(): m_dt(0), m_lastUpdate(0), m_timeState(PLAY_STATE) {

}

TimeController::~TimeController() {

}

void TimeController::Play() {
	if (m_timeState == PAUSE_STATE) {
		m_dt = 0;
	}
	else {
		Uint32 timeDiff = SDL_GetTicks() - m_lastUpdate;
		m_dt = timeDiff / 1000; // move these ms into the decimal place are e.g 104ticks = 0.104s
	}
	m_lastUpdate = SDL_GetTicks();
}

void TimeController::Pause() {
	m_timeState = PAUSE_STATE;
}

void TimeController::UnPause() {
	m_timeState = PLAY_STATE;
}

void TimeController::Reset() {
	m_dt = 0;
	m_lastUpdate = SDL_GetTicks();
}
