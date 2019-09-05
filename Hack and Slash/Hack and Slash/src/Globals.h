#ifndef GLOBALS
#define GLOBALS

#include <string>
#include <iostream>
#include <SDL.h>
#include "randomNumber.h"

using namespace std;

namespace Globals{

	//math helpers
	extern const float PI;

	//useful for me as a dev
	extern bool debugging;

	//sdl related
	extern int ScreenWidth, ScreenHeight, ScreenScale;
	extern SDL_Renderer* renderer;

	//clips off header. e.g "clip: 50 114 44 49" turns into "50 114 44 49"
	string clipOffDataHeader(string data);
	
};

#endif