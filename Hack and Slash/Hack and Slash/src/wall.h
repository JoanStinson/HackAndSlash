#ifndef WALL
#define WALL

#include "entity.h"

class Wall : public Entity{
public:
	Wall(AnimationSet *animSet);
	void update();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateCollisions(){
		//dont do anything. Walls dont move or get pushed in this game
	}
};

#endif