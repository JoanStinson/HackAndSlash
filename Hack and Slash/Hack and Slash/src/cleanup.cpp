#include "cleanup.h"



void cleanup(SDL_Window *win){
	if (!win){
		return;
	}
	SDL_DestroyWindow(win);
}
void cleanup(SDL_Renderer *ren){
	if (!ren){
		return;
	}
	SDL_DestroyRenderer(ren);
}
void cleanup(SDL_Texture *tex){
	if (!tex){
		return;
	}
	SDL_DestroyTexture(tex);
}
void cleanup(SDL_Surface *surf){
	if (!surf){
		return;
	}
	SDL_FreeSurface(surf);
}
