#include "Game.h"
#include <ctime> 

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 352;
constexpr int SCREEN_SCALE = 2;

int main(int argc, char *agv[]) {
	srand(static_cast<unsigned>(time(NULL)));
	Game game("Hack and Slash", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
	game.Update();
	SDL_Quit();
	return 0;
}