#pragma once
#include <string>
#include <iostream>
#include <SDL.h>
using namespace std;

namespace utils {

	extern bool debugging;
	string clipOffDataHeader(string data);							//!< Clips off header. e.g "clip: 50 114 44 49" turns into "50 114 44 49"
	string getResourcePath(const string &subDir = "");
};