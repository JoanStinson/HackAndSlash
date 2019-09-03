#include "AnimationSet.h"
#include "Screen.h"

AnimationSet::~AnimationSet() {
	SDL_DestroyTexture(m_spriteSheet);
	if (m_whiteSpriteSheet != nullptr)
		SDL_DestroyTexture(m_whiteSpriteSheet);
}

Animation* AnimationSet::getAnimation(string name) {
	Animation *anim = nullptr;

	for (auto a : m_animations) {

		if (name == anim->m_name) {
			anim = &a; // &a if it's an object, &(*a) if it's an iterator
			break;
		}

	}

	return anim;
}

//loads a fdset file, also takes a list of what types of data we expect to loading.
//Also, if we're working with an 8bit image and we know in that 8bit image what coloured pixel in the palette 
//we would prefer to be transparent, then we can say yes to 'colorKeying'(make a colour in the palette actually equal full transparent)
//and then just use the index of that transparentPixel (e.g, if its the third colour in the palette, then put in 2 as index starts at 0)
//if you need an alternative white version of the sprite sheet, then make this last option true (maybe move this out of this class? not sure)	
void AnimationSet::loadAnimationSet(string fileName, list<DataGroupType>& groupTypes, bool setColorKey, int transparentPixelIndex, bool createWhiteTexture) {
	ifstream file;
	string resPath = getResourcePath();
	file.open(resPath + fileName);
	if (file.good()) {
		getline(file, m_imageName);
		if (setColorKey) {
			SDL_Surface* spriteSurface = loadSurface(resPath + m_imageName, Screen::GetRenderer());

			//for transparency, we will grab the [transparentPixelIndex] from the surface we just made
			SDL_Color* transparentPixel = &spriteSurface->format->palette->colors[transparentPixelIndex];
			SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

			m_spriteSheet = convertSurfaceToTexture(spriteSurface, Screen::GetRenderer(), false);

			if (createWhiteTexture) {
				SDL_Surface* whiteSurface = loadSurface(resPath + "allwhite.png", Screen::GetRenderer());
				surfacePaletteSwap(spriteSurface, whiteSurface);
				SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
				m_whiteSpriteSheet = convertSurfaceToTexture(spriteSurface, Screen::GetRenderer(), false); //create the texture whilst destroying the surface

				SDL_FreeSurface(whiteSurface);
			}
			else {
				m_whiteSpriteSheet = nullptr;
			}

			SDL_FreeSurface(spriteSurface);
		}
		else
			m_spriteSheet = loadTexture(resPath + m_imageName, Screen::GetRenderer());

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
			m_animations.push_back(newAnimation);
		}

	}
	file.close();

}
