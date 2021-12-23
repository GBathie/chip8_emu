#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "memory.hpp"
#include "keyboard.h"
#include "display.h"
#include "timer.hpp"

using namespace std;

const int N_REGISTERS = 16;
const int MEMORY_SIZE_BYTES = 4096;
const int MEMORY_OFFSET = 0x200;
const int SCREEN_W = 64;
const int SCREEN_H = 32;

class Processor
{
public:
	Processor();

	void load(const string &filename);
	void step();
	void print();
	void draw(SDL_Renderer *r);

	inline void key_update(SDL_Event &e) { keyboard.update(e); }
	inline void init_display(SDL_Renderer *r) { display.init(r); }

private:
	uint16_t pc, index;
	uint8_t reg[N_REGISTERS] = {};
	vector<uint16_t> call_stack;
	Memory memory;
	Display display;
	Keyboard keyboard;
	Timer delay_timer;
	SoundTimer sound_timer;
	bool must_draw;

	void clear_screen();
	void arithmetic_operation(uint16_t instr);
	void draw_sprite(uint8_t x, uint8_t y, uint8_t n);
	void key_check(uint16_t instr, uint8_t x);
	void handle_f(uint16_t instr, uint8_t x);

};