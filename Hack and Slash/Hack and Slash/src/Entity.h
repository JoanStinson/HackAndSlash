#pragma once
#include "Globals.h"
#include "TimeManager.h"
#include "AnimationSet.h"

#define PLAYER "player"
#define ENEMY "enemy"

//Abstract Class. cannot instantiate an object of type Entity e.g cannot do Entity e;
class Entity {
public:
	//reference constants
	enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

	//quick label to see what the entity is up to
	int state;
	float x, y;
	int direction;
	bool solid = true; //is this thing solid, can things pass through me
	bool collideWithSolids = true; //sometimes we are solid, but I pass through other solids
	bool dieOnSolids = false;
	bool active = true; //entity turned on or off
	string type; //what type of entity is it? e.g hero, enemy, wall etc
	bool moving; //is the entity moving
	float angle; //angle to move entity in (360 degree angle)
	float moveSpeed;
	float moveSpeedMax;
	float slideAngle; //direction pushed in
	float slideAmount; //amount of push in the slideAngle
	float moveLerp = 4;
	float totalXMove, totalYMove; //keeps track of total x,y movement per movement turn. just incase we need to retract movement

	SDL_Rect collisionBox; //box describing the size of our entity and this is used to bump into things
	SDL_Rect lastCollisionBox; //where our collisionbox was last
	int collisionBoxW, collisionBoxH; //our default collisionBox sizes
	float collisionBoxYOffset; //from my entities y value, where should I sit this collisionBox

	AnimationSet *animSet; //set of all animations this entity can have
	Animation *currentAnim; //current animation the entity is using
	Frame *currentFrame; //the current frame in the above animation the entity using
	float frameTimer; //helps animate frame to frame
	static list<Entity*> entities;

	//VIRTUAL FUNCTIONS
	virtual void Update();
	virtual void Draw();
	virtual void Move(float angle);
	virtual void UpdateMovement();
	virtual void UpdateCollisionBox();
	virtual void ChangeAnimation(int newState, bool resetFrameToBeginning = true) = 0;//abstract function
	virtual void UpdateCollisions(); //how we bump into stuff in the world
	virtual void CrashOntoSolid() { ; }

	//HELP FUNCTIONS
	static float DistBetweenTwoEntities(Entity *e1, Entity *e2);
	static float AngleBetweenTwoEntities(Entity *e1, Entity *e2);
	static int AngleToDir(float angle);
	static bool CompareEntity(const Entity* const &a, const Entity * const &b); //compare 2 entities in a list to help sorting (sorts based on y value)
	static void RemoveInactiveEntities(list<Entity*> *entityList, bool deleteEntities);
	static void DeleteAllEntities(list<Entity*> *entityList, bool deleteEntities);
};