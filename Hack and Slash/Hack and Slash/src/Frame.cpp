#include "Frame.h"
#include "Frame.h"
#include "Screen.h"
#include "Globals.h"

void Frame::Draw(SDL_Texture *spritesheet, float x, float y) {
	SDL_Rect dest; // destination of where we want to draw this frame
	dest.x = x - offSet.x;
	dest.y = y - offSet.y;
	dest.w = clip.w;
	dest.h = clip.h;

	renderTexture(spritesheet, globals::renderer, dest, &clip);
}

void Frame::LoadFrame(ifstream &file, list<DataGroupType> &groupTypes) {
	// Ok, we dragged these groupTypes allllll the way here, so we could construct groups on our frame to hold the data in
	GroupBuilder::buildGroups(groupTypes, frameData);

	string buffer;
	// frame - dud data saying this is a frame
	getline(file, buffer);
	// clip
	getline(file, buffer);
	stringstream ss;
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h; // cin >> clip.x >> clip.y >> etc
	// offset/pivot
	getline(file, buffer);
	ss.clear();
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offSet.x >> offSet.y;
	// duration
	getline(file, buffer);
	ss.clear();
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;
	// index
	getline(file, buffer);
	ss.clear();
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNumber;

	GroupBuilder::loadGroups(file, frameData);
}
