#include "Game.h"
#include "Math.h"
#include "Globals.h"
using namespace math;

Game::Game() {
	string resPath = globals::getResourcePath();
	backgroundImage = LoadTexture(resPath + "map.png", globals::renderer);
	splashImage = LoadTexture(resPath + "cyborgtitle.png", globals::renderer);
	overlayImage = LoadTexture(resPath + "overlay.png", globals::renderer);

	splashShowing = true;
	overlayTimer = 2;

	// setup camera
	globals::camera.x = 0;
	globals::camera.y = 0;
	globals::camera.w = globals::ScreenWidth;
	globals::camera.h = globals::ScreenHeight;

	//load up sounds
	SoundManager::soundManager.LoadSound("hit", resPath + "Randomize2.wav");
	SoundManager::soundManager.LoadSound("enemyHit", resPath + "Hit_Hurt9.wav");
	SoundManager::soundManager.LoadSound("swing", resPath + "Randomize21.wav");
	SoundManager::soundManager.LoadSound("dash", resPath + "dash.wav");
	SoundManager::soundManager.LoadSound("growl", resPath + "Randomize34.wav");
	SoundManager::soundManager.LoadSound("enemyDie", resPath + "Randomize41.wav");
	//new sounds for boss
	SoundManager::soundManager.LoadSound("crash", resPath + "crash.wav");
	SoundManager::soundManager.LoadSound("smash", resPath + "smash.wav");
	SoundManager::soundManager.LoadSound("shoot", resPath + "shoot2.wav");
	SoundManager::soundManager.LoadSound("laugh", resPath + "laugh2.wav");

	song = Mix_LoadMUS(string(resPath + "Fatal Theory.wav").c_str());//Song by Ryan Beveridge https://soundcloud.com/ryan-beveridge
	if (song != NULL)
		Mix_PlayMusic(song, -1);

	//holds a list of group types. this list describes the types of groups of data our frames can have!
	list<DataGroupType> dataGroupTypes;

	//so what data can a frame have?
	//collisionBoxes (although we have hardcoded the collision boxes)
	DataGroupType colBoxType;
	colBoxType.groupName = "collisionBox";
	colBoxType.dataType = DataGroupType::DATATYPE_BOX;

	//hitboxes
	DataGroupType hitBoxType;
	hitBoxType.groupName = "hitBox";
	hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

	//damage
	DataGroupType dmgType;
	dmgType.groupName = "damage";
	dmgType.dataType = DataGroupType::DATATYPE_NUMBER;

	//add all of these dataTypes to the list
	dataGroupTypes.push_back(colBoxType);
	dataGroupTypes.push_back(hitBoxType);
	dataGroupTypes.push_back(dmgType);

	heroAnimSet = new AnimationSet();
	heroAnimSet->LoadAnimationSet("gameData.fdset", dataGroupTypes, true, 0, true);

	globAnimSet = new AnimationSet();
	globAnimSet->LoadAnimationSet("glob.fdset", dataGroupTypes, true, 0, true);

	grobAnimSet = new AnimationSet();
	grobAnimSet->LoadAnimationSet("grob.fdset", dataGroupTypes, true, 0, true);

	roundKingAnimSet = new AnimationSet();
	roundKingAnimSet->LoadAnimationSet("roundKing.fdset", dataGroupTypes, true, 0, true);

	bulletAnimSet = new AnimationSet();
	bulletAnimSet->LoadAnimationSet("bullet.fdset", dataGroupTypes, true, 0, true);

	wallAnimSet = new AnimationSet();
	wallAnimSet->LoadAnimationSet("wall.fdset", dataGroupTypes);

	//build hero entity
	player = new Player(heroAnimSet);
	player->invincibleTimer = 0;
	player->x = globals::ScreenWidth / 2;
	player->y = globals::ScreenHeight / 2;
	//tells keyboard input to manage hero
	heroInput.hero = player;
	//add hero to the entity list
	Entity::entities.push_back(player);

	//Get camera to follow hero
	camController.target = player;

	int tileSize = 32;
	//build all the walls for this game
	//first. build walls on top and bottom of screen
	for (int i = 0; i < globals::ScreenWidth / tileSize; i++) {
		//fills in top row
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//re-using pointer to create bottom row
		newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = globals::ScreenHeight - tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}
	//now the sides
	for (int i = 1; i < globals::ScreenHeight / tileSize - 1; i++) {
		//fills in left column
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = tileSize / 2;
		newWall->y = i * tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//re-using pointer to create right column
		newWall = new Wall(wallAnimSet);
		newWall->x = globals::ScreenWidth - tileSize / 2;
		newWall->y = i * tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

	buildBossNext = false;
	bossActive = false;

	//setup hpBar's x and y to be centered
	hpBar.x = globals::ScreenWidth / 2.0f - (hpBar.barWidth / 2.0f); //centered horizontally
	hpBar.y = globals::ScreenHeight - hpBar.barHeight - 20;//20 pixels off the bottom of the screen
}

Game::~Game() {
	SDL_DestroyTexture(backgroundImage);
	SDL_DestroyTexture(splashImage);
	SDL_DestroyTexture(overlayImage);

	Mix_PausedMusic();
	Mix_FreeMusic(song);

	if (scoreTexture != NULL) 
		SDL_DestroyTexture(scoreTexture);

	Entity::DeleteAllEntities(&Entity::entities, false);

	delete heroAnimSet;
	delete globAnimSet;
	delete grobAnimSet;
	delete wallAnimSet;
	delete roundKingAnimSet;
	delete bulletAnimSet;

	delete player;

	//delete all of the wall entities
	Entity::DeleteAllEntities(&walls, true);
	Entity::DeleteAllEntities(&enemies, true);
}

void Game::Update() {
	//enemy related
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1;

	bool quit = false;

	SDL_Event e;
	//setup my time controller before the game starts
	TimeManager::timeController.Reset();
	//game loop!
	while (!quit) {
		TimeManager::timeController.UpdateTime();

		Entity::RemoveInactiveEntities(&Entity::entities, false);
		//remove/delete enemies in the enemy list who are dead/inactive
		Entity::RemoveInactiveEntities(&enemies, true);

		//check for any events that might have happened
		while (SDL_PollEvent(&e)) {
			//close the window
			if (e.type == SDL_QUIT)
				quit = true;
			//if keydown event
			if (e.type == SDL_KEYDOWN) {
				//switch case on which button was pressed
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE: //esc key
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					if (splashShowing)
						splashShowing = false;
					if (overlayTimer <= 0 && player->hp < 1) {
						//cleanup and restart the game
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemyBuildTimer = 3;
						overlayTimer = 2;
						enemyWavesTillBoss = 3;
						bossActive = false;
						buildBossNext = false;
						//make hpBar point to no entities health
						hpBar.entity = NULL;

						Boss::roundKingsKilled = 0;
						Glob::globsKilled = 0;
						Grob::grobsKilled = 0;
						if (scoreTexture != NULL) {
							SDL_DestroyTexture(scoreTexture);
							scoreTexture = NULL;
						}

						//remove all existing enemies
						for (list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++) {
							(*enemy)->active = false;
						}
						player->Revive();
					}
					break;
				case SDL_SCANCODE_C:
					globals::smoothCamera = !globals::smoothCamera;
					break;

				}
			}
			heroInput.Update(&e);
		}
		//make our overlay timer tick down
		if (player->hp < 1 && overlayTimer > 0) {
			overlayTimer -= TimeManager::timeController.dT;
		}

		//update all entities
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			//remember how awesome polymorphism is?
			//update all entities in game world at once
			(*entity)->Update();
		}

		//SPAWN ENEMIES
		if (player->hp > 0 && !splashShowing) {
			if (enemiesToBuild == enemiesBuilt && enemies.size() <= 0) {
				enemiesToBuild = enemiesToBuild + 2;
				enemiesBuilt = 0;
				enemyBuildTimer = 4;
				enemyWavesTillBoss--;

				if (enemyWavesTillBoss <= 0) {
					buildBossNext = true;
				}
			}
			enemyBuildTimer -= TimeManager::timeController.dT;
			//if no bosses on the prowl, check to see if we should build some jerks
			if (!buildBossNext && !bossActive && enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10) {//TODO 10 - MAX_ENEMIES
				Glob *enemy = new Glob(globAnimSet);
				//set enemies position to somewhere random within the arena's open space
				enemy->x = randomNumber(globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
				enemy->y = randomNumber(globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
				enemy->invincibleTimer = 0.1;

				//PUSHBACK
				enemies.push_back(enemy);
				Entity::entities.push_back(enemy);

				if (enemiesBuilt % 5 == 0) {
					Grob *grob = new Grob(grobAnimSet);
					grob->x = randomNumber(globals::ScreenWidth - (2 * 32) - 32) + 32 + 16; //random x value between our walls
					grob->y = randomNumber(globals::ScreenHeight - (2 * 32) - 32) + 32 + 16; //random x value between our walls
					grob->invincibleTimer = 0.01;

					// PUSHBACK
					enemies.push_back(grob);
					Entity::entities.push_back(grob);
				}

				enemiesBuilt++;
				enemyBuildTimer = 1;
			}

			//FOR THE BOSS
			if (buildBossNext && enemyBuildTimer <= 0 && enemies.size() == 0) {
				Boss *round = new Boss(roundKingAnimSet, bulletAnimSet);
				round->invincibleTimer = 0.1;
				enemies.push_back(round);
				Entity::entities.push_back(round);

				//make hpBar point to Boss
				hpBar.entity = round;


				bossActive = true;
				buildBossNext = false;
				enemyWavesTillBoss = 3;
			}

			//if there was a boss, but hes dead now, then go back to spawning normal enemies till the next boss
			if (bossActive && enemies.size() == 0) {
				bossActive = false;
				buildBossNext = false;
				enemiesBuilt = 0;
				enemiesToBuild = 2;

				//when boss dead, make sure hpBar doesn't reference him anymore
				hpBar.entity = NULL;
			}

		}

		//update camera positions
		camController.Update();

		//draw all entities
		Draw();
	}

}

void Game::Draw() {
	//clear the screen
	SDL_SetRenderDrawColor(globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(globals::renderer);

	if (splashShowing) {
		RenderTexture(splashImage, globals::renderer, 0, 0);
	}
	else {
		//draw the background
		RenderTexture(backgroundImage, globals::renderer, 0 - globals::camera.x, 0 - globals::camera.y);

		//sort all entities based on y(depth)
		Entity::entities.sort(Entity::CompareEntity);
		//draw all of the entities
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			(*entity)->Draw();
		}

		//draw UI stuff
		hpBar.Draw();

		if (overlayTimer <= 0 && player->hp < 1) {
			RenderTexture(overlayImage, globals::renderer, 0, 0);
			if (scoreTexture == NULL) {
				//generate score text
				SDL_Color color = { 255, 255, 255, 255 };//white

				stringstream ss;
				ss << "Enemies dispatched: " << Glob::globsKilled + Grob::grobsKilled + Boss::roundKingsKilled;

				string resPath = globals::getResourcePath();
				scoreTexture = RenderText(ss.str(), resPath + "vermin_vibes_1989.ttf", color, 30, globals::renderer);
			}
			RenderTexture(scoreTexture, globals::renderer, 20, 50);
		}
	}
	//after we're done drawing/rendering, show it to the screen
	SDL_RenderPresent(globals::renderer);
}