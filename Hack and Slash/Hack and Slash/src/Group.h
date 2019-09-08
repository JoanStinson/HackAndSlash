#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <SDL.h>
#include "DataGroupType.h"
using namespace std;

/*! Abstract class 
Groups manage a frames data
*/
class Group {
public:
	Group() {}
	Group(DataGroupType type) : type(type) {}
	virtual int GetGroupSize() = 0;									//!< How many bits of data in here
	virtual void AddToGroup(string str) = 0;						//!< Add to group using string. Converts string into correct data for each group type

	DataGroupType type;												/*!< Describes its name, type and other rules */
};