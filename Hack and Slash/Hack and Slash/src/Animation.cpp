#include "Animation.h"

Animation::Animation(string name) : name(name) {

}

int Animation::GetNextFrameNumber(int frameNumber) {
	//return (frameNumber + 1) % frames.size();
	if (frameNumber + 1 < frames.size())
		return frameNumber + 1;
	else
		return 0;
}

Frame* Animation::GetNextFrame(Frame* frame) {
	return GetFrame(GetNextFrameNumber(frame->frameNumber));
}

int Animation::GetEndFrameNumber() {
	return frames.size() - 1;
}

Frame* Animation::GetFrame(int frameNumber) {
	if (frames.size() == 0)
		return nullptr;

	list<Frame>::iterator i = frames.begin(); //point iterator to first frame in the frames list

	for (int counter = 0; counter < frameNumber && counter < GetEndFrameNumber(); counter++) {
		i++; //make iterator point to the next frame in the list
	}

	Frame *frame = &(*i); //make frame point to the frame inside the list our iterator is pointing at
						//*i = gets frame at i's position in the list (de-referencing)
						//& = get the memory address of whatever is on the right (referencing)

	return frame;
}

void Animation::LoadAnimation(ifstream &file, list<DataGroupType> &groupTypes) {
	getline(file, name);
	string buffer;
	getline(file, buffer);
	stringstream ss;
	buffer = globals::clipOffDataHeader(buffer);
	ss << buffer;
	int numberOfFrames;
	ss >> numberOfFrames;

	for (int i = 0; i < numberOfFrames; i++) {
		Frame newFrame;
		newFrame.LoadFrame(file, groupTypes);
		frames.push_back(newFrame);
	}
}