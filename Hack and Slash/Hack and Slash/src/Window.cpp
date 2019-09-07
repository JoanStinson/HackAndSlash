#include "Window.h"
#include <iostream>

Window::Window(const string &name, const int screenWidth, const int screenHeight, const int screenScale)
	: NAME(name), SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight), SCREEN_SCALE(screenScale) {
	//setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "error initialising SDL" << endl;
		//return 1;
	}

	//setup window
	window = SDL_CreateWindow("Hack and Slash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH*SCREEN_SCALE, SCREEN_HEIGHT*SCREEN_SCALE, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI); //SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	if (window == nullptr) {
		SDL_Quit();
		cout << "window error" << endl;
		//return 1;
	}
}

Window::~Window() {
	SDL_DestroyWindow(window), window = nullptr;
	SDL_Quit();
}
