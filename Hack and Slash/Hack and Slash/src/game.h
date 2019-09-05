#ifndef GAME
#define GAME

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "glob.h"
#include "keyboardInput.h"
#include "drawing_functions.h"

class Game{
public:
	AnimationSet* heroAnimSet;
	AnimationSet* globAnimSet;
	AnimationSet* wallAnimSet;

	SDL_Texture* backgroundImage;
	SDL_Texture* splashImage;
	SDL_Texture* overlayImage;

	SDL_Texture *scoreTexture = NULL; // for drawing strings to the screen

	Hero *hero;
	KeyboardInput heroInput;

	list<Entity*> enemies;
	list<Entity*> walls;

	bool splashShowing;
	float overlayTimer;

	Game();
	~Game();

	void update();
	void draw();

};


#endif