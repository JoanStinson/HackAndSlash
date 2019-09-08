#include "Game.h"
#include "Math.h"
#include "Utils.h"
using namespace math;

Game::Game(const string &name, int screenWidth, int screenHeight, int screenScale) {
	Window::Instance(move(name), screenWidth, screenHeight, screenScale); // Initialises window singleton instance 

	string resPath = utils::getResourcePath();
	backgroundImage = RENDERER.LoadTexture(resPath + "map.png");
	splashImage = RENDERER.LoadTexture(resPath + "cyborgtitle.png");
	overlayImage = RENDERER.LoadTexture(resPath + "overlay.png");

	splashShowing = true;
	overlayTimer = 2;

	// Setup camera
	RENDERER.camera.x = 0;
	RENDERER.camera.y = 0;
	RENDERER.camera.w = screenWidth;
	RENDERER.camera.h = screenHeight;

	// Load up sounds
	SM.LoadSound("hit", resPath + "Randomize2.wav");
	SM.LoadSound("enemyHit", resPath + "Hit_Hurt9.wav");
	SM.LoadSound("swing", resPath + "Randomize21.wav");
	SM.LoadSound("dash", resPath + "dash.wav");
	SM.LoadSound("growl", resPath + "Randomize34.wav");
	SM.LoadSound("enemyDie", resPath + "Randomize41.wav");

	// New sounds for boss
	SM.LoadSound("crash", resPath + "crash.wav");
	SM.LoadSound("smash", resPath + "smash.wav");
	SM.LoadSound("shoot", resPath + "shoot2.wav");
	SM.LoadSound("laugh", resPath + "laugh2.wav");

	song = Mix_LoadMUS(string(resPath + "Fatal Theory.wav").c_str()); // Song by Ryan Beveridge https://soundcloud.com/ryan-beveridge
	if (song != nullptr)
		Mix_PlayMusic(song, -1);

	// Holds a list of group types. this list describes the types of groups of data our frames can have!
	list<DataGroupType> dataGroupTypes;

	// So what data can a frame have?
	// CollisionBoxes (although we have hardcoded the collision boxes)
	DataGroupType colBoxType;
	colBoxType.groupName = "collisionBox";
	colBoxType.dataType = DataGroupType::BOX;

	// Hitboxes
	DataGroupType hitBoxType;
	hitBoxType.groupName = "hitBox";
	hitBoxType.dataType = DataGroupType::BOX;

	// Damage
	DataGroupType dmgType;
	dmgType.groupName = "damage";
	dmgType.dataType = DataGroupType::NUMBER;

	// Add all of these dataTypes to the list
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

	// Build hero entity
	player = new Player(heroAnimSet);
	player->invincibleTimer = 0;
	player->x = screenWidth / 2;
	player->y = screenHeight / 2;
	// Tells keyboard input to manage hero
	IM.player = player;
	// Add hero to the entity list
	Entity::entities.push_back(player);

	// Get camera to follow hero
	camController.SetTarget(player);

	int tileSize = 32;
	// Build all the walls for this game
	// First. build walls on top and bottom of screen
	for (int i = 0; i < screenWidth / tileSize; i++) {
		// Fills in top row
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		// Re-using pointer to create bottom row
		newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = screenHeight - tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}
	// Now the sides
	for (int i = 1; i < screenHeight / tileSize - 1; i++) {
		// Fills in left column
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = tileSize / 2;
		newWall->y = i * tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		// Re-using pointer to create right column
		newWall = new Wall(wallAnimSet);
		newWall->x = screenWidth - tileSize / 2;
		newWall->y = i * tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

	buildBossNext = false;
	bossActive = false;

	// Setup hpBar's x and y to be centered
	hpBar.x = screenWidth / 2.0f - (hpBar.barWidth / 2.0f); // Centered horizontally
	hpBar.y = screenHeight - hpBar.barHeight - 20; // 20 pixels off the bottom of the screen
}

Game::~Game() {
	SDL_DestroyTexture(backgroundImage);
	SDL_DestroyTexture(splashImage);
	SDL_DestroyTexture(overlayImage);

	Mix_PausedMusic();
	Mix_FreeMusic(song);

	if (scoreTexture != nullptr)
		SDL_DestroyTexture(scoreTexture);

	Entity::DeleteAllEntities(&Entity::entities, false);

	delete heroAnimSet;
	delete globAnimSet;
	delete grobAnimSet;
	delete wallAnimSet;
	delete roundKingAnimSet;
	delete bulletAnimSet;

	delete player;

	// Delete all of the wall entities
	Entity::DeleteAllEntities(&walls, true);
	Entity::DeleteAllEntities(&enemies, true);
}

void Game::Run() {

	bool quit = false;

	SDL_Event e;
	// Setup my time controller before the game starts
	TM.Reset();

	/// GAME LOOP ///
	while (!quit) {
		TM.UpdateTime();

		Entity::RemoveInactiveEntities(&Entity::entities, false);
		// Remove/delete enemies in the enemy list who are dead/inactive
		Entity::RemoveInactiveEntities(&enemies, true);

		// Check for any events that might have happened
		while (SDL_PollEvent(&e)) {
			// Close the window
			if (e.type == SDL_QUIT)
				quit = true;
			// If keydown event
			if (e.type == SDL_KEYDOWN) {
				// Switch case on which button was pressed
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE: // Esc key
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					if (splashShowing)
						splashShowing = false;
					if (overlayTimer <= 0 && player->hp < 1) {
						// Cleanup and restart the game
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemyBuildTimer = 3;
						overlayTimer = 2;
						enemyWavesTillBoss = 3;
						bossActive = false;
						buildBossNext = false;
						// Make hpBar point to no entities health
						hpBar.entity = nullptr;

						Boss::roundKingsKilled = 0;
						Glob::globsKilled = 0;
						Grob::grobsKilled = 0;

						if (scoreTexture != nullptr) {
							SDL_DestroyTexture(scoreTexture);
							scoreTexture = nullptr;
						}

						// Remove all existing enemies
						for each(auto *entity in enemies) {
							entity->active = false;
						}
						player->Revive();
					}
					break;
				case SDL_SCANCODE_C:
					RENDERER.smoothCamera = !RENDERER.smoothCamera;
					break;
				}
			}
			IM.Update(&e);
		}

		/// Update all entities ///
		Update();

		/// Draw all entities ///
		Draw();
	}
}

void Game::Update() {
	// Make our overlay timer tick down
	if (player->hp < 1 && overlayTimer > 0) {
		overlayTimer -= TM.GetDt();
	}

	// Update all entities
	for each(auto *entity in Entity::entities) {
		entity->Update();
	}

	// Spawn enemies
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
		enemyBuildTimer -= TM.GetDt();
		// If no bosses on the prowl, check to see if we should build some jerks
		if (!buildBossNext && !bossActive && enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < MAX_ENEMIES) {
			Glob *enemy = new Glob(globAnimSet);
			// Set enemies position to somewhere random within the arena's open space
			enemy->x = randomNumber(WINDOW.SCREEN_WIDTH - (2 * 32) - 32) + 32 + 16;
			enemy->y = randomNumber(WINDOW.SCREEN_HEIGHT - (2 * 32) - 32) + 32 + 16;
			enemy->invincibleTimer = 0.1;

			// Pushback
			enemies.push_back(enemy);
			Entity::entities.push_back(enemy);

			if (enemiesBuilt % 5 == 0) {
				Grob *grob = new Grob(grobAnimSet);
				grob->x = randomNumber(WINDOW.SCREEN_WIDTH - (2 * 32) - 32) + 32 + 16; // Random x value between our walls
				grob->y = randomNumber(WINDOW.SCREEN_HEIGHT - (2 * 32) - 32) + 32 + 16; // Random x value between our walls
				grob->invincibleTimer = 0.01;

				// Pushback
				enemies.push_back(grob);
				Entity::entities.push_back(grob);
			}

			enemiesBuilt++;
			enemyBuildTimer = 1;
		}

		// Boss
		if (buildBossNext && enemyBuildTimer <= 0 && enemies.size() == 0) {
			Boss *round = new Boss(roundKingAnimSet, bulletAnimSet);
			round->invincibleTimer = 0.1;
			enemies.push_back(round);
			Entity::entities.push_back(round);

			// Make hpBar point to boss
			hpBar.entity = round;

			bossActive = true;
			buildBossNext = false;
			enemyWavesTillBoss = 3;
		}

		// If there was a boss, but hes dead now, then go back to spawning normal enemies till the next boss
		if (bossActive && enemies.size() == 0) {
			bossActive = false;
			buildBossNext = false;
			enemiesBuilt = 0;
			enemiesToBuild = 2;

			// When boss dead, make sure hpBar doesn't reference him anymore
			hpBar.entity = nullptr;
		}

	}

	// Update camera positions
	camController.Update();
}

void Game::Draw() {
	// Clear the screen
	SDL_SetRenderDrawColor(RENDERER(), 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(RENDERER());

	if (splashShowing) {
		RENDERER.RenderTexture(splashImage, 0, 0);
	}
	else {
		// Draw the background
		RENDERER.RenderTexture(backgroundImage, 0 - RENDERER.camera.x, 0 - RENDERER.camera.y);

		// Sort all entities based on y(depth)
		Entity::entities.sort(Entity::CompareEntity);
		// Draw all of the entities
		for each (auto *entity in Entity::entities) {
			entity->Draw();
		}

		// Draw UI stuff
		hpBar.Draw();

		if (overlayTimer <= 0 && player->hp < 1) {
			RENDERER.RenderTexture(overlayImage, 0, 0);
			if (scoreTexture == nullptr) {
				// Generate score text
				SDL_Color color = { 255, 255, 255, 255 }; // White

				stringstream ss;
				ss << "Enemies dispatched: " << Glob::globsKilled + Grob::grobsKilled + Boss::roundKingsKilled;

				string resPath = utils::getResourcePath();
				scoreTexture = RENDERER.RenderText(ss.str(), resPath + "vermin_vibes_1989.ttf", color, 30);
			}
			RENDERER.RenderTexture(scoreTexture, 20, 50);
		}
	}
	// After we're done drawing/rendering, show it to the screen
	SDL_RenderPresent(RENDERER());
}
