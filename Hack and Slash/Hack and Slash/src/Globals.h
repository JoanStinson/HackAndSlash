#ifndef GLOBALS
#define GLOBALS

#include <string>
#include <iostream>
#include <SDL.h>
#include "randomNumber.h"

using namespace std;

namespace Globals{

	//math helpers
	static const float PI = M_PI;

	//useful for me as a dev
	static bool debugging = true;

	//sdl related
	static int ScreenWidth = 640, ScreenHeight = 352, ScreenScale = 2;
	extern SDL_Renderer* renderer;

	//clips off header. e.g "clip: 50 114 44 49" turns into "50 114 44 49"
	static string clipOffDataHeader(string data) {
		int pos = data.find(":", 0);//returns where we find the : in the string, otherwise, return -1
		if (pos != -1) {
			data = data.substr(pos + 1, data.length() - pos + 2);
		}
		return data;
	}

	
};

#endif