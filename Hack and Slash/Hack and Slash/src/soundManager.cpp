#include "SoundManager.h"

SoundManager::SoundManager() {
	//inialise sdl mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		SDL_Quit();
		cout << "mixer did not initalise" << endl;
	}
}

SoundManager::~SoundManager() {
	for (auto it = sounds.begin(); it != sounds.end(); it++)
		Mix_FreeChunk(it->second);
}

void SoundManager::LoadSound(const string &name, const string &file) {
	sounds[name] = Mix_LoadWAV(file.c_str());
}

void SoundManager::PlaySound(const string &name) {
	Mix_PlayChannel(-1, sounds[name], false);
}
