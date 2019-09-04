#ifndef GAME
#define GAME

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "keyboardInput.h"
#include "drawing_functions.h"

class Game{
public:
	AnimationSet* heroAnimSet;
	AnimationSet* wallAnimSet;

	SDL_Texture* backgroundImage;

	Hero *hero;
	KeyboardInput heroInput;

	list<Entity*> walls;

	Game();
	~Game();

	void update();
	void draw();

};


#endif