#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <SDL.h>
#include "DataGroupType.h"
using namespace std;

//ABSTRACT CLASS
//groups manage a frames data
class Group{
public:
	DataGroupType type; //describes its name, type and other rules
	Group(){
		//nothing...
	}
	Group(DataGroupType type){
		this->type = type;
	}
	//how many bits of data in here
	virtual int GetGroupSize() = 0;
	/*add to group using string. Converts string into correct data for each group type*/
	virtual void AddToGroup(string str) = 0;


	//virtual void draw() = 0;
};