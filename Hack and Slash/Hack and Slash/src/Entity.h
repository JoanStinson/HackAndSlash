#pragma once
#include "Globals.h"
#include "AnimationSet.h"
#include <list>
// Abstract Class (it has at least 1 pure virtual function): cannot instantiate an object of type Entity e.g cannot do "Entity e;"
// "m_" for member variables? https://stackoverflow.com/questions/13018189/what-does-m-variable-prefix-mean
//LNK2001 error -> to solve it define what it complains about static if multiple redefinition
class Entity {
public:
	// Reference constants for directions
	//TODO change for enum
	static const int DIR_UP = 0;
	static const int DIR_DOWN = 1;
	static const int DIR_LEFT = 2;
	static const int DIR_RIGHT = 3;
	static const int DIR_NONE = -1;

	// quick label to see what the entity is up to
	int state;

	float x, y;
	int direction;
	bool solid = true; // is this thing solid, can things pass through me
	bool collideWithSolids = true; // sometimes we are solid, but I pass through other solids
	bool active = true; // entity turned on or off
	string type = "entity"; // what type of entity is it? e.g hero, enemy, wall etc.
	bool moving; // is the entity moving
	float angle; // angle to move entity in (360 degree angle)
	float moveSpeed;
	float moveSpeedMax;
	float slideAngle; // direction pushed in
	float slideAmount; // amount of push in the m_slideAngle
	float moveLerp = 4.f;
	float totalXMove, totalYMove; // keeps track of total x,y movement per movement turn. just incase we need to retract movement

	SDL_Rect collisionBox; // box describing the size of our entity and this is used to bump into things
	SDL_Rect lastCollisionBox; // where our collisionBox was last
	int collisionBoxW, collisionBoxH; // our default collisionBox sizes
	float collisionBoxYOffset; // from my entities y value, where should I sit this collisionBox

	AnimationSet *animSet; // set of all animations this entity can have
	Animation *currentAnim; // current animation the entity is using
	Frame *currentFrame; // the current frame in the above animation the entity is using
	float frameTimer; // helps animate frame to frame

	// VIRTUAL PURE FUNCTIONS
	virtual void Update();
	virtual void Draw();

	virtual void Move(float angle);
	virtual void UpdateMovement();
	virtual void UpdateCollisionBox();

	virtual void ChangeAnimation(int newState, bool resetFrameToBeginning) = 0; // abstract function
	virtual void UpdateCollisions(); // how we bump into stuff in the world

	// HELP FUNCTIONS
	//TODO put them somewhere else
	//Math.h or something
	static float distanceBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
	static float distanceBetweenTwoEntities(Entity *e1, Entity *e2);
	static float angleBetweenTwoEntities(Entity *e1, Entity *e2);
	static bool checkCollision(SDL_Rect cbox1, SDL_Rect cbox2);
	static int angleToDirection(float angle);
	static float angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);

	// global entities list I can refer to at anytime
	static list<Entity*> entities; //static before
	static bool EntityCompare(const Entity* const &e1, const Entity* const &e2); // compare 2 entities in a list to help sorting (sorts based on y value)
	
	// pass list by pointer if you want to change elements of the list (modify or delete), if not (only to read), pass by reference
	static void removeInactiveEntitiesFromList(list<Entity*> *entityList, bool deleteEntities);
	static void removeAllFromList(list<Entity*> *entityList, bool deleteEntities);

	virtual ~Entity();
};