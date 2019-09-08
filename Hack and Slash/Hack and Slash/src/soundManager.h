#pragma once
#include "SDL/SDL_mixer.h"
#include "SDL/SDL.h"
#include <string>
#include <map>
#include <iostream>
using namespace std;

#define SM SoundManager::Instance()

class SoundManager {
public:
	inline static SoundManager& Instance() {
		static SoundManager instance;
		return instance;
	}
	void LoadSound(const string &name, const string &file);
	void PlaySound(const string &name);

private:
	SoundManager();														//!< Private constructor so that no objects can be created (disallow instantiation outside of the class)
	~SoundManager();
	SoundManager(const SoundManager&) = delete; 						//!< Delete copy constructor 
	SoundManager& operator=(const SoundManager&) = delete; 				//!< Delete copy assignment operator
	SoundManager(SoundManager &&) = delete; 							//!< Delete move constructor
	SoundManager& operator=(SoundManager &&) = delete; 					//!< Delete move assignment operator

	map<string, Mix_Chunk*> sounds;										/*!< Stores all the in-game sounds, very easy to filter using the song's name (aka string) */
};