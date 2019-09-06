#pragma once

template <class T>
class GroupType : public Group {
public:
	GroupType(DataGroupType type) : Group(type) {}

	list<SDL_Rect>& GetBoxData() { return boxData; }
	list<float>& GetNumData() { return numData; }
	list<SDL_Point>& GetPosData() { return posData; }
	list<string>& GetStrData() { return strData; }

	int GetGroupSize() {
		if (type.dataType == type.DATATYPE_BOX)
			return boxData.size();
		else if (type.dataType == type.DATATYPE_NUMBER)
			return numData.size();
		else if (type.dataType == type.DATATYPE_POSITION)
			return posData.size();
		else if (type.dataType == type.DATATYPE_STRING)
			return strData.size();
	}

	void AddToGroup(string str) override {
		stringstream ss;
		if (type.dataType == type.DATATYPE_BOX) {
			SDL_Rect box;
			ss << str;
			ss >> box.x >> box.y >> box.w >> box.h;
			boxData.push_back(box);
		}
		else if (type.dataType == type.DATATYPE_NUMBER) {
			float num;
			ss << str;
			ss >> num;
			numData.push_back(num);
		}
		else if (type.dataType == type.DATATYPE_POSITION) {
			SDL_Point pos;
			ss << str;
			ss >> pos.x >> pos.y;
			posData.push_back(pos);
		}
		else if (type.dataType == type.DATATYPE_STRING) {
			if (str[0] == ' ')
				str.erase(0, 1);
			strData.push_back(str);
		}
		ss.clear();
	}

private:
	list<SDL_Rect> boxData;
	list<float> numData;
	list<SDL_Point> posData;
	list<string> strData;
};