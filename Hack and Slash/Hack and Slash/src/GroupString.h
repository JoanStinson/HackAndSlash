#ifndef GROUPSTRING_H
#define GROUPSTRING_H

#include "group.h"

class GroupString : public Group{
public:
	list<string> data;

	GroupString(DataGroupType type){
		this->type = type;
	}
	int numberOfDataInGroup(){
		return data.size();
	}
	void addToGroup(string str){
		//bah, if we got a whitespace at the front, lets get rid of that
		if (str[0] == ' '){
			str.erase(0, 1);
		}

		data.push_back(str);
	}

	void draw(){
		//TODO
	}

};


#endif