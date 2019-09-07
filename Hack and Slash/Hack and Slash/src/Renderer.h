#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
using namespace std;

#define RENDERER Renderer::Instance()

class Renderer {
public:
	inline static Renderer& Instance() {
		static Renderer instance;
		return instance;
	}

	/**
	* Loads an image into a texture on the rendering device
	* @param file The image file to load
	* @param ren The renderer to load the texture onto
	* @return the loaded texture, or nullptr if something went wrong.
	*/
	SDL_Texture* LoadTexture(const string &file);

	/**
	*Loads an image up as a surface. Generally we want to do this if we want to palette swap
	*/
	SDL_Surface* LoadSurface(const string &file);
	/**
	*Copies the palette of 1 surface to another
	*/
	void SurfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette);

	/**
	*Converts a surface to a texture and optionally deletes the surface
	*/
	SDL_Texture *ConvertSurfaceToTexture(SDL_Surface* surface, bool cleanSurface = false);

	/**
	* Draw an SDL_Texture to an SDL_Renderer at some destination rect
	* taking a clip of the texture if desired
	* @param tex The source texture we want to draw
	* @param ren The renderer we want to draw to
	* @param dst The destination rectangle to render the texture to
	* @param clip The sub-section of the texture to draw (clipping rect)
	*		default of nullptr draws the entire texture
	*/
	void RenderTexture(SDL_Texture *tex, SDL_Rect dst, SDL_Rect *clip = nullptr);
	/**
	* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
	* the texture's width and height and taking a clip of the texture if desired
	* If a clip is passed, the clip's width and height will be used instead of
	*	the texture's
	* @param tex The source texture we want to draw
	* @param ren The renderer we want to draw to
	* @param x The x coordinate to draw to
	* @param y The y coordinate to draw to
	* @param clip The sub-section of the texture to draw (clipping rect)
	*		default of nullptr draws the entire texture
	*/
	void RenderTexture(SDL_Texture *tex, int x, int y, SDL_Rect *clip = nullptr);


	/**
	* Render the message we want to display to a texture for drawing
	* @param message The message we want to display
	* @param fontFile The font we want to use to render the text
	* @param color The color we want the text to be
	* @param fontSize The size we want the font to be
	* @param renderer The renderer to load the texture in
	* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
	*/
	SDL_Texture* RenderText(const string &message, const string &fontFile, SDL_Color color, int fontSize);

	/**
	* Render the message we want to display to a texture for drawing
	* @param message The message we want to display
	* @param font The font we want to use to render the text
	* @param color The color we want the text to be
	* @param fontSize The size we want the font to be
	* @param renderer The renderer to load the texture in
	* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
	*/
	SDL_Texture* RenderText(const string &message, TTF_Font*font, SDL_Color color);

	bool SaveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow);

	/*!
	* @brief Get the SDL renderer through the () operator.
	* @return The SDL Window pointer @m_SDLRenderer.
	*/
	inline SDL_Renderer* operator()() const {
		return renderer;
	}

private:
	// private constructor so that no objects can be created (disallow instantiation outside of the class)
	Renderer();
	~Renderer();
	// delete copy constructor 
	Renderer(const Renderer&) = delete;
	// delete copy assignment operator
	Renderer& operator=(const Renderer&) = delete;
	// delete move constructor
	Renderer(Renderer &&) = delete;
	// delete move assignment operator
	Renderer& operator=(Renderer &&) = delete;

	SDL_Renderer *renderer = nullptr;
};