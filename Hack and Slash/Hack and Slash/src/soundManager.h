#pragma once
#include "SDL/SDL_mixer.h"
#include "SDL/SDL.h"
#include <string>
#include <map>
#include <iostream>
using namespace std;

//Abbreviation to the SoundManager singleton instance
#define SM SoundManager::Instance()

class SoundManager {
public:
	inline static SoundManager& Instance() {
		static SoundManager instance;
		return instance;
	}
	void LoadSound(string name, string file);
	void PlaySound(string name);

private:
	// private constructor so that no objects can be created (disallow instantiation outside of the class)
	SoundManager();
	~SoundManager();
	// delete copy constructor 
	SoundManager(const SoundManager&) = delete;
	// delete copy assignment operator
	SoundManager& operator=(const SoundManager&) = delete;
	// delete move constructor
	SoundManager(SoundManager &&) = delete;
	// delete move assignment operator
	SoundManager& operator=(SoundManager &&) = delete;

	map<string, Mix_Chunk*> sounds;
};