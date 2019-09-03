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

class Frame {
public:
	void Draw(SDL_Texture *spritesheet, float x, float y);
	void LoadFrame(ifstream &file, list<DataGroupType> &groupTypes);

	int m_frameNumber; // frame index
	SDL_Rect m_clip; // it's the region on the spritesheet where this frame is
	float m_duration; // how long does frame run for
	SDL_Point m_offSet; // pivot point to help align frames in an animation

	list<Group*> m_frameData; // holds groups of additional data for the frame
};