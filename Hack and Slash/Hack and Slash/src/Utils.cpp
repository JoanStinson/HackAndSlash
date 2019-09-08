#include "Utils.h"

namespace utils {

	bool debugging = false;

	string clipOffDataHeader(string data) {
		int pos = data.find(":", 0); // Returns where we find the : in the string, otherwise, return -1
		if (pos != -1) {
			data = data.substr(pos + 1, data.length() - pos + 2);
		}
		return data;
	}

	string getResourcePath(const string &subDir) {
		// We need to choose the path separator properly based on which
		// platform we're running on, since Windows uses a different
		// separator than most systems
#ifdef _WIN32
		const char PATH_SEP = '\\';
#else
		const char PATH_SEP = '/';
#endif
		// This will hold the base resource path: Lessons/res/
		// We give it static lifetime so that we'll only need to call
		// SDL_GetBasePath once to get the executable path
		static std::string baseRes;
		if (baseRes.empty()) {
			// SDL_GetBasePath will return NULL if something went wrong in retrieving the path
			char *basePath = SDL_GetBasePath();
			if (basePath) {
				baseRes = basePath;
				SDL_free(basePath);
			}
			else {
				std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
				return "";
			}
			// We replace the last bin/ with res/ to get the the resource path
			size_t pos = baseRes.rfind("bin");
			baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
		}
		// If we want a specific subdirectory path in the resource directory
		// append it to the base path. This would be something like Lessons/res/Lesson0
		return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
	}
}