#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "headerPlayer.h"
using namespace std;

App::App(int SCREEN_WIDTH, int SCREEN_HEIGHT)
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

    //-1 so SDL use the first graphics acceleration device it finds
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    //Check if renderer is working
    if(!renderer)
    {
        cout << "Renderer failed: " << SDL_GetError();
    }

    //Allows window to load .png and .jpg images
    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if ((initted & flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
}


//For loading image to the window
SDL_Texture* App::loadImages(char* imageFile)
{
    //A variable to pass the image to SDL
    SDL_Texture *Image;

    Image = IMG_LoadTexture(renderer, imageFile);

    return Image;
}

//sets image location
void App::imagePos(SDL_Texture* image, int x, int y, int w, int h)
{
    SDL_Rect dest;

    //Where the image goes
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    //Query the attributes of a texture
    //Takes image, Format(just set to NULL), Access(Also set to NULL), width and height
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image
    SDL_RenderCopy(renderer, image, NULL, &dest);
}

void App::makeVisuals()
{
    //Color for background         0, 0, 0, 0, is for black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void App::showVisuals()
{
    SDL_RenderPresent(renderer);
}

