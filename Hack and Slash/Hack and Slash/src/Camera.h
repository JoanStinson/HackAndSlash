#pragma once
#include "Entity.h"

class Camera {
public:
	Entity *target = nullptr;
	float lerp = 2.6f;

	void Update();
};
