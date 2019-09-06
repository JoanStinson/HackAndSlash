#ifndef GAME
#define GAME

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "glob.h"
#include "grob.h"
#include "keyboardInput.h"
#include "drawing_functions.h"
#include "soundManager.h"
#include "CameraController.h"
#include "RoundKing.h"
#include "hpBar.h"

class Game {
public:
	Mix_Music* song;

	AnimationSet* heroAnimSet;
	AnimationSet* globAnimSet;
	AnimationSet* grobAnimSet;
	AnimationSet* wallAnimSet;
	AnimationSet* roundKingAnimSet;
	AnimationSet* bulletAnimSet;

	SDL_Texture* backgroundImage;
	SDL_Texture* splashImage;
	SDL_Texture* overlayImage;

	SDL_Texture* scoreTexture = NULL; //for drawing strings to the screen

	Hero *hero;
	KeyboardInput heroInput;

	list<Entity*> enemies;
	list<Entity*> walls;

	int enemyWavesTillBoss = 1;
	bool buildBossNext = false;
	bool bossActive = false;

	HPBar hpBar;

	bool splashShowing;
	float overlayTimer;

	CameraController camController;

	Game();
	~Game();

	void update();
	void draw();

};


#endif