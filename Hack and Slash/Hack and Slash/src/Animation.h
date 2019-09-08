#pragma once
#include "Frame.h"
using namespace std;

class Animation {
public:
	Animation(const string &name = "");
	int GetNextFrameNumber(int frameNumber) const;							//!< Returns the next frame number in the list
	Frame* GetNextFrame(Frame *frame);										//!< Returns the next frame in the list
	int GetEndFrameNumber() const;											//!< Returns the last frames number
	Frame* GetFrame(int frameNumber);										//!< Get frame using frame number
	void LoadAnimation(ifstream &file, list<DataGroupType> &groupTypes);
	string GetName() const;

private:
	string name;															/*!< Name of the animation */
	list<Frame> frames;														/*!< List of our frames */
};