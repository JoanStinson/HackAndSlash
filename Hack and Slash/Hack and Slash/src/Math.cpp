#include "Math.h"
#include <limits>
#include <algorithm>

namespace math {

	bool math::collBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2) {
		SDL_Rect intersection;
		return SDL_IntersectRect(&r1, &r2, &intersection) ? true : false;
	}

	float math::angleBetweenTwoPoints(float x1, float y1, float x2, float y2) {
		float dx = x2 - x1;
		float dy = y2 - y1;
		return atan2(dy, dx) * 180 / M_PI;
	}

	float math::angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2) {
		float x1 = r1.x + (r1.w / 2);
		float y1 = r1.y + (r1.h / 2);

		float x2 = r2.x + (r2.w / 2);
		float y2 = r2.y + (r2.h / 2);

		return angleBetweenTwoPoints(x1, y1, x2, y2);
	}

	float math::distBetweenTwoPoints(float x1, float y1, float x2, float y2) {
		return abs(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
	}

	float math::distBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2) {
		SDL_Point p1, p2;

		p1.x = r1.x + r1.w / 2;
		p1.y = r1.y + r1.h / 2;

		p2.x = r2.x + r2.w / 2;
		p2.y = r2.y + r2.h / 2;

		return abs(sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)));
	}

	/*! Return type: gives 0-1 depending on where collision is. 1 means, no collisions, 0 means collide immediately and 0.5 is half way etc.
	* params: movingBox is entity being checked
	* xv and vy are the velocities our moving box is moving at
	* otherbox is some other entities collision box we may collide with
	* normalx and normaly let us know which side of otherBox we collided with. these are pass by reference
	*/
	float math::sweptAABB(SDL_Rect &boxA, float vx, float vy, SDL_Rect &boxB, float &normalX, float &normalY) {

		float xInvEntry, xInvExit;
		float yInvEntry, yInvExit;

		// Find the distance between the objects on the near and far sides or both x and y
		if (vx > 0.0f) {
			xInvEntry = boxB.x - (boxA.x + boxA.w);
			xInvExit = (boxB.x + boxB.w) - boxA.x;
		}
		else {
			xInvEntry = (boxB.x + boxB.w) - boxA.x;
			xInvExit = boxB.x - (boxA.x + boxA.w);
		}

		if (vy > 0.0f) {
			yInvEntry = boxB.y - (boxA.y + boxA.h);
			yInvExit = (boxB.y + boxB.h) - boxA.y;
		}
		else {
			yInvEntry = (boxB.y + boxB.h) - boxA.y;
			yInvExit = boxB.y - (boxA.y + boxA.h);
		}

		// Find time of collision and time of leacing for each axis (if statement is to prevent dividing by zero)
		float xEntry, xExit;
		float yEntry, yExit;

		if (vx == 0.0f) {
			xEntry = -std::numeric_limits<float>::infinity();
			xExit = std::numeric_limits<float>::infinity();
		}
		else {
			xEntry = xInvEntry / vx;
			xExit = xInvExit / vx;
		}

		if (vy == 0.0f) {
			yEntry = -std::numeric_limits<float>::infinity();
			yExit = std::numeric_limits<float>::infinity();
		}
		else {
			yEntry = yInvEntry / vy;
			yExit = yInvExit / vy;
		}

		// Find the earliest/latest times of collision
		float entryTime = std::max(xEntry, yEntry);
		float exitTime = std::min(xExit, yExit);

		// If there was NO collision
		if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f) {
			normalX = 0.0f;
			normalY = 0.0f;
			return 1.0f;
		}
		else {
			// There was a collision 
			// Work out which sides/normals of the otherbox we collided with
			if (xEntry > yEntry) {
				// Assume we hit otherbox on the x axis
				if (xInvEntry < 0.0f) {
					normalX = 1; // Hit right hand side
					normalY = 0; // Not hit top or bottom
				}
				else {
					normalX = -1; // Hit left hand side
					normalY = 0; // Not hit top or bottom of box
				}
			}
			else {
				// Assume we hit otherbox on y axis
				if (yEntry < 0.0f) {
					normalX = 0;
					normalY = 1;
				}
				else {
					normalX = 0;
					normalY = -1;
				}
			}
			// Return the time of collision
			return entryTime;
		}
	}
}