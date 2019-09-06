#ifndef GROUPBOX_H
#define GROUPBOX_H

#include "group.h"

//class GroupBox : public Group{
//public:
//	list<SDL_Rect> data;
//
//	GroupBox(DataGroupType type){
//		this->type = type;
//	}
//
//	int numberOfDataInGroup(){
//		return data.size();
//	}
//	
//	void addToGroup(string str){
//		//hope this works :/
//		stringstream ss;
//		ss << str;
//		SDL_Rect box;
//		ss >> box.x >> box.y >> box.w >> box.h;
//
//		data.push_back(box);
//	}
//
//	void draw(){
//		//TODO
//	}
//
//};

template <class T>
class Type: public Group {
public:
	Type(DataGroupType type) : Group(type) {}

	int GetGroupSize() { 
		if (type.dataType == type.DATATYPE_BOX) //SDL_Rect
			return boxData.size();
		else if (type.dataType == type.DATATYPE_NUMBER) //float
			return numberData.size();
		else if (type.dataType == type.DATATYPE_POSITION) //SDL_Point
			return posData.size();
		else if (type.dataType == type.DATATYPE_STRING) //string
			return strData.size();
	}

	SDL_Rect GetBoxFront() {
		return boxData.front();
	}
	float GetNumberFront() {
		return numberData.front();
	}
	SDL_Point GetPosFront() {
		return posData.front();
	}
	string GetStrFront() {
		return strData.front();
	}

	void AddToGroup(string str) override {
		if (type.dataType == type.DATATYPE_BOX) //SDL_Rect
			AddBox(str);
		else if (type.dataType == type.DATATYPE_NUMBER) //float
			AddNumber(str);
		else if (type.dataType == type.DATATYPE_POSITION) //SDL_Point
			AddPosition(str);
		else if (type.dataType == type.DATATYPE_STRING) //string
			AddString(str);
	}

private:
	void AddBox(string str) {
		stringstream ss;
		SDL_Rect box;
		ss << str;
		ss >> box.x >> box.y >> box.w >> box.h;
		boxData.push_back(box);
	}

	void AddNumber(string str) {
		stringstream ss;
		float number;
		ss << str;
		ss >> number;
		numberData.push_back(number);
	}

	void AddPosition(string str) {
		stringstream ss;
		SDL_Point pos;
		ss << str;
		ss >> pos.x >> pos.y;
		posData.push_back(pos);
	}

	void AddString(string str) {
		if (str[0] == ' ')
			str.erase(0, 1);
		strData.push_back(str);
	}

	list<SDL_Rect> boxData;
	list<float> numberData;
	list<SDL_Point> posData;
	list<string> strData;
};


#endif