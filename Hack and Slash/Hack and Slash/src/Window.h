#pragma once
#include <SDL/SDL.h>
#include <string>
using namespace std;

#define WINDOW Window::Instance()

class Window {
public:
	inline static Window& Instance(const string &name = "", const int screenWidth = 0, const int screenHeight = 0, const int screenScale = 0) {
		static Window instance(move(name), move(screenWidth), move(screenHeight), move(screenScale));
		return instance;
	}

	inline SDL_Window* operator()() const {
		return window;
	}

	const int SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE;
	const string NAME;

private:
	// private constructor so that no objects can be created (disallow instantiation outside of the class)
	Window();
	Window(const string &name, const int screenWidth, const int screenHeight, const int screenScale);
	~Window();
	// delete copy constructor 
	Window(const Window&) = delete;
	// delete copy assignment operator
	Window& operator=(const Window&) = delete;
	// delete move constructor
	Window(Window &&) = delete;
	// delete move assignment operator
	Window& operator=(Window &&) = delete;

	SDL_Window *window = nullptr;
};
