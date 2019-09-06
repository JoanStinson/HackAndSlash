#include "DrawingFunctions.h"
#include "SDL/SDL_mixer.h"
#include "globals.h"
#include "game.h"
#include <cstdlib> //srand, rand
#include <ctime> //time

int main(int argc, char *agv[]){
	//set the random seed for the random number sequence
	srand(static_cast<unsigned>(time(NULL)));

	//setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "error initialising SDL" << endl;
		return 1;
	}
	//setup window
	SDL_Window *window = SDL_CreateWindow("Hack and Slash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		globals::ScreenWidth*globals::ScreenScale, globals::ScreenHeight*globals::ScreenScale
		, SDL_WINDOW_SHOWN); //SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	if (window == nullptr){
		SDL_Quit(); 
		cout << "window error" << endl;
		return 1;
	}

	//setup renderer
	globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (globals::renderer == nullptr){
		SDL_DestroyWindow(window);
		SDL_Quit();
		cout << "renderer error" << endl;
		return 1;
	}
	//this is the size to draw things at, before we scale it to the screen size dimensions mentioned in createWindow
	SDL_RenderSetLogicalSize(globals::renderer, globals::ScreenWidth, globals::ScreenHeight);

	//initialise sdl_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		SDL_Quit();
		cout << "sdl image did not initialise" << endl;
		return 1;
	}
	//initialise text to font 
	if (TTF_Init() != 0){
		SDL_Quit();
		cout << "sdl ttf did not initialise" << endl;
		return 1;
	}
	//inialise sdl mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_Quit();
		cout << "mixer did not initalise" << endl;
		return 1;
	}
	Game game;
	game.Update();

	SDL_DestroyRenderer(globals::renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;


}