#include "game.h"

Game::Game(){
	string resPath = getResourcePath();
	backgroundImage = loadTexture(resPath + "map.png", Globals::renderer);

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

	wallAnimSet = new AnimationSet();
	wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

	//build hero entity
	hero = new Hero(heroAnimSet);
	hero->invincibleTimer = 0;
	hero->x = Globals::ScreenWidth / 2;
	hero->y = Globals::ScreenHeight / 2;
	//tells keyboard input to manage hero
	heroInput.hero = hero;
	//add hero to the entity list
	Entity::entities.push_back(hero);

	int tileSize = 32;
	//build all the walls for this game
	//first. build walls on top and bottom of screen
	for (int i = 0; i < Globals::ScreenWidth / tileSize; i++){
		//fills in top row
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize/2;
		newWall->y = tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//re-using pointer to create bottom row
		newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = Globals::ScreenHeight - tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}
	//now the sides
	for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++){
		//fills in left column
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		//re-using pointer to create right column
		newWall = new Wall(wallAnimSet);
		newWall->x = Globals::ScreenWidth - tileSize / 2;
		newWall->y = i*tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

}

Game::~Game(){
	cleanup(backgroundImage);

	Entity::removeAllFromList(&Entity::entities, false);
	//delete all of the wall entities
	Entity::removeAllFromList(&walls, true);
	Entity::removeAllFromList(&enemies, true);

	delete heroAnimSet;
	delete globAnimSet;
	delete wallAnimSet;

	delete hero;
}

void Game::update(){
	// enemy related
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1;

	bool quit = false;

	SDL_Event e;
	//setup my time controller before the game starts
	TimeController::timeController.reset();
	//game loop!
	while (!quit){
		TimeController::timeController.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);
		// remove/delete enemies in the enemy list who are dead/inactive
		Entity::removeInactiveEntitiesFromList(&enemies, true);

		//check for any events that might have happened
		while (SDL_PollEvent(&e)){
			//close the window
			if (e.type == SDL_QUIT)
				quit = true;
			//if keydown event
			if (e.type == SDL_KEYDOWN){
				//switch case on which button was pressed
				switch (e.key.keysym.scancode){
				case SDL_SCANCODE_ESCAPE: //esc key
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					hero->revive();
					break;

				}
			}
			heroInput.update(&e);
		}

		//update all entities
		for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
			//remember how awesome polymorphism is?
			//update all entities in game world at once
			(*entity)->update();
		}

		// generate enemies
		if (hero->hp > 0) {
			if (enemiesToBuild == enemiesBuilt) {
				enemiesToBuild *= 2;
				enemiesBuilt = 0;
				enemyBuildTimer = 4;
			}
			enemyBuildTimer -= TimeController::timeController.dT;
			if (enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && enemies.size() < 10) {
				Glob *enemy = new Glob(globAnimSet);
				// set enemies position to somewhere random within the arena's open space
				enemy->x = getRandomNumber(Globals::ScreenWidth - (2 * 32) - 32) + 32 + 16;
				enemy->y = getRandomNumber(Globals::ScreenHeight - (2 * 32) - 32) + 32 + 16;
				enemy->invincibleTimer = 0.1;

				enemies.push_back(enemy);
				Entity::entities.push_back(enemy);
			}
		}

		//draw all entities
		draw();
	}

}
void Game::draw(){
	//clear the screen
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);

	//draw the background
	renderTexture(backgroundImage, Globals::renderer, 0, 0);

	//sort all entities based on y(depth)
	Entity::entities.sort(Entity::EntityCompare);
	//draw all of the entities
	for (list<Entity*>::iterator entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++){
		(*entity)->draw();
	}

	//after we're done drawing/rendering, show it to the screen
	SDL_RenderPresent(Globals::renderer);
}