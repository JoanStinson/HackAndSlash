#pragma once
#include "TimeManager.h"
#include "AnimationSet.h"

#define PLAYER "player"
#define ENEMY "enemy"

/*! Abstract class, cannot instantiate an object of type Entity e.g can't do Entity e; */
class Entity {
public:
	virtual void Update() {}
	virtual void Draw();
	virtual void Move(float angle);

	/* Helper Methods */
	static float DistBetweenTwoEntities(Entity *e1, Entity *e2);
	static float AngleBetweenTwoEntities(Entity *e1, Entity *e2);
	static int AngleToDir(float angle);
	static bool CompareEntity(const Entity* const &a,					//!< Compare 2 entities in a list to help sorting (sorts based on y value)
		const Entity* const &b); 
	static void RemoveInactiveEntities(list<Entity*> *entityList, bool deleteEntities);
	static void DeleteAllEntities(list<Entity*> *entityList, bool deleteEntities);

	int state;
	float x;
	float y;
	bool moving;														/*!< Is the entity moving */
	bool active = true;													/*!< Entity turned on or off */
	string type;														/*!< What type of entity is it? e.g hero, enemy, wall etc */
	float angle;														/*!< Angle to move entity in (360 degree angle) */
	static list<Entity*> entities;

protected:
	/* Virtual Methods */
	virtual void UpdateMovement();
	virtual void UpdateCollisionBox();
	virtual void ChangeAnimation(int newState,							//!< Abstract function
		bool resetAnim = true) = 0; 
	virtual void UpdateCollisions();									//!< How we bump into stuff in the world
	virtual void CrashOntoSolid() {}

	enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

	int direction;
	bool solid = true;													/*!< Is this thing solid, can things pass through me */
	bool collideWithSolids = true;										/*!< Sometimes we are solid, but I pass through other solids */
	bool dieOnSolids = false;
	float moveSpeed;
	float moveSpeedMax;
	float slideAngle;													/*!< Direction pushed in */
	float slideAmount;													/*!< Amount of push in the slideAngle */
	float moveLerp = 4;
	float totalXMove, totalYMove;										/*!< Keeps track of total x,y movement per movement turn. just incase we need to retract movement */
	SDL_Rect collisionBox;												/*!< Box describing the size of our entity and this is used to bump into things */
	SDL_Rect lastCollisionBox;											/*!< Where our collisionbox was last */
	int collisionBoxW, collisionBoxH;									/*!< Our default collisionBox sizes */
	float collisionBoxYOffset;											/*!< From my entities y value, where should I sit this collisionBox */
	AnimationSet *animSet;												/*!< Set of all animations this entity can have */
	Animation *currentAnim;												/*!< Current animation the entity is using */
	Frame *currentFrame;												/*!< The current frame in the above animation the entity using */
	float frameTimer;													/*!< Helps animate frame to frame */
};