#include "Frame.h"
#include "Utils.h"

void Frame::Draw(SDL_Texture* spriteSheet, float x, float y) {
	SDL_Rect dest; // Destination of where we want to draw this frame
	dest.x = x - offSet.x;
	dest.y = y - offSet.y;
	dest.w = clip.w;
	dest.h = clip.h;
	RENDERER.RenderTexture(spriteSheet, dest, &clip);
}

void Frame::LoadFrame(ifstream &file, list<DataGroupType> &groupTypes) {
	// Cnstruct groups on our frame to hold the data in
	GroupBuilder::BuildGroups(groupTypes, frameData);

	string buffer;
	// Frame - dud data saying this is a frame
	getline(file, buffer);
	// Clip
	getline(file, buffer);
	stringstream ss; // Good for building strings and reading from strings
	buffer = utils::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h;
	// Offset/pivot
	getline(file, buffer);
	ss.clear();
	buffer = utils::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offSet.x >> offSet.y;
	// Duration
	getline(file, buffer);
	ss.clear();
	buffer = utils::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;
	// Index
	getline(file, buffer);
	ss.clear();
	buffer = utils::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNumber;

	GroupBuilder::LoadGroups(file, frameData);
}

int Frame::GetFrameNumber() const {
	return frameNumber;
}

float Frame::GetDuration() const {
	return duration;
}

SDL_Point& Frame::GetOffSet() {
	return offSet;
}

list<Group*>& Frame::GetFrameData() {
	return frameData;
}
