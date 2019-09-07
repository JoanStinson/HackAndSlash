#pragma once
#include <string>
#include <iostream>
#include <SDL.h>
using namespace std;

namespace globals {

	//useful for me as a dev
	extern bool debugging;
	extern bool smoothCamera;

	//sdl related
	//extern int ScreenWidth, ScreenHeight, ScreenScale;
	//extern SDL_Renderer* renderer;
	// camera
	extern SDL_Rect camera;

	//clips off header. e.g "clip: 50 114 44 49" turns into "50 114 44 49"
	string clipOffDataHeader(string data);
	string getResourcePath(const string &subDir = "");

};