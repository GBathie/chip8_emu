#pragma once

#include <vector>
#include <iostream>

#include <SDL.h>

using namespace std;

class Display
{
public:
	Display(int w, int h);
	~Display();

	void init(SDL_Renderer *r);
	void draw(SDL_Renderer *r);
	
	inline bool flip(int x, int y)
	{
		int t = x + y * width;
		cout << "accessing at " << t  << " : " << pixels[t] << endl;
		pixels[t] ^= 0xFFFFFF;
		cout << "after : " << pixels[t] << endl;
		return pixels[t] == 0;
	}
	void update_texture();
	void clear();
	
private:
	int width, height;
	SDL_Texture *content = nullptr;
	vector<uint32_t> pixels;
};