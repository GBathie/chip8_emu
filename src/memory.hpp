#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Memory
{
public:
	Memory(int size = 4096): data(size, 0)
	{

	}

	// Todo: check bounds
	inline uint8_t read(uint16_t loc) const 
	{ 
		return data[loc]; 
	} 

	inline void write(uint16_t loc, uint8_t val) 
	{
		data[loc] = val;
	}

private:
	vector<uint8_t> data; 
	
};