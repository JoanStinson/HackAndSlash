#pragma once
#include <SDL.h>
#include <string>
#include "TimeController.h"
using namespace std;

namespace globals {

	// Math helpers
	 const float PI = 3.14f;
	static bool debugging;
	// extern
	static SDL_Renderer *renderer;//TODO fix
	static TimeController *timeController;

	// clips off header. e.g "clip: 50 114 44 49" turns into "50 114 44 49"
	string clipOffDataHeader(string data);
};