#pragma once
#include "SDL.h"

namespace math {

	float angleBetweenTwoPoints(float x1, float y1, float x2, float y2);
	float angleBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
	bool collBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
	float distBetweenTwoPoints(float x1, float y1, float x2, float y2);
	float distBetweenTwoRects(SDL_Rect &r1, SDL_Rect &r2);
	template <class T> T randomNumber(T mod) { return rand() % mod; }
	float sweptAABB(SDL_Rect &boxA, float vx, float vy, SDL_Rect &boxB, float &normalX, float &normalY);
}