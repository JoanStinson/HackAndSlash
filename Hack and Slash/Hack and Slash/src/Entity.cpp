#include "Entity.h"
#include "TimeController.h"

void Entity::Update() {
}

void Entity::Draw() {
	// draws current frame
	if (currentFrame != nullptr && active) {
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	// draw collisionBox
	if (solid && globals::debugging) {
		// sets the current drawing color (doesn't affect textures and what not)
		SDL_SetRenderDrawColor(globals::renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(globals::renderer, &collisionBox);
	}
}

void Entity::Move(float angle) {
	moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	int newDirection = angleToDirection(angle);
	// if direction changed, update current animation
	if (direction != newDirection) {
		direction = newDirection;
		ChangeAnimation(state, false);
	}
}

void Entity::UpdateMovement() {
	UpdateCollisionBox();
	// store collisionBox before we move
	lastCollisionBox = collisionBox;

	// reset total moves
	totalXMove = 0;
	totalYMove = 0;

	//TODO movespeed and slideamount put in function
	if (moveSpeed > 0) {
		// works out move distance using speed, dt (time since last loop) and multiplies by lerp
		float moveDist = moveSpeed * (globals::timeController->dt)*moveLerp;
		if (moveDist > 0) {
			// xmove = distance * cos(angle in radians)
			float xMove = moveDist * (cos(angle*globals::PI / 180));
			// ymove = distance * sin(angle in radians)
			float yMove = moveDist * (sin(angle*globals::PI / 180));

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			if (!moving) {
				moveSpeed -= moveDist;
			}
		}
	}

	if (slideAmount > 0) {
		float slideDist = slideAmount * globals::timeController->dt*moveLerp;
		if (slideDist > 0) {
			float xMove = slideDist * (cos(slideAngle*globals::PI / 180));
			float yMove = slideDist * (sin(slideAngle*globals::PI / 180));

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

	// now that we've moved, move the collisionBox up to where we are now
	UpdateCollisionBox();
	// to help with collision checking, union collisionBox with lastCollisionBox
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void Entity::UpdateCollisionBox() {
	collisionBox.x = x - collisionBox.w / 2;
	collisionBox.y = y - collisionBox.h / 2;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
}

void Entity::UpdateCollisions() {
	if (active && collideWithSolids && (moveSpeed > 0 || slideAmount > 0)) { /* get rid of the movespeed and slideamount if you want to check collisions when not moving*/
		// list of potential collisions
		list<Entity*> collisions;

		for (auto *e : this->entities) {
			if (e->active && e->type != this->type && e->solid && checkCollision(collisionBox, e->collisionBox)) {
				// add it to the list
				collisions.push_back(e);
			}
		}
		// if we have a list of potential entities we may hit, then lets check them properly to do collision resolution
		if (collisions.size() > 0) {
			UpdateCollisionBox();

			// multisample check for collisions from where we started to where we are planning to go to

			// first we are going to find the sample distance we should travel between checks
			float boxTravelSize = 0;
			if (collisionBox.w < collisionBox.h) {
				boxTravelSize = collisionBox.w / 4;
			}
			else {
				boxTravelSize = collisionBox.h / 4;
			}

			// use sampleBox to check for collisions from start point to end point, moving at boxTravelSize each sample
			SDL_Rect sampleBox = lastCollisionBox;
			float movementAngle = angleBetweenTwoRects(lastCollisionBox, collisionBox);

			bool collisionFound = false;
			while (!collisionFound) {
				// check samplebox for collisions where it is now
				SDL_Rect intersection;
				for (auto *e : collisions) {
					if (SDL_IntersectRect(&sampleBox, &e->collisionBox, &intersection)) {

						collisionFound = true;
						moveSpeed = 0;
						moving = false;
						slideAngle = angleBetweenTwoEntities(e, this);

						// currently intersecting a entity, now we need to do collision resolution
						if (intersection.w < intersection.h) {
							if (lastCollisionBox.x + lastCollisionBox.w / 2 < e->collisionBox.x + e->collisionBox.w / 2) {
								sampleBox.x -= intersection.w; // started on left, so move left out of our collision
							}
							else {
								sampleBox.x += intersection.w; // otherwise, starated on right
							}
						}
						else {
							if (lastCollisionBox.y + lastCollisionBox.h / 2 < e->collisionBox.y + e->collisionBox.h / 2) {
								sampleBox.y -= intersection.h; // started above, so move up out of collision
							}
							else {
								sampleBox.y += intersection.h; // otherwise, started below
							}
						}
					}
				}

				// if collisions found or sampleBox is at sample place as collisionBox, exit loop
				if (collisionFound || (sampleBox.x == collisionBox.x && sampleBox.y == collisionBox.y)) {
					break;
				}

				// move sample box up to check the next spot
				if (distanceBetweenTwoRects(sampleBox, collisionBox) > boxTravelSize) {
					float xMove = boxTravelSize * (cos(movementAngle*globals::PI / 180));
					float yMove = boxTravelSize * (sin(movementAngle*globals::PI / 180));

					sampleBox.x += xMove;
					sampleBox.y += yMove;
				}
				else {
					sampleBox = collisionBox;
				}
			}

			if (collisionFound) {
				// move our entity to where the sampleBox ended up
				slideAmount = slideAmount / 2;
				x = sampleBox.x + sampleBox.w / 2;
				y = sampleBox.y - collisionBoxYOffset;
			}

			UpdateCollisionBox();
		}
	}
}

float Entity::distanceBetweenTwoRects(SDL_Rect & r1, SDL_Rect & r2) {
	SDL_Point e1, e2;
	e1.x = r1.x + r1.w / 2;
	e1.y = r1.y + r1.h / 2;

	e2.x = r2.x + r2.w / 2;
	e2.y = r2.y + r2.h / 2;

	return abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2)));;
}

float Entity::distanceBetweenTwoEntities(Entity * e1, Entity * e2) {
	return abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));;
}

float Entity::angleBetweenTwoEntities(Entity *e1, Entity *e2) {
	return angleBetweenTwoPoints(e1->x, e1->y, e2->x, e2->y);
}

bool Entity::checkCollision(SDL_Rect cbox1, SDL_Rect cbox2) {
	if (SDL_IntersectRect(&cbox1, &cbox2, nullptr)) {
		return true;
	}
	// if a rectangle is in another rectangle 
	// do it here
	return false;
}

int Entity::angleToDirection(float angle) {
	if ((angle >= 0.f && angle <= 45.f) || angle >= 315 && angle <= 360) {
		return DIR_RIGHT;
	}
	else if (angle >= 45 && angle <= 135) {
		return DIR_DOWN;
	}
	else if (angle >= 135 && angle <= 225) {
		return DIR_LEFT;
	}
	else {
		return DIR_UP;
	}
}

float Entity::angleBetweenTwoPoints(float cx1, float cy1, float cx2, float cy2) {
	float dx = cx2 - cx1;
	float dy = cy2 - cy1;

	return atan2(dy, dx) * 180 / globals::PI;
}

float Entity::angleBetweenTwoRects(SDL_Rect & r1, SDL_Rect & r2) {
	float cx1 = r1.x + (r1.w / 2);
	float cy1 = r1.y + (r1.h / 2);

	float cx2 = r2.x + (r2.w / 2);
	float cy2 = r2.y + (r2.h / 2);

	return angleBetweenTwoPoints(cx1, cy1, cx2, cy2);
}

list<Entity*> Entity::entities; // used to initialise a static member

bool Entity::EntityCompare(const Entity * const & e1, const Entity * const & e2) {
	if (e1 != 0 && e2 != 0) {
		return (e1->y < e1->y);
	}
	else {
		return false;
	}
}

void Entity::removeInactiveEntitiesFromList(list<Entity*>* entityList, bool deleteEntities) {
	for (auto entity = entityList->begin(); entity != entityList->end();) {
		if (!(*entity)->active) {
			if (deleteEntities) {
				delete (*entity);
			}
			entity = entityList->erase(entity);
		}
		else {
			entity++;
		}
	}
}

void Entity::removeAllFromList(list<Entity*>* entityList, bool deleteEntities) {
	for (auto entity = entityList->begin(); entity != entityList->end();) {
		if (deleteEntities) {
			delete (*entity);
		}
		entity = entityList->erase(entity);
	}
}

Entity::~Entity() {
}
