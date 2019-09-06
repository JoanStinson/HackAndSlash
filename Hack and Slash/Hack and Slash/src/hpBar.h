#pragma once
#include "livingEntity.h"

class HPBar {
public:
	int x, y;
	int barWidth = 200, barHeight = 30;
	LivingEntity *entity = NULL;

	void draw();
};