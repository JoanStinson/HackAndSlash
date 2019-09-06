#pragma once
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "DrawingFunctions.h"
#include "Globals.h"
#include "GroupBuilder.h"
using namespace std;

class Frame{
public:
	void Draw(SDL_Texture *spriteSheet, float x, float y);
	void LoadFrame(ifstream &file, list<DataGroupType> &groupTypes);

	int frameNumber; //or frame index
	SDL_Rect clip; //its the region on the spritesheet where this frame is
	float duration; //how long does frame run for
	SDL_Point offSet; //pivot point to help align frames in an animation
	list<Group*> frameData;//holds groups of additional data for the frame
};