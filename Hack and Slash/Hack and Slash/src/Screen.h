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

private:
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;
	SDL_Surface *m_surface;
	Uint32 *m_buffer;
};