#include "game.h"
#include "Math.h"
#include "globals.h"
using namespace math;

Game::Game() {
	string resPath = globals::getResourcePath();
	backgroundImage = loadTexture(resPath + "map.png", globals::renderer);
	splashImage = loadTexture(resPath + "cyborgtitle.png", globals::renderer);
	overlayImage = loadTexture(resPath + "overlay.png", globals::renderer);

	splashShowing = true;
	overlayTimer = 2;

	// setup camera
	globals::camera.x = 0;
	globals::camera.y = 0;
	globals::camera.w = globals::ScreenWidth;
	globals::camera.h = globals::ScreenHeight;

	//load up sounds
	SoundManager::soundManager.loadSound("hit", resPath + "Randomize2.wav");
	SoundManager::soundManager.loadSound("enemyHit", resPath + "Hit_Hurt9.wav");
	SoundManager::soundManager.loadSound("swing", resPath + "Randomize21.wav");
	SoundManager::soundManager.loadSound("dash", resPath + "dash.wav");
	SoundManager::soundManager.loadSound("growl", resPath + "Randomize34.wav");
	SoundManager::soundManager.loadSound("enemyDie", resPath + "Randomize41.wav");
	//new sounds for boss
	SoundManager::soundManager.loadSound("crash", resPath + "crash.wav");
	SoundManager::soundManager.loadSound("smash", resPath + "smash.wav");
	SoundManager::soundManager.loadSound("shoot", resPath + "shoot2.wav");
	SoundManager::soundManager.loadSound("laugh", resPath + "laugh2.wav");

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
	heroAnimSet->loadAnimationSet("gameData.fdset", dataGroupTypes, true, 0, true);

	globAnimSet = new AnimationSet();
	globAnimSet->loadAnimationSet("glob.fdset", dataGroupTypes, true, 0, true);

	grobAnimSet = new AnimationSet();
	grobAnimSet->loadAnimationSet("grob.fdset", dataGroupTypes, true, 0, true);

	roundKingAnimSet = new AnimationSet();
	roundKingAnimSet->loadAnimationSet("roundKing.fdset", dataGroupTypes, true, 0, true);

	bulletAnimSet = new AnimationSet();
	bulletAnimSet->loadAnimationSet("bullet.fdset", dataGroupTypes, true, 0, true);

	wallAnimSet = new AnimationSet();
	wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

	//build hero entity
	hero = new Hero(heroAnimSet);
	hero->invincibleTimer = 0;
	hero->x = globals::ScreenWidth / 2;
	hero->y = globals::ScreenHeight / 2;
	//tells keyboard input to manage hero
	heroInput.hero = hero;
	//add hero to the entity list
	Entity::entities.push_back(hero);

	//Get camera to follow hero
	camController.target = hero;

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

	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimSet;
	delete globAnimSet;
	delete grobAnimSet;
	delete wallAnimSet;
	delete roundKingAnimSet;
	delete bulletAnimSet;

	delete hero;

	//delete all of the wall entities
	Entity::removeAllFromList(&walls, true);
	Entity::removeAllFromList(&enemies, true);
}

void Game::update() {
	//enemy related
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1;

	bool quit = false;

	SDL_Event e;
	//setup my time controller before the game starts
	TimeController::timeController.reset();
	//game loop!
	while (!quit) {
		TimeController::timeController.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);
		//remove/delete enemies in the enemy list who are dead/inactive
		Entity::removeInactiveEntitiesFromList(&enemies, true);

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
					if (overlayTimer <= 0 && hero->hp < 1) {
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

						RoundKing::roundKingsKilled = 0;
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
						hero->revive();
					}
					break;
				case SDL_SCANCODE_C:
					globals::smoothCamera = !globals::smoothCamera;
					break;

				}
			}
			heroInput.update(&e);
		}
		//make our overlay timer tick down
		if (hero->hp < 1 && overlayTimer > 0) {
			overlayTimer -= TimeController::timeController.dT;
		}

		//update all entities
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			//remember how awesome polymorphism is?
			//update all entities in game world at once
			(*entity)->update();
		}

		//SPAWN ENEMIES
		if (hero->hp > 0 && !splashShowing) {
			if (enemiesToBuild == enemiesBuilt && enemies.size() <= 0) {
				enemiesToBuild = enemiesToBuild + 2;
				enemiesBuilt = 0;
				enemyBuildTimer = 4;
				enemyWavesTillBoss--;

				if (enemyWavesTillBoss <= 0) {
					buildBossNext = true;
				}
			}
			enemyBuildTimer -= TimeController::timeController.dT;
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
				RoundKing *round = new RoundKing(roundKingAnimSet, bulletAnimSet);
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
		camController.update();

		//draw all entities
		draw();
	}

}
void Game::draw() {
	//clear the screen
	SDL_SetRenderDrawColor(globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(globals::renderer);

	if (splashShowing) {
		renderTexture(splashImage, globals::renderer, 0, 0);
	}
	else {
		//draw the background
		renderTexture(backgroundImage, globals::renderer, 0 - globals::camera.x, 0 - globals::camera.y);

		//sort all entities based on y(depth)
		Entity::entities.sort(Entity::EntityCompare);
		//draw all of the entities
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			(*entity)->draw();
		}

		//draw UI stuff
		hpBar.draw();

		if (overlayTimer <= 0 && hero->hp < 1) {
			renderTexture(overlayImage, globals::renderer, 0, 0);
			if (scoreTexture == NULL) {
				//generate score text
				SDL_Color color = { 255, 255, 255, 255 };//white

				stringstream ss;
				ss << "Enemies dispatched: " << Glob::globsKilled + Grob::grobsKilled + RoundKing::roundKingsKilled;

				string resPath = globals::getResourcePath();
				scoreTexture = renderText(ss.str(), resPath + "vermin_vibes_1989.ttf", color, 30, globals::renderer);
			}
			renderTexture(scoreTexture, globals::renderer, 20, 50);
		}
	}
	//after we're done drawing/rendering, show it to the screen
	SDL_RenderPresent(globals::renderer);
}