#include "Frame.h"
#include "Frame.h"
#include "Screen.h"

void Frame::Draw(SDL_Texture *spritesheet, float x, float y) {
	SDL_Rect dest; // destination of where we want to draw this frame
	dest.x = x - m_offSet.x;
	dest.y = y - m_offSet.y;
	dest.w = m_clip.w;
	dest.h = m_clip.h;

	renderTexture(spritesheet, Screen::GetRenderer(), dest, &m_clip);
}

void Frame::LoadFrame(ifstream &file, list<DataGroupType> &groupTypes) {
	// Ok, we dragged these groupTypes allllll the way here, so we could construct groups on our frame to hold the data in
	GroupBuilder::buildGroups(groupTypes, m_frameData);

	string buffer;
	// frame - dud data saying this is a frame
	getline(file, buffer);
	// clip
	getline(file, buffer);
	stringstream ss;
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> m_clip.x >> m_clip.y >> m_clip.w >> m_clip.h; // cin >> clip.x >> clip.y >> etc
	// offset/pivot
	getline(file, buffer);
	ss.clear();
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> m_offSet.x >> m_offSet.y;
	// duration
	getline(file, buffer);
	ss.clear();
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> m_duration;
	// index
	getline(file, buffer);
	ss.clear();
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> m_frameNumber;

	GroupBuilder::loadGroups(file, m_frameData);
}
