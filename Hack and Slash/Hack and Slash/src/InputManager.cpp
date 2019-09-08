#include "InputManager.h"

void InputManager::Update(SDL_Event* e) {

	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.scancode == X) {
			player->Dash();
		}
		if (e->key.keysym.scancode == Z) {
			player->Slash();
		}
	}

	//button holds
	const Uint8 *keystates = SDL_GetKeyboardState(nullptr);
	//if hero not able to move or no direction buttons are being held down, then stop moving (slide to a halt)
	if ((player->state != Player::MOVE && player->state != Player::IDLE) || (!keystates[UP] && !keystates[DOWN] && !keystates[LEFT] && !keystates[RIGHT])) {
		player->moving = false;
	}
	else {
		//ups
		if (keystates[UP]) {
			//upright
			if (keystates[RIGHT])
				player->Move(270 + 45);
			else if (keystates[LEFT])
				player->Move(270 - 45);
			else
				player->Move(270);
		}
		//downs
		if (keystates[DOWN]) {
			//downright
			if (keystates[RIGHT])
				player->Move(90 - 45);
			else if (keystates[LEFT])
				player->Move(90 + 45);
			else
				player->Move(90);
		}
		//left
		if (!keystates[UP] && !keystates[DOWN] && !keystates[RIGHT] && keystates[LEFT])
			player->Move(180);
		//right
		if (!keystates[UP] && !keystates[DOWN] && keystates[RIGHT] && !keystates[LEFT])
			player->Move(0);
	}
}
