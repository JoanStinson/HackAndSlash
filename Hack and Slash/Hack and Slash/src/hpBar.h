#pragma once
#include "Creature.h"

class HpBar {
public:
	int x, y;
	int barWidth = 200, barHeight = 30;
	Creature *entity = nullptr;

	void Draw();
};