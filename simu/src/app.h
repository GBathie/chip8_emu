#pragma once

#include <SDL.h>

#include "processor.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class App
{
public:
	App(const string &s);
	~App();

	void run();

private:
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;

	Processor p;
};