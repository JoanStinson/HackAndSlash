#pragma once
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "Renderer.h"
#include "GroupBuilder.h"
using namespace std;

class Frame {
public:
	void Draw(SDL_Texture *spriteSheet, float x, float y);
	void LoadFrame(ifstream &file, list<DataGroupType> &groupTypes);
	int GetFrameNumber() const;
	float GetDuration() const;
	SDL_Point& GetOffSet();
	list<Group*>& GetFrameData();

private:
	int frameNumber;													/*!< Frame index */
	SDL_Rect clip;														/*!< Its the region on the spritesheet where this frame is */
	float duration;														/*!< How long does frame run for */
	SDL_Point offSet;													/*!< Pivot point to help align frames in an animation */
	list<Group*> frameData;												/*!< Holds groups of additional data for the frame */
};