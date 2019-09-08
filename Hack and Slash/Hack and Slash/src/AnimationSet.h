#pragma once
#include "Animation.h"
using namespace std;

class AnimationSet {
public:
	~AnimationSet();													//!< Properly cleans up any dynamic memory e.g textures
	Animation* GetAnimation(const string &name);
	void LoadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
	SDL_Texture* GetSpriteSheet() const;
	SDL_Texture* GetWhiteSpriteSheet() const;

private:
	string imageName;
	SDL_Texture *spriteSheet = nullptr;									/*!< Holds the spritesheet image for all of our animations */
	SDL_Texture *whiteSpriteSheet = nullptr;							/*!< I use this spritesheet to show damage */
	list<Animation> animations;
};