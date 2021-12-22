#include "app.h"

#include <stdexcept>
#include <iostream>

App::App(const string &s)
{
    cout << "SDL NOT initialized" << endl;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw runtime_error("Could not initialize SDL in App()!");
    cout << "SDL initialized" << endl;
    window = SDL_CreateWindow("Chip8 eµlator", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr)
        throw runtime_error("Could not create window in App()!");

    renderer = SDL_CreateRenderer(window, -1, 0);
    p.init_display(renderer);
    p.load(s);
    cout << "App initialized" << endl;
}

void App::run()
{
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
            if(e.type == SDL_QUIT)
                quit = true;

        p.step();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        p.draw(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}

App::~App()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}