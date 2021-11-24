#include <iostream>

#include "processor.h"

using namespace std;

int main(int argc, char const *argv[])
{
	Processor p;
	p.load("IBM Logo.ch8");
	// p.load("maze.ch8");
	while (true)
	{
		p.print();
		p.step();
		getchar();
	}
	return 0;
}