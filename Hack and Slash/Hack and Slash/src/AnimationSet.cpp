#include "AnimationSet.h"
#include "Globals.h"

AnimationSet::~AnimationSet() {
	SDL_DestroyTexture(spriteSheet);
	if (whiteSpriteSheet != nullptr)
		SDL_DestroyTexture(whiteSpriteSheet);
}

Animation* AnimationSet::GetAnimation(string name) {
	for (list<Animation>::iterator animation = animations.begin(); animation != animations.end(); animation++) {
		Animation* anim = &(*animation);

		//if we find a match on name, return that animation from the list
		if (name == anim->name) {
			return anim;
		}
	}

	return nullptr;
}

//loads a fdset file, also takes a list of what types of data we expect to loading.
//Also, if we're working with an 8bit image and we know in that 8bit image what coloured pixel in the palette 
//we would prefer to be transparent, then we can say yes to 'colorKeying'(make a colour in the palette actually equal full transparent)
//and then just use the index of that transparentPixel (e.g, if its the third colour in the palette, then put in 2 as index starts at 0)
//if you need an alternative white version of the sprite sheet, then make this last option true (maybe move this out of this class? not sure)	
void AnimationSet::LoadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey, int transparentPixelIndex, bool createWhiteTexture) {

	ifstream file;
	string resPath = globals::getResourcePath();
	file.open(resPath + fileName);
	if (file.good()) {
		getline(file, imageName);
		if (setColourKey) {
			SDL_Surface* spriteSurface = LoadSurface(resPath + imageName, globals::renderer);

			//for transparency, we will grab the [transparentPixelIndex] from the surface we just made
			SDL_Color* transparentPixel = &spriteSurface->format->palette->colors[transparentPixelIndex];
			SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

			spriteSheet = ConvertSurfaceToTexture(spriteSurface, globals::renderer, false);

			if (createWhiteTexture) {
				SDL_Surface* whiteSurface = LoadSurface(resPath + "allwhite.png", globals::renderer);
				SurfacePaletteSwap(spriteSurface, whiteSurface);
				SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
				whiteSpriteSheet = ConvertSurfaceToTexture(spriteSurface, globals::renderer, false); //create the texture whilst destroying the surface

				SDL_FreeSurface(whiteSurface);
			}
			else {
				whiteSpriteSheet = nullptr;
			}

			SDL_FreeSurface(spriteSurface);
		}
		else
			spriteSheet = LoadTexture(resPath + imageName, globals::renderer);

		string buffer;
		getline(file, buffer);
		stringstream ss;
		buffer = globals::clipOffDataHeader(buffer);
		ss << buffer;
		int numberOfAnimations;
		ss >> numberOfAnimations;

		for (int i = 0; i < numberOfAnimations; i++) {
			Animation newAnimation;
			newAnimation.LoadAnimation(file, groupTypes);
			animations.push_back(newAnimation);
		}

	}
	file.close();
}