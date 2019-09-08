#pragma once
#include <string>
using namespace std;

/*! Frame data is all about the data. These types help to describe what types of data are in a group */
struct DataGroupType {
	enum Type { BOX, NUMBER, POSITION, STRING };
	string groupName;												/*!< e.g hitBoxes */
	int dataType;													/*!< What do we expect to see here e.g string, position, rectangle region, rotation, etc */
	bool singleItem;												/*!< Can this datagroup only have 1 item inside (usually false, but you never know) */
};