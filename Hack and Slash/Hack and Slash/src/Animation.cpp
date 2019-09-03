#include "Animation.h"

Animation::Animation(string name) : name(name) {

}

int Animation::GetNextFrameNumber(int frameNumber) {
	if (frameNumber + 1 < frames.size()) {
		return frameNumber + 1;
	}
	else {
		return 0;
	}
}

Frame* Animation::GetNextFrame(Frame *frame) {
	return GetFrame(GetNextFrameNumber(frame->frameNumber));
}

int Animation::GetEndFrameNumber() {
	return frames.size() - 1;
}

Frame* Animation::GetFrame(int frameNumber) {
	if (frames.size() == 0) {
		return nullptr;
	}

	auto it = frames.begin(); // point iterator to first frame in the m_frames list
	int counter = 0;
	for (counter = 0; counter < frameNumber && counter < GetEndFrameNumber(); counter++) {
		it++; // make iterator point to the next frame in the list
	}

	Frame *frame = &(*it); // make frame point to the frame inside the list our iterator is pointing at
	// *it = gets frame at it's position in the list
	// & = get the memory address of whatever is on the right

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
