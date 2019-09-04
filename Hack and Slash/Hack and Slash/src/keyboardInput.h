#ifndef KEYBOARDINPUT
#define KEYBOARDINPUT

#include "hero.h"

class KeyboardInput{
public:
	Hero* hero; //to which hero I look after
	SDL_Scancode UP, DOWN, LEFT, RIGHT;
	SDL_Scancode SLASH, DASH;

	KeyboardInput();
	void update(SDL_Event* e);
};

#endif