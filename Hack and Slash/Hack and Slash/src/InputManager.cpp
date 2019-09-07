#include "InputManager.h"

InputManager::InputManager(){
	//hardcoded keyboard buttons
	UP = SDL_SCANCODE_UP;
	DOWN = SDL_SCANCODE_DOWN;
	LEFT = SDL_SCANCODE_LEFT;
	RIGHT = SDL_SCANCODE_RIGHT;
	SLASH = SDL_SCANCODE_Z;
	DASH = SDL_SCANCODE_X;
	//BONUS task: load scancodes up from a textfile
	//int test = 82;
	//UP = (SDL_Scancode)test;
}

void InputManager::Update(SDL_Event* e){
	//button presses
	if (e->type == SDL_KEYDOWN){
		if (e->key.keysym.scancode == DASH){
			hero->Dash();
		}
		if (e->key.keysym.scancode == SLASH){
			hero->Slash();
		}
	}

	//button holds
	//check for keys still being held
	const Uint8 *keystates = SDL_GetKeyboardState(nullptr);
	//if hero not able to move or no direction buttons are being held down, then stop moving (slide to a halt)
	if ((hero->state != Player::MOVE && hero->state != Player::IDLE)
		|| (!keystates[UP] && !keystates[DOWN] && !keystates[LEFT] && !keystates[RIGHT]))
	{
		hero->moving = false;
	}
	else{
		//ups
		if (keystates[UP]){
			//upright
			if (keystates[RIGHT])
				hero->Move(270 + 45);
			else if (keystates[LEFT])
				hero->Move(270 - 45);
			else
				hero->Move(270);
		}
		//downs
		if (keystates[DOWN]){
			//downright
			if (keystates[RIGHT])
				hero->Move(90 - 45);
			else if (keystates[LEFT])
				hero->Move(90 + 45);
			else
				hero->Move(90);
		}
		//left
		if (!keystates[UP] && !keystates[DOWN] && !keystates[RIGHT] && keystates[LEFT])
			hero->Move(180);
		//right
		if (!keystates[UP] && !keystates[DOWN] && keystates[RIGHT] && !keystates[LEFT])
			hero->Move(0);
	}
}