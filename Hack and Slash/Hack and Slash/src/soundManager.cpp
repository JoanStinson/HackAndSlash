#include "SoundManager.h"

SoundManager SoundManager::soundManager;

SoundManager::SoundManager() {
}

SoundManager::~SoundManager() {
	for (auto it = sounds.begin(); it != sounds.end(); it++) {
		Mix_FreeChunk(it->second);
	}
}

void SoundManager::LoadSound(string name, string file) {
	sounds[name] = Mix_LoadWAV(file.c_str());
}

void SoundManager::PlaySound(string name) {
	Mix_PlayChannel(-1, sounds[name], false);
}
