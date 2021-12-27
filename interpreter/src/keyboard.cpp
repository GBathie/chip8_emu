#include "keyboard.h"

#include <cassert>

using namespace std;

Keyboard::Keyboard(): key_pressed(16, false)
{

}

/* Keypad:
 * 1 2 3 C
 * 4 5 6 D
 * 7 8 9 E
 * A 0 B F
 *
 * Stored in memory as 1,2,3,4,...
 * This function takes a scancode from the left of 
 * the keyboard an returns the corresponding memory loc
 */
int scancode_to_index(SDL_Scancode c)
{
	switch (c)
	{
		// 1st row
		case SDL_SCANCODE_1:
			return 0x1;
		case SDL_SCANCODE_2:
			return 0x2;
		case SDL_SCANCODE_3:
			return 0x3;
		case SDL_SCANCODE_4:
			return 0xc;
		// 2nd row
		case SDL_SCANCODE_Q:
			return 0x4;
		case SDL_SCANCODE_W:
			return 0x5;
		case SDL_SCANCODE_E:
			return 0x6;
		case SDL_SCANCODE_R:
			return 0xd;
		// 3rd row
		case SDL_SCANCODE_A:
			return 0x7;
		case SDL_SCANCODE_S:
			return 0x8;
		case SDL_SCANCODE_D:
			return 0x9;
		case SDL_SCANCODE_F:
			return 0xe;
		// 4th row
		case SDL_SCANCODE_Z:
			return 0xa;
		case SDL_SCANCODE_X:
			return 0x0;
		case SDL_SCANCODE_C:
			return 0xb;
		case SDL_SCANCODE_V:
			return 0xf;
		default:
			return -1;
	}
}

void Keyboard::update(SDL_Event &e)
{
	assert(e.type == SDL_KEYUP || e.type == SDL_KEYDOWN);
	SDL_KeyboardEvent k = e.key;
	int i = scancode_to_index(k.keysym.scancode);
	if (i >= 0)
		key_pressed[i] = (k.type == SDL_KEYDOWN);
}

int Keyboard::get_pressed()
{

	for (int i = 0; i < key_pressed.size(); ++i)
		if (key_pressed[i])
			return i;

	return -1;
}