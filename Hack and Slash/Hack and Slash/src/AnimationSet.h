#pragma once
#include "Animation.h"
using namespace std;

class AnimationSet{
public:
	~AnimationSet(); //properly cleans up any dynamic memory e.g textures
	Animation* GetAnimation(string name);
	void LoadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
	SDL_Texture* GetSpriteSheet();
	SDL_Texture* GetWhiteSpriteSheet();

private:
	string imageName;
	SDL_Texture* spriteSheet; //holds the spritesheet image for all of our animations
	SDL_Texture* whiteSpriteSheet = nullptr; //I use this spritesheet to show damage
	list<Animation> animations;
};