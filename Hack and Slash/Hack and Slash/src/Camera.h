#pragma once
#include "Globals.h"
#include "Entity.h"

class Camera {
public:
	Entity *target = NULL;
	float lerp = 2.6f;

	void Update();
};
