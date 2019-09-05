#include "globals.h"

namespace Globals {

	const float PI = M_PI;

	bool debugging = false;
	bool smoothCamera = true;
	
	int ScreenWidth = 640, ScreenHeight = 352, ScreenScale = 2;
	SDL_Renderer* renderer = NULL;

	SDL_Rect camera;

	string clipOffDataHeader(string data) {
		int pos = data.find(":", 0);//returns where we find the : in the string, otherwise, return -1
		if (pos != -1) {
			data = data.substr(pos + 1, data.length() - pos + 2);
		}
		return data;
	}
}