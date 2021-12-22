#include "display.h"

#include <cstring>

Display::Display(int w, int h): width(w), height(h), pixels(w * h)
{

}

Display::~Display()
{
	SDL_DestroyTexture(content);
}

void Display::init(SDL_Renderer *r)
{
	content = SDL_CreateTexture(r, 
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 
		width, height);
	update_texture();
	draw(r);
}

void Display::update_texture()
{
	uint8_t* texture_pixels;
	int pitch;
	SDL_LockTexture(content, nullptr, (void**)&texture_pixels, &pitch);
	std::memcpy(texture_pixels, (uint8_t*)pixels.data(), 4 * pixels.size());
	SDL_UnlockTexture(content);
	// SDL_UpdateTexture(content, nullptr, (void*)pixels.data(), 4 * SCREEN_WIDTH);
}

void Display::draw(SDL_Renderer *r)
{
	SDL_RenderCopy(r, content, nullptr, nullptr);
}

void Display::clear() 
{
	fill(pixels.begin(), pixels.end(), 0);
	update_texture();
}
