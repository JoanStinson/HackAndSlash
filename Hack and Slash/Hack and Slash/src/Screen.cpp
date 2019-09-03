#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "Screen.h"
#include "ResPath.h"
#include "DrawingFunctions.h"
using namespace std;

Screen::Screen() : m_window(nullptr), /*m_renderer(nullptr),*/ m_texture(nullptr), m_buffer(nullptr) {

}

bool Screen::Start() {
	// Setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Error initialising SDL!" << endl;
		return false;
	}

	// Setup window
	m_window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH*SCREEN_SCALE, SCREEN_HEIGHT*SCREEN_SCALE,
		SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI); // SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	if (m_window == nullptr) {
		SDL_Quit();
		cout << "Window error!" << endl;
		return false;
	}

	// Setup renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_renderer == nullptr) {
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		cout << "Renderer error!" << endl;
		return false;
	}
	// This is the size to draaaw things at, before we scale it to the screen size dimensions mentioned in createWindow
	SDL_RenderSetLogicalSize(m_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Initialise sdl_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_Quit();
		cout << "SDL image did not initialise!" << endl;
		return false;
	}

	// Initialise sdl_ttf (text to font)
	if (TTF_Init() != 0) {
		SDL_Quit();
		cout << "SDL ttf did not initialise!" << endl;
		return false;
	}

	// Initialise sdl_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		SDL_Quit();
		cout << "Mixer did not initialise!" << endl;
		return false;
	}

	// Load up a texture to draw
	string resPath = getResourcePath();
	m_texture = loadTexture(resPath + "map.png", m_renderer);

	return true;
}

bool Screen::Update() {
	// Run game for 5000 ticks (5000ms)
	while (SDL_GetTicks() < 5000) {
		// Clear the screen
		SDL_RenderClear(m_renderer);
		// Draw what we want to the screen
		renderTexture(m_texture, m_renderer, 0, 0);
		// Show image we've been rendering
		SDL_RenderPresent(m_renderer);
	}
	return true;
}

void Screen::Quit() {
	delete[] m_buffer;
	SDL_DestroyRenderer(m_renderer);
	SDL_FreeSurface(m_surface);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

SDL_Renderer* Screen::GetRenderer() {
	return m_renderer;
}
