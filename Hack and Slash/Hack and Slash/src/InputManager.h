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
	InputManager() = default;												//!< Private constructor so that no objects can be created (disallow instantiation outside of the class)
	InputManager(const InputManager&) = delete; 							//!< Delete copy constructor 
	InputManager& operator=(const InputManager&) = delete; 					//!< Delete copy assignment operator
	InputManager(InputManager &&) = delete; 								//!< Delete move constructor
	InputManager& operator=(InputManager &&) = delete; 						//!< Delete move assignment operator

	enum Key {
		UP = SDL_SCANCODE_UP,
		DOWN = SDL_SCANCODE_DOWN,
		LEFT = SDL_SCANCODE_LEFT,
		RIGHT = SDL_SCANCODE_RIGHT,
		X = SDL_SCANCODE_X,
		Z = SDL_SCANCODE_Z
	};
};