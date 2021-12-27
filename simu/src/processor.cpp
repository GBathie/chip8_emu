#include "processor.h"

#include <cassert>
#include <fstream>

Processor::Processor(): 
call_stack(), memory(MEMORY_SIZE_BYTES), 
display(SCREEN_W, SCREEN_H), keyboard()
{
}

void Processor::load(const string &filename) 
{
	ifstream rom(filename, ios::binary);
	std::vector<uint8_t> bytes(
         (std::istreambuf_iterator<char>(rom)),
         (std::istreambuf_iterator<char>()));

	int i = 0;
	for (auto b: bytes)
	{
		memory.write(MEMORY_OFFSET + i++, b);
	}

	pc = MEMORY_OFFSET;
}

string opcode_to_string(uint16_t opcode)
{
	switch (opcode)
	{
		case 0x0:
			return "0";
		case 0x1:
			return "Jump";
		case 0x2:
			return "Call";
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x9:
			return "Skip";
		case 0x6:
		case 0x7:
			return "Regedit";
		case 0x8:
			return "Arithmetic";
		case 0xA:
			return "Set Index";
		case 0xB:
			return "Offset Jump";
		case 0xC:
			return "Random";
		case 0xD:
			return "Display Sprite";
		case 0xE:
			return "Keypad test";
		case 0xF:
			return "Other";
		default:
			return "Unknown";
	}
}

void Processor::step()
{
	uint16_t instr = (memory.read(pc) << 8) | memory.read(pc+1);
	pc += 2;
	
	uint16_t opcode = (instr >> 12) & 0xf;
	
	uint8_t x = (instr >> 8) & 0xf;
	uint8_t y = (instr >> 4) & 0xf;
	uint8_t n = instr & 0xf;
	uint8_t nn = instr & 0xff;
	uint16_t nnn = instr & 0xfff;

	// cerr << "Executing instruction: 0x" << hex << instr << " (" << opcode_to_string(opcode) << ")" << endl;
	// cerr << "pc = " << hex << pc << endl;
	switch (opcode)
	{
		case 0x0:
			if (instr == 0x00E0)
				clear_screen();
			else if (instr == 0x00EE)
			{
				pc = call_stack.back();
				call_stack.pop_back();
			}
			break;
		case 0x1:
			pc = nnn;
			break;
		case 0x2:
			call_stack.push_back(pc);
			pc = nnn;
			break;
		// Skips
		case 0x3:
			if (reg[x] == nn)
				pc += 2;
			break;
		case 0x4:
			if (reg[x] != nn)
				pc += 2;
			break;
		case 0x5:
			if (reg[x] == reg[y])
				pc += 2;
			break;
		case 0x9:
			if (reg[x] != reg[y])
				pc += 2;
			break;
		// Register modification
		case 0x6:
			reg[x] = nn;
			break;
		case 0x7:
			reg[x] += nn;
			break;
		// Arithmetic
		case 0x8:
			arithmetic_operation(instr);
			break;
		// Set index
		case 0xA:
			index = nnn;
			break;
		// Offset jump
		case 0xB:
			pc = nnn + reg[0];
			break;
		// Random
		case 0xC:
			reg[x] = (rand() % 0xff) & nn;
			break;
		// Display sprite
		case 0xD:
			draw_sprite(x, y, n);
			break;
		// Check input
		case 0xE:
			key_check(instr, x);
			break;
		// Timers, etc
		case 0xF:
			handle_f(instr, x);
			break;
		default:
			cerr << "Unknown instruction: 0x" << hex << instr << endl;
	}
}

void Processor::clear_screen()
{
	display.clear();
	must_draw = true;
}

void Processor::arithmetic_operation(uint16_t instr)
{
	uint16_t opcode = (instr >> 12) & 0xf;
	assert(opcode == 0x8);

	uint8_t x = (instr >> 8) & 0xf;
	uint8_t y = (instr >> 4) & 0xf;
	opcode = instr & 0xf;

	switch (opcode)
	{
		case 0x0:
			reg[x] = reg[y];
			break;
		// Bitwise OP
		case 0x1:
			reg[x] = reg[x] | reg[y];
			break;
		case 0x2:
			reg[x] = reg[x] & reg[y];
			break;
		case 0x3:
			reg[x] = reg[x] ^ reg[y];
			break;
		// Adition
		case 0x4:
			reg[x] += reg[y];
			// Check for overflow
			reg[0xf] = (reg[x] < reg[y])? 1 : 0;
			break;
		// Substraction
		case 0x5:
			reg[x] = reg[x] - reg[y];
			// reversed overflow
			reg[0xf] = (reg[x] > reg[y])? 1 : 0;
			break;
		case 0x7:
			reg[x] = reg[y] - reg[x];
			// reversed overflow
			reg[0xf] = (reg[x] < reg[y])? 1 : 0;
			break;
		// Shifts
		case 0x6:
			// Right shift
			// reg[x] = reg[y]; //Old behavior
			reg[0xf] = reg[x] & 0x1;
			reg[x] = reg[x] >> 1;
			break;
			// Left shift
		case 0xE:
			// reg[x] = reg[y]; //Old behavior
			reg[0xf] = (reg[x] >> 7) & 0x1;
			reg[x] = reg[x] << 1;
			break;
		default:
			cerr << "Unknown instruction: 0x" << hex << instr << endl;
	}
}


void Processor::draw_sprite(uint8_t x, uint8_t y, uint8_t n)
{
	uint8_t cx = reg[x] % 64;
	uint8_t cy = reg[y] % 32;

	reg[0xf] = 0;
	// For each line of the sprite
	for (int i = 0; i < n; ++i)
	{
		if (cy + i >= SCREEN_H)
			break;
		// Load data,
		uint8_t data = memory.read(index + i);
		for (int j = 0; j < 8; ++j)
		{
			if (cx + j >= SCREEN_W)
				break;
			// Flip pixel if needed,
			uint8_t b = (data >> (7-j)) & 0x1;
			if (b && display.flip(cx + j, cy + i))
				reg[0xf] = 1;
			// display[cx + j][cy + i] = display[cx + j][cy + i] ^ b;
			// // If zeroed, set flag to 1
			// if (b && (display[cx + j][cy + i] == 0))
			// 	reg[0xf] = 1;
		}
	}
	must_draw = true;
}

void Processor::key_check(uint16_t instr, uint8_t x)
{
	uint8_t key = reg[x] & 0xf;
	bool is_pressed = keyboard.is_pressed(key);
	uint16_t low = instr & 0xff;
	if ((low == 0x9e && is_pressed)
		|| (low == 0xa1 && !is_pressed))
		pc += 2;

	if (low != 0x9e && low != 0xa1)
		throw runtime_error("Incorrect low value in key_check: " + to_string(low));
}

void Processor::handle_f(uint16_t instr, uint8_t x)
{
	uint16_t low = instr & 0xff;
	uint8_t n;
	int k;
	switch (low)
	{
		// Timers
		case 0x07:
			reg[x] = delay_timer.get();
			break;
		case 0x15:
			delay_timer.set(reg[x]);
			break;
		case 0x18:
			sound_timer.set(reg[x]);
			break;
		// Get key
		case 0x0A:
			k = keyboard.get_pressed();
			if (k < 0)
				pc -= 2;
			else
				reg[x] = k;
			break;
		// Add vx to index
		case 0x1e:
			index += reg[x];
			reg[0xf] = (index > 0xFFF) ? 1 : 0;
			break;
		// Font offset
		case 0x29:
			n = reg[x] & 0xF;
			index = FONT_OFFSET + 5*n;
			break;
		// Convert to decimal
		case 0x33:
			memory.write(index+2, reg[x] % 10);
			memory.write(index+1, (reg[x] / 10) % 10);
			memory.write(index, reg[x] / 100);
			break;
		// Write registers to memory
		case 0x55:
			for (int i = 0; i <= x; ++i)
				memory.write(index + i, reg[i]);
			break;
		// Read registers from memory
		case 0x65:
			for (int i = 0; i <= x; ++i)
				reg[i] = memory.read(index + i);
			break;
		default:
			cerr << "Unknown instruction in handle_f: 0x" << hex << instr << endl;
	}
}

void Processor::draw(SDL_Renderer *r)
{
	if (must_draw)
	{
		display.update_texture();
		must_draw = false;
	}
	display.draw(r);
}