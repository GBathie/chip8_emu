#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "memory.hpp"
#include "display.h"

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

	inline void init_display(SDL_Renderer *r) { display.init(r); }

private:
	uint16_t pc, index;
	uint8_t reg[N_REGISTERS] = {};
	vector<uint16_t> call_stack;
	Memory memory;
	Display display;
	// uint8_t display[SCREEN_W][SCREEN_H];
	bool must_draw;
	// Timer delay_timer;
	// SoundTimer sound_timer;

	void clear_screen();
	void arithmetic_operation(uint16_t instr);
	void draw_sprite(uint8_t x, uint8_t y, uint8_t n);

};