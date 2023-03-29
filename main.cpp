#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
using namespace std;

//Screen size
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//needed for window
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//create window
void initSDL()
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;
    
    //Check if it is working
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Could not start SDL:" << SDL_GetError();
    }

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    //Check if window was created
    if (!window)
    {
        cout << "Failed to open window: " << SDL_GetError();
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    //Check if renderer is working
    if(!renderer)
    {
        cout << "Renderer failed: " << SDL_GetError();
    }
}

void makeVisuals()
{
    //Color for background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void showVisuals()
{
    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[])
{
    initSDL();

    while (1)
    {
        makeVisuals();
	input();
	showVisuals();
	SDL_Delay(10);
    }
    return 0;
}
