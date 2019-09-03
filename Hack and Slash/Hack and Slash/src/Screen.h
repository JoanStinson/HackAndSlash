#pragma once
#include <SDL.h>

class Screen {
public:
	Screen();
	bool Start();
	bool Update();
	void Quit();


	const static int SCREEN_WIDTH = 640;
	const static int SCREEN_HEIGHT = 352;
	const static int SCREEN_SCALE = 2;

private:
	SDL_Window *window;
	SDL_Texture *texture;
	SDL_Surface *surface;
	Uint32 *buffer;
};