#pragma once
#include <string>
#include "Frame.h"
using namespace std;

class Animation {
public:
	string m_name; 
	list<Frame> m_frames;

	Animation(string name = "");

	int GetNextFrameNumber(int frameNumber); // returns the next frame number in the list
	Frame* GetNextFrame(Frame *frame);
	int GetEndFrameNumber(); // returns the last frames number
	Frame* GetFrame(int frameNumber); // get frame using frame number

	void LoadAnimation(ifstream &file, list<DataGroupType> &groupTypes);

};