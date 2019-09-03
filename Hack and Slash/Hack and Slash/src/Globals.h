#pragma once
#include <string>
using namespace std;

namespace globals {

	// Math helpers
	const float PI = 3.14f;

	// clips off header. e.g "clip: 50 114 44 49" turns into "50 114 44 49"
	string clipOffDataHeader(string data);
};