#include "Globals.h"

#include "Globals.h"

namespace globals {

	string clipOffDataHeader(string data) {
		int pos = data.find(":", 0); // returns where we find the ':' in the string, otherwise, return -1
		if (pos != -1) {
			data = data.substr(pos + 1, data.length() - pos + 2);
		}
		return data;
	}
}