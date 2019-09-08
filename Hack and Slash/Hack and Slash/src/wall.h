#pragma once
#include "Entity.h"

class Wall : public Entity {
public:
	Wall(AnimationSet *animSet);
	void Update();

private:
	void ChangeAnimation(int newState, bool resetAnim = true) override;
};