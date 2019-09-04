#ifndef GROUPNUMBER_H
#define GROUPNUMBER_H

#include "group.h"

class GroupNumber : public Group{
public:
	list<float> data;

	GroupNumber(DataGroupType type){
		this->type = type;
	}
	int numberOfDataInGroup(){
		return data.size();
	}
	void addToGroup(string str){
		//hope this works :/
		stringstream ss;
		ss << str;
		float r;
		ss >>r;

		data.push_back(r);
	}

	void draw(){
		//TODO
	}

};


#endif