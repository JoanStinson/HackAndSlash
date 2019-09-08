#pragma once
#include "Player.h"

#define IM InputManager::Instance()

class InputManager {
public:
	inline static InputManager& Instance() {
		static InputManager instance;
		return instance;
	}
	void Update(SDL_Event* e);
	Player *player;

private:
	// private constructor so that no objects can be created (disallow instantiation outside of the class)
	InputManager() = default;
	//~InputManager();
	// delete copy constructor 
	InputManager(const InputManager&) = delete;
	// delete copy assignment operator
	InputManager& operator=(const InputManager&) = delete;
	// delete move constructor
	InputManager(InputManager &&) = delete;
	// delete move assignment operator
	InputManager& operator=(InputManager &&) = delete;

	enum Key {
		UP = SDL_SCANCODE_UP,
		DOWN = SDL_SCANCODE_DOWN,
		LEFT = SDL_SCANCODE_LEFT,
		RIGHT = SDL_SCANCODE_RIGHT,
		X = SDL_SCANCODE_X,
		Z = SDL_SCANCODE_Z
	};
};