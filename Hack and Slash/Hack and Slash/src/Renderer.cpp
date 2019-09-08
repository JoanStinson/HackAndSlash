#include "Renderer.h"
#include "Window.h"

Renderer::Renderer() {
	// Setup renderer
	renderer = SDL_CreateRenderer(WINDOW(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(WINDOW());
		SDL_Quit();
		cout << "renderer error" << endl;
	}
	// This is the size to draw things at, before we scale it to the screen size dimensions mentioned in createWindow
	SDL_RenderSetLogicalSize(renderer, WINDOW.SCREEN_WIDTH, WINDOW.SCREEN_HEIGHT);

	// Initialise sdl_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_Quit();
		cout << "sdl image did not initialise" << endl;
	}

	// Initialise text to font 
	if (TTF_Init() != 0) {
		SDL_Quit();
		cout << "sdl ttf did not initialise" << endl;
	}
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* Renderer::LoadTexture(const string &file) {
	SDL_Texture *texture = IMG_LoadTexture(renderer, file.c_str());
	if (texture == nullptr) {
		cout << "LoadTexture error" << endl;
	}
	return texture;
}

/**
*Loads an image up as a surface. Generally we want to do this if we want to palette swap
*/
SDL_Surface* Renderer::LoadSurface(const string &file) {
	SDL_Surface *surface = IMG_Load(file.c_str());
	if (surface == nullptr) {
		cout << "LoadSurface error" << endl;
	}
	return surface;
}
/**
*Copies the palette of 1 surface to another
*/
void Renderer::SurfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette) {
	SDL_SetPaletteColors(surface->format->palette, palette->format->palette->colors, 0, palette->format->palette->ncolors);
}

/**
*Converts a surface to a texture and optionally deletes the surface
*/
SDL_Texture* Renderer::ConvertSurfaceToTexture(SDL_Surface* surface, bool cleanSurface) {
	SDL_Texture*texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (cleanSurface)
		SDL_FreeSurface(surface);
	return texture;
}
/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void Renderer::RenderTexture(SDL_Texture *tex, SDL_Rect dst, SDL_Rect *clip) {
	SDL_RenderCopy(renderer, tex, clip, &dst);
}
/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height and taking a clip of the texture if desired
* If a clip is passed, the clip's width and height will be used instead of
*	the texture's
* @param tex The source texture we want to draw
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void Renderer::RenderTexture(SDL_Texture *tex, int x, int y, SDL_Rect *clip) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr) {
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
	}
	RenderTexture(tex, dst, clip);
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* Renderer::RenderText(const string &message, const string &fontFile, SDL_Color color, int fontSize) {
	// Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr) {
		cout << "TTF_OpenFont error" << endl;
		return nullptr;
	}
	// We need to first render to a surface as that's what TTF_RenderText
	// returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		cout << "TTF_RenderText error" << endl;
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		cout << "CreateTexture" << endl;
	}
	// Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* Renderer::RenderText(const string &message, TTF_Font *font, SDL_Color color) {
	if (font == nullptr) {
		cout << "TTF_OpenFont" << endl;
		return nullptr;
	}
	// We need to first render to a surface as that's what TTF_RenderText
	// returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		cout << "TTF_RenderText" << endl;
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		cout << "CreateTexture" << endl;
	}
	// Clean up the surface and font
	SDL_FreeSurface(surf);
	//	TTF_CloseFont(font);
	return texture;
}

bool Renderer::SaveScreenshotBMP(string filepath, SDL_Window* SDLWindow) {
	SDL_Surface* saveSurface = nullptr;
	SDL_Surface* infoSurface = nullptr;
	infoSurface = SDL_GetWindowSurface(SDLWindow);
	if (infoSurface == nullptr) {
		std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
	}
	else {
		unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
		if (pixels == 0) {
			std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
			return false;
		}
		else {
			if (SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
				std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
				pixels = nullptr;
				return false;
			}
			else {
				saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
				if (saveSurface == nullptr) {
					std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
					return false;
				}
				SDL_SaveBMP(saveSurface, filepath.c_str());
				SDL_FreeSurface(saveSurface);
				saveSurface = nullptr;
			}
			delete[] pixels;
		}
		SDL_FreeSurface(infoSurface);
		infoSurface = nullptr;
	}
	return true;
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(renderer), renderer = nullptr;
	IMG_Quit();
}
