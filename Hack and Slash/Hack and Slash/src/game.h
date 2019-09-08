#pragma once
#include "Player.h"
#include "Wall.h"
#include "Glob.h"
#include "Grob.h"
#include "InputManager.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "Camera.h"
#include "Boss.h"
#include "HpBar.h"
#include "Window.h"

#define MAX_ENEMIES 10

class Game {
public:
	Game(const string &name, int screenWidth, int screenHeight, int screenScale);
	~Game();
	void Run();

private:
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
	SDL_Texture* scoreTexture = nullptr; 
	Player *player;
	list<Entity*> enemies;
	list<Entity*> walls;
	int enemyWavesTillBoss = 3;
	bool buildBossNext = false;
	bool bossActive = false;
	HpBar hpBar;
	bool splashShowing;
	float overlayTimer;
	Camera camController;
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1;
};