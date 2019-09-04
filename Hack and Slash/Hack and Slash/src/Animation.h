#ifndef ANIMATION
#define ANIMATION

#include "frame.h"

using namespace std;

class Animation{
public:
	string name; //name of the animation
	list<Frame> frames; //list of our frames

	Animation(string name = "");

	int getNextFrameNumber(int frameNumber);//returns the next frame number in the list
	Frame *getNextFrame(Frame* frame);
	int getEndFrameNumber(); //returns the last frames number
	Frame* getFrame(int frameNumber); //get frame using frame number

	void loadAnimation(ifstream &file, list<DataGroupType> &groupTypes);
};

#endif