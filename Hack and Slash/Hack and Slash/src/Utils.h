#pragma once
#include <string>
#include <iostream>
#include <SDL.h>
using namespace std;

namespace utils {

	extern bool debugging;

	//clips off header. e.g "clip: 50 114 44 49" turns into "50 114 44 49"
	string clipOffDataHeader(string data);
	string getResourcePath(const string &subDir = "");
};