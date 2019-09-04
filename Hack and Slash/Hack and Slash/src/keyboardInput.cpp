#include "keyboardInput.h"

KeyboardInput::KeyboardInput(){
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
void KeyboardInput::update(SDL_Event* e){
	//button presses
	if (e->type == SDL_KEYDOWN){
		if (e->key.keysym.scancode == DASH){
			hero->dash();
		}
		if (e->key.keysym.scancode == SLASH){
			hero->slash();
		}
	}

	//button holds
	//check for keys still being held
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
	//if hero not able to move or no direction buttons are being held down, then stop moving (slide to a halt)
	if ((hero->state != Hero::HERO_STATE_MOVE && hero->state != Hero::HERO_STATE_IDLE) 
		|| (!keystates[UP] && !keystates[DOWN] && !keystates[LEFT] && !keystates[RIGHT]))
	{
		hero->moving = false;
	}
	else{
		//ups
		if (keystates[UP]){
			//upright
			if (keystates[RIGHT])
				hero->move(270 + 45);
			else if (keystates[LEFT])
				hero->move(270 - 45);
			else
				hero->move(270);
		}
		//downs
		if (keystates[DOWN]){
			//downright
			if (keystates[RIGHT])
				hero->move(90 - 45);
			else if (keystates[LEFT])
				hero->move(90 + 45);
			else
				hero->move(90);
		}
		//left
		if (!keystates[UP] && !keystates[DOWN] && !keystates[RIGHT] && keystates[LEFT])
			hero->move(180);
		//right
		if (!keystates[UP] && !keystates[DOWN] && keystates[RIGHT] && !keystates[LEFT])
			hero->move(0);
	}
}