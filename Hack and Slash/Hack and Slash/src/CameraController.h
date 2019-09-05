#pragma once

#include "globals.h"
#include "entity.h"

class CameraController {
public:
	Entity *target = NULL;
	float lerp = 2.6f;

	void update();
};
