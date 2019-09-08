#pragma once
#include "Entity.h"

class Camera {
public:
	void Update();
	void SetTarget(Entity *entity);

private:
	Entity *target = nullptr;
	float lerp = 2.6f;
};
