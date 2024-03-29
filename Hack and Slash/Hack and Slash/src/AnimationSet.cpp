#include "AnimationSet.h"
#include "Utils.h"

AnimationSet::~AnimationSet() {
	SDL_DestroyTexture(spriteSheet);
	if (whiteSpriteSheet != nullptr)
		SDL_DestroyTexture(whiteSpriteSheet);
}

Animation* AnimationSet::GetAnimation(const string &name) {
	for (list<Animation>::iterator animation = animations.begin(); animation != animations.end(); animation++) {
		Animation *anim = &(*animation);

		// If we find a match on name, return that animation from the list
		if (name == anim->GetName()) {
			return anim;
		}
	}

	return nullptr;
}

/*! Loads a fdset file, also takes a list of what types of data we expect to loading.
* Also, if we're working with an 8bit image and we know in that 8bit image what coloured pixel in the palette
* we would prefer to be transparent, then we can say yes to 'colorKeying'(make a colour in the palette actually equal full transparent)
* and then just use the index of that transparentPixel (e.g, if its the third colour in the palette, then put in 2 as index starts at 0)
* if you need an alternative white version of the sprite sheet, then make this last option true (maybe move this out of this class? not sure)	
*/
void AnimationSet::LoadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey, int transparentPixelIndex, bool createWhiteTexture) {

	ifstream file;
	string resPath = utils::getResourcePath();
	file.open(resPath + fileName);
	if (file.good()) {
		getline(file, imageName);
		if (setColourKey) {
			SDL_Surface *spriteSurface = Renderer::Instance().LoadSurface(resPath + imageName);

			// For transparency, we will grab the [transparentPixelIndex] from the surface we just made
			SDL_Color *transparentPixel = &spriteSurface->format->palette->colors[transparentPixelIndex];
			SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

			spriteSheet = Renderer::Instance().ConvertSurfaceToTexture(spriteSurface, false);

			if (createWhiteTexture) {
				SDL_Surface* whiteSurface = Renderer::Instance().LoadSurface(resPath + "allwhite.png");
				Renderer::Instance().SurfacePaletteSwap(spriteSurface, whiteSurface);
				SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
				whiteSpriteSheet = Renderer::Instance().ConvertSurfaceToTexture(spriteSurface, false); // Create the texture whilst destroying the surface

				SDL_FreeSurface(whiteSurface);
			}
			else {
				whiteSpriteSheet = nullptr;
			}

			SDL_FreeSurface(spriteSurface);
		}
		else
			spriteSheet = Renderer::Instance().LoadTexture(resPath + imageName);

		string buffer;
		getline(file, buffer);
		stringstream ss;
		buffer = utils::clipOffDataHeader(buffer);
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

SDL_Texture* AnimationSet::GetSpriteSheet() const {
	return spriteSheet;
}

SDL_Texture* AnimationSet::GetWhiteSpriteSheet() const {
	return whiteSpriteSheet;
}
