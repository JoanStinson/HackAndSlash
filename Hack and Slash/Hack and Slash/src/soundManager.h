#pragma once
#include "SDL/SDL_mixer.h"
#include <string>
#include <map>
using namespace std;

class SoundManager {
public:
	SoundManager();
	~SoundManager();
	void LoadSound(string name, string file);
	void PlaySound(string name);

	map<string, Mix_Chunk*> sounds;
	static SoundManager soundManager;
};