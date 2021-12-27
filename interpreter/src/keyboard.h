#pragma once

#include <vector>

#include <SDL.h>

typedef int Key;

class Keyboard
{
public:
	Keyboard();

	inline bool is_pressed(Key k) { return key_pressed[k]; }

	void update(SDL_Event &e);
	int get_pressed();

private:
	std::vector<bool> key_pressed;
	
};