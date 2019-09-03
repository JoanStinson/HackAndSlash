#pragma once
#include "Animation.h"
using namespace std;

class AnimationSet {
public:
	string imageName;
	SDL_Texture *spriteSheet; // holds the spritesheet image for all of our animations
	SDL_Texture *whiteSpriteSheet = nullptr; // use this spritesheet to show damage
	list<Animation> animations;

	~AnimationSet(); // properly cleans up any dynamic memory e.g textures

	Animation* getAnimation(string name);

	void loadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColorKey = false, int transparentPixelIndex = 0,
		bool createWhiteTexture = false);

};