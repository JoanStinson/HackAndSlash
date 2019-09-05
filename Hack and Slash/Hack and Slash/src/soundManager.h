#pragma once
#include "SDL/SDL_mixer.h"
#include <string>
#include <map>
using namespace std;

class SoundManager {
public:
	map<string, Mix_Chunk*> sounds;
	SoundManager();
	~SoundManager();
	void loadSound(string name, string file);
	void playSound(string name);

	static SoundManager soundManager;
};