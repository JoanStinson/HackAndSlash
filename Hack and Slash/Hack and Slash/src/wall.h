#pragma once
#include "Entity.h"

class Wall : public Entity {
public:
	Wall(AnimationSet *animSet);
	void Update();
	void ChangeAnimation(int newState, bool resetFrameToBeginning);
	void UpdateCollisions() {
		//dont do anything. Walls dont move or get pushed in this game
	}
};