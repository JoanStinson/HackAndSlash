#include "GroupBuilder.h"
#include "Utils.h"

const bool GroupBuilder::SavedInGroups = false;

template <class T>
Group* GroupBuilder::BuildGroup(DataGroupType dataType) {
	Group *group = new GroupType<T>(dataType);
	return group;
}

void GroupBuilder::BuildGroups(list<DataGroupType> groupTypes, list<Group*> &groups) {
	for (list<DataGroupType>::iterator dgt = groupTypes.begin(); dgt != groupTypes.end(); dgt++) {
		if ((*dgt).dataType == (*dgt).BOX) {
			Group *group = BuildGroup<SDL_Rect>((*dgt));
			groups.push_back(group);
		}
		else if ((*dgt).dataType == (*dgt).NUMBER) {
			Group *group = BuildGroup<float>((*dgt));
			groups.push_back(group);
		}
		else if ((*dgt).dataType == (*dgt).POSITION) {
			Group *group = BuildGroup<SDL_Point>((*dgt));
			groups.push_back(group);
		}
		else if ((*dgt).dataType == (*dgt).STRING) {
			Group *group = BuildGroup<string>((*dgt));
			groups.push_back(group);
		}
	}
}

Group* GroupBuilder::AddGroupStringToGroup(string name, list<Group*> &groups) {
	DataGroupType dgt;
	dgt.dataType = DataGroupType::STRING;
	dgt.groupName = name;
	dgt.singleItem = false;
	Group *group = new GroupType<string>(dgt);
	groups.push_back(group); // Added some random group on the fly for this frame only
	return group;
}

void GroupBuilder::LoadGroups(ifstream &file, list<Group*> &groups) {
	// Read each group in
	while (!file.eof()) {

		// First, get the next line out of the file
		int positionBeforeRead = file.tellg();
		string line;
		getline(file, line);
		if (line.empty() || line == "")
			break; // Must actually be at the end of the file
		if (SavedInGroups) {
			// Find the semi colon
			int pos = line.find(":", 0);
			if (pos == -1) {
				file.seekg(positionBeforeRead);
				break; 
			}
			Group *group = FindGroupByName(line.substr(0, pos), groups);
			if (group == nullptr) {
				// Can't find the group, but lets not lose the data
				group = AddGroupStringToGroup(line.substr(0, pos), groups);
			}
			// Get the rest of the string and work out how many of these we have to build
			string numStr = line.substr(pos + 1, line.length() - pos + 2);
			stringstream ss;
			ss << numStr << endl;
			int num;
			ss >> num;
			// Finally can read in all the data
			for (int i = 0; i < num; i++) {
				if (!file.good())
					break;

				getline(file, line);

				group->AddToGroup(line);
			}

		}
		else {
			// Dealing with 1 item at a time, regardless of in a group or not
			// Find the semi colon
			int pos = line.find(":", 0);
			if (pos == -1) {
				file.seekg(positionBeforeRead);
				break; 
			}
			Group *group = FindGroupByName(line.substr(0, pos), groups);
			if (group == nullptr) {
				// Can't find the group, but lets not lose the data
				group = AddGroupStringToGroup(line.substr(0, pos), groups);
			}
			// Clean up string so we can add data to the group
			line = utils::clipOffDataHeader(line); // Get rid of the 'groupName: '
			group->AddToGroup(line);
		}
	}

}

Group* GroupBuilder::FindGroupByName(string str, list<Group*> &groups) {
	for (list<Group*>::iterator group = groups.begin(); group != groups.end(); group++) {
		if (str == (*group)->type.groupName)
			return (*group);
	}
	return nullptr;
}