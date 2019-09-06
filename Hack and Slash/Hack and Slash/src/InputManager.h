#pragma once
#include "Player.h"

class InputManager{
public:
	InputManager();
	void Update(SDL_Event* e);

	Player* hero; //to which hero I look after
	SDL_Scancode UP, DOWN, LEFT, RIGHT;
	SDL_Scancode SLASH, DASH;
};