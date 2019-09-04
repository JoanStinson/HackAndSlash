#ifndef GROUPBUILDER
#define GROUPBUILDER

#include "globals.h"
#include "group.h"
#include "groupBox.h"
#include "groupPosition.h"
#include "GroupNumber.h"
#include "groupString.h"

class GroupBuilder
{
public:
	/**
	NOTE: This is the only one a user has to set. As it depends on if they have their data output in groups or not
	In groups example:
	hitboxes: 2
	12 12 34 56
	24 45 57 79
	Not In Groups: (DEFAULT CHOICE)
	hitboxes: 12 12 34 56
	hitboxes: 24 45 57 79
	If in doubt, check the fdset and see what happens with the data
	*/
	static const bool savedInGroups; //TODO - Change based on above explaination

	/**
	builds a Group based on dataType. default is STRING (because we may want to ignore some data in a set, but still load it)
	*/
	static Group* buildGroup(DataGroupType dataType);
	/**
	Builds a set of Groups based on dataGroupTypes list
	*/
	static void buildGroups(list<DataGroupType> groupTypes, list<Group*> &groups);

	/**
	Add GroupString to groups (build on the fly). Gives back a reference, if you need one
	*/
	static Group* addGroupStringToGroup(string name, list<Group*> &groups);
	/**
	Loads a group out to a file
	*/
	static void loadGroups(ifstream &file, list<Group*> &groups);
	/**
	Finds a group based on name. Can give back a null
	*/
	static Group* findGroupByName(string str, list<Group*> &groups);

};

#endif