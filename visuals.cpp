#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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

    if(TTF_Init() < 0)
    {
        cout << "Could not start SDL ttf:" << SDL_GetError();
    }

    window = SDL_CreateWindow("Amongus 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

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

void App::initFont(char* message)
{
    // Font path /usr/share/fonts/chromeos/google-sans/static/GoogleSans-BoldItalic-v3.003.ttf
    //this opens a font style and sets a size
    font = TTF_OpenFont("/usr/share/fonts/chromeos/google-sans/static/GoogleSans-Bold-v3.003.ttf", 24);

    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    surfaceMessage = TTF_RenderText_Solid(font, message, White); 

    // now you can convert it into a texture
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    Message_rect.x = 0;  //controls the rect's x coordinate 
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 50; // controls the width of the rect
    Message_rect.h = 50; // controls the height of the rect
}

void App::newMessage(const char* message)
{
    surfaceMessage = TTF_RenderText_Solid(font, message, White);
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
}

void App::drawText()
{
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
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

void App::quit()
{
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}
