#ifndef DATAGROUPTYPE_H
#define DATAGROUPTYPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

//Frame data is all about the data. These types help to describe what types of data are in a group
class DataGroupType{
public:
	string groupName; //e.g hitBoxes
	int dataType; //what do we expect to see here e.g string, position, rectangle region, rotation, et
	bool singleItem; //can this datagroup only have 1 item inside (usually false, but you never know)

	//saving and loading these for RuleSets(RS)
	static void saveRSDataGroupType(ofstream &file, DataGroupType dataGroupType);
	static DataGroupType loadRSDataGroupType(ifstream &file);

	//STATIC DATA TYPES
	static const int DATATYPE_STRING = 0, DATATYPE_POSITION = 1, DATATYPE_BOX = 2, DATATYPE_NUMBER = 3;

};

#endif