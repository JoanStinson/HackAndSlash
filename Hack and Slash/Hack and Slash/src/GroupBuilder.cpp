#include "groupBuilder.h"

const bool GroupBuilder::savedInGroups = false;//TODO change this if fdset has saveInGroups selected

Group* GroupBuilder::buildGroup(DataGroupType dataType)
{
	Group* group = NULL;

	if (dataType.dataType == DataGroupType::DATATYPE_NUMBER)
	{
		group = new GroupNumber(dataType);
	}
	else if (dataType.dataType == DataGroupType::DATATYPE_POSITION)
	{
		group = new GroupPosition(dataType);
	}
	else if (dataType.dataType == DataGroupType::DATATYPE_BOX)
	{
		group = new GroupBox(dataType);
	}
	else{
		group = new GroupString(dataType);
	}

	return group;
}

void GroupBuilder::buildGroups(list<DataGroupType> groupTypes, list<Group*> &groups){
	for (list<DataGroupType>::iterator dgt = groupTypes.begin(); dgt != groupTypes.end(); dgt++)
	{
		Group *group = buildGroup((*dgt));
		groups.push_back(group);
	}
}

Group* GroupBuilder::addGroupStringToGroup(string name, list<Group*> &groups){
	DataGroupType dgt;
	dgt.dataType = DataGroupType::DATATYPE_STRING;
	dgt.groupName = name;
	dgt.singleItem = false;
	Group *group = new GroupString(dgt);
	groups.push_back(group); //ok, we just added some random group on the fly for this frame only. cool, I hope
	return group;
}
void GroupBuilder::loadGroups(ifstream &file, list<Group*> &groups) {
	//read each group in:
	while (!file.eof())
	{

		//first, get the next line out of the file
		int positionBeforeRead = file.tellg();
		string line;
		getline(file, line);
		if (line.empty() || line == "")
			break; //must actually be at the end of the file
		if (savedInGroups)
		{
			//find the semi colon
			int pos = line.find(":", 0);
			if (pos == -1)
			{
				file.seekg(positionBeforeRead);
				break; //must actually be reading something else, like the next animation or something. TODO how to deal with this?
				//TODO
				//somehow backtrack to the start of this line, so the rest of the reading can happen for other animations
			}
			Group *group = findGroupByName(line.substr(0, pos), groups);
			if (group == NULL){
				//can't find the group, but lets not lose the data
				group = addGroupStringToGroup(line.substr(0, pos), groups);
			}
			//ok, get the rest of the string and work out how many of these we have to build
			string numStr = line.substr(pos + 1, line.length() - pos + 2);
			stringstream ss;
			ss << numStr << endl;
			int num;
			ss >> num;
			//finally can read in all the data
			for (int i = 0; i < num; i++)
			{
				if (!file.good())
					break;

				getline(file, line);

				group->addToGroup(line);
			}
				 
		}
		else
		{
			//dealing with 1 item at a time, regardless of in a group or not
			//find the semi colon

			int pos = line.find(":", 0);
			if (pos == -1){
				file.seekg(positionBeforeRead);
				break; //must actually be reading something else, like the next animation or something. TODO how to deal with this?
				//TODO
				//somehow backtrack to the start of this line, so the rest of the reading can happen for other animations
			}
			cout << "sub: " << line.substr(0, pos) << endl;
			Group *group = findGroupByName(line.substr(0, pos), groups);
			if (group == NULL){
				//can't find the group, but lets not lose the data
				group = addGroupStringToGroup(line.substr(0, pos), groups);
			}
			//clean up string so we can add data to the group
			line = Globals::clipOffDataHeader(line);//get rid of the 'groupName: '
			group->addToGroup(line);
			//done, keep going
			
		}
	}

}

Group* GroupBuilder::findGroupByName(string str, list<Group*> &groups){
	for (list<Group*>::iterator group = groups.begin(); group != groups.end(); group++)
	{
		if (str == (*group)->type.groupName)
			return (*group);
	}

	return NULL;
}