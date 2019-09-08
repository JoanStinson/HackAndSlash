#pragma once
#include "Frame.h"
using namespace std;

class Animation {
public:
	Animation(const string &name = "");
	int GetNextFrameNumber(int frameNumber) const; //returns the next frame number in the list
	Frame* GetNextFrame(Frame *frame);
	int GetEndFrameNumber() const; //returns the last frames number
	Frame* GetFrame(int frameNumber); //get frame using frame number
	void LoadAnimation(ifstream &file, list<DataGroupType> &groupTypes);
	string GetName() const;

private:
	string name; //name of the animation
	list<Frame> frames; //list of our frames
};