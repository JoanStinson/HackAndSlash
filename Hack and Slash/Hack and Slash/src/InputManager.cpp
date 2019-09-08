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

	// Button holds
	const Uint8 *keystates = SDL_GetKeyboardState(nullptr);
	// If hero not able to move or no direction buttons are being held down, then stop moving (slide to a halt)
	if ((player->state != Player::MOVE && player->state != Player::IDLE) || (!keystates[UP] && !keystates[DOWN] && !keystates[LEFT] && !keystates[RIGHT])) {
		player->moving = false;
	}
	else {
		// Ups
		if (keystates[UP]) {
			// Upright
			if (keystates[RIGHT])
				player->Move(270 + 45);
			else if (keystates[LEFT])
				player->Move(270 - 45);
			else
				player->Move(270);
		}
		// Downs
		if (keystates[DOWN]) {
			// Downright
			if (keystates[RIGHT])
				player->Move(90 - 45);
			else if (keystates[LEFT])
				player->Move(90 + 45);
			else
				player->Move(90);
		}
		// Left
		if (!keystates[UP] && !keystates[DOWN] && !keystates[RIGHT] && keystates[LEFT])
			player->Move(180);
		// Right
		if (!keystates[UP] && !keystates[DOWN] && keystates[RIGHT] && !keystates[LEFT])
			player->Move(0);
	}
}
