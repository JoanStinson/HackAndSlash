#include "Entity.h"
#include "Math.h"
using namespace math;

list<Entity*> Entity::entities;

void Entity::Update() { ; }//override me to do something useful

void Entity::Draw() {
	//override me if you want something else or more specific to happen
	//draws current frame
	if (currentFrame != NULL && active) {
		currentFrame->Draw(animSet->spriteSheet, x - globals::camera.x, y - globals::camera.y);
	}
	//draw collsionBox
	if (solid && globals::debugging) {
		//sets the current drawing colour (Doesn't affect textures and what not)
		SDL_SetRenderDrawColor(globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(globals::renderer, &collisionBox);
	}
}

void Entity::Move(float angle) {
	moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	int newDirection = AngleToDir(angle);
	//if direction changed, update current animation
	if (direction != newDirection) {
		direction = newDirection;
		ChangeAnimation(state, false);
	}
}

void Entity::UpdateMovement() {
	UpdateCollisionBox();
	//store collisiobox before we move
	lastCollisionBox = collisionBox;

	//reset total moves
	totalXMove = 0;
	totalYMove = 0;

	if (moveSpeed > 0) {
		//works out move distance using speed, dt(time since last loop) and multiplies by lerp
		float moveDist = moveSpeed * (TimeManager::timeController.dT)*moveLerp;
		if (moveDist > 0) {
			//xmove = distance * cos(angle in radians)
			float xMove = moveDist * (cos(angle*M_PI / 180));
			//ymove = distance * sin(angle in radians)
			float yMove = moveDist * (sin(angle*M_PI / 180));

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			if (!moving)
				moveSpeed -= moveDist;
		}
	}
	//sliding around!
	if (slideAmount > 0) {
		float slideDist = slideAmount * TimeManager::timeController.dT*moveLerp;
		if (slideDist > 0) {
			float xMove = slideDist * (cos(slideAngle*M_PI / 180));
			float yMove = slideDist * (sin(slideAngle*M_PI / 180));

			x += xMove;
			y += yMove;

			totalXMove += xMove;
			totalYMove += yMove;
			slideAmount -= slideDist;
		}
		else {
			slideAmount = 0;
		}
	}
	//now that we've moved, move the collisionBox up to where we are now
	UpdateCollisionBox();
	//to help with collision checking, union collisionBox with lastCollisionBox
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void Entity::UpdateCollisionBox() {
	collisionBox.x = x - collisionBox.w / 2;
	collisionBox.y = y + collisionBoxYOffset;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
}

void Entity::UpdateCollisions() {
	if (active && collideWithSolids && (moveSpeed > 0 || slideAmount > 0)) {
		//list of potential collisions
		list<Entity*> collisions;

		//BROAD PHASE
		//list<Entity*>::iterator entity = ...
		for (auto entity = Entity::entities.begin(); entity != Entity::entities.end(); entity++) {
			//if we collide with this entity with our currently unioned collisionbox, add to the list
			if ((*entity)->active
				//&& (*entity)->type != this->type
				&& (*entity)->solid
				&& collBetweenTwoRects(collisionBox, (*entity)->collisionBox)) {

				//add it to the list
				collisions.push_back(*entity);

			}
		}
		//if we have a list of potential entities we may hit, then lets check them properly to do collision resolution
		if (collisions.size() > 0) {
			UpdateCollisionBox();

			float collisionTime = 1;//1 means no collisions, anything less, e.g 0.234, means we collided part of the of our movement
			float normalX, normalY;//to help work out which side we crash into stuff

			// our collisionbox before we tried to move
			SDL_Rect startingCollisionBox = lastCollisionBox;

			//loop through the entities that are in our short list from broadphase
			for (auto entity = collisions.begin(); entity != collisions.end(); entity++) {
				//temporary variables for normal x and y and also temp collisionTime
				float tmpNormalX, tmpNormalY;
				float tmpCollisionTime = sweptAABB(startingCollisionBox, totalXMove, totalYMove, (*entity)->collisionBox, tmpNormalX, tmpNormalY);

				//if this tmpcolltime is less than last colltime, use it instead
				if (tmpCollisionTime < collisionTime) {
					collisionTime = tmpCollisionTime;
					normalX = tmpNormalX;
					normalY = tmpNormalY;
				}
			}

			//if there was a collision, lets slide off of it
			if (collisionTime < 1.0f) {
				// if we die on solids, run the crash function
				if (dieOnSolids) {
					CrashOntoSolid();
				}

				//move our collisionBox position up to where we collided
				startingCollisionBox.x += totalXMove * collisionTime;
				startingCollisionBox.y += totalYMove * collisionTime;

				//how much move time was remaining
				float remainingTime = 1.0f - collisionTime;
				//update entities x and y to where we bumped into other entity
				x = startingCollisionBox.x + (startingCollisionBox.w / 2);
				y = startingCollisionBox.y - collisionBoxYOffset;
				//collision response:slide
				//work out dotproduct using remainintime
				float dotProd = (totalXMove * normalY + totalYMove * normalX)*remainingTime;
				totalXMove = dotProd * normalY;
				totalYMove = dotProd * normalX;
				x += totalXMove;
				y += totalYMove;
				//store collisionBox at this point
				UpdateCollisionBox();

				//BECAUSE OUR SLIDING MAY HAVE BUMPED INTO OTHER, BETTER RUN THE FUNCTION AGAIN, recursive wtfff broo :'''(()()()()()(
				UpdateCollisions();
			}
		}
	}
}

float Entity::DistBetweenTwoEntities(Entity *e1, Entity *e2) {
	return abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));
}

float Entity::AngleBetweenTwoEntities(Entity *e1, Entity *e2) {
	float dx, dy;
	float x1 = e1->x, y1 = e1->y, x2 = e2->x, y2 = e2->y;

	dx = x2 - x1;
	dy = y2 - y1;

	return atan2(dy, dx) * 180 / M_PI;
}

int Entity::AngleToDir(float angle) {
	if ((angle >= 0 && angle <= 45) || angle >= 315 && angle <= 360)
		return DIR_RIGHT;
	else if (angle >= 45 && angle <= 135)
		return DIR_DOWN;
	else if (angle >= 135 && angle <= 225)
		return DIR_LEFT;
	else
		return DIR_UP;
}

bool Entity::CompareEntity(const Entity* const &a, const Entity * const &b) {
	if (a != 0 && b != 0) {
		return (a->y < b->y);
	}
	else {
		return false;
	}
}

void Entity::RemoveInactiveEntities(list<Entity*> *entityList, bool deleteEntities) {
	for (auto entity = entityList->begin(); entity != entityList->end();) {
		//if entity is not active
		if (!(*entity)->active) {
			if (deleteEntities)
				delete (*entity);
			entity = entityList->erase(entity);
		}
		else {
			entity++;
		}
	}
}

void Entity::DeleteAllEntities(list<Entity*> *entityList, bool deleteEntities) {
	for (auto entity = entityList->begin(); entity != entityList->end();) {
		if (deleteEntities)
			delete (*entity);
		entity = entityList->erase(entity);
	}
}
