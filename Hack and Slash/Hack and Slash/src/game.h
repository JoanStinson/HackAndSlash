#pragma once
#include "Globals.h"
#include "Player.h"
#include "Wall.h"
#include "Glob.h"
#include "Grob.h"
#include "InputManager.h"
#include "DrawingFunctions.h"
#include "SoundManager.h"
#include "Camera.h"
#include "Boss.h"
#include "HpBar.h"

#define MAX_ENEMIES 10

class Game {
public:
	Game();
	~Game();

	void Update();
	void Draw();



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

	SDL_Texture* scoreTexture = nullptr; //for drawing strings to the screen

	Player *player;
	InputManager heroInput;

	list<Entity*> enemies;
	list<Entity*> walls;

	int enemyWavesTillBoss = 3;
	bool buildBossNext = false;
	bool bossActive = false;

	HpBar hpBar;

	bool splashShowing;
	float overlayTimer;

	Camera camController;

};