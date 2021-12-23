#include "app.h"

#include <stdexcept>
#include <iostream>

App::App(const string &s)
{
    cout << "SDL NOT initialized" << endl;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        throw runtime_error("Could not initialize SDL in App()!");
    cout << "SDL initialized" << endl;
    
    window = SDL_CreateWindow("Chip8 eµlator", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(window == nullptr)
        throw runtime_error("Could not create window in App()!");

    int flags = MIX_INIT_OGG | MIX_INIT_MP3;
    if ((Mix_Init(flags) & flags) != flags)
        throw runtime_error("Could not initialize SDL_Mixer");

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
        throw runtime_error("Could not open audio");

    renderer = SDL_CreateRenderer(window, -1, 0);
    p.init_display(renderer);
    p.load(s);
    cout << "App initialized" << endl;
}

void App::run()
{
    bool quit = false;
    SDL_Event e;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    while (!quit)
    {

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                quit = true;
            if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
                p.key_update(e);
        }

        p.step();
        SDL_RenderClear(renderer);
        p.draw(renderer);
        SDL_RenderPresent(renderer);
        // SDL_Delay(10);
    }
}

App::~App()
{
    Mix_Quit();
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}