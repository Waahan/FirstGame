#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "headerPlayer.h"

App::App(const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT)
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Could not start SDL:" << SDL_GetError() << std::endl;
    }

    if(TTF_Init() < 0)
    {
        std::cerr << "Could not start SDL ttf:" << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow("Amongus 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    //Check if window was created
    if (!window)
    {
        std::cerr << "Failed to open window: " << SDL_GetError() << std::endl;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    //-1 so SDL use the first graphics acceleration device it finds
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    //Check if renderer is working
    if(!renderer)
    {
        std::cerr << "Renderer failed: " << SDL_GetError() << std::endl;
    }

    //Allows window to load .png and .jpg images
    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if ((initted & flags) != flags) {
	std::cerr << "IMG_Init: Failed to init required jpg and png support!" << std::endl;
	std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
    }
}

App::~App()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

//For loading image to the window
SDL_Texture* App::loadImages(const char* imageFile)
{
    //A variable to pass the image to SDL
    SDL_Texture *Image;

    Image = IMG_LoadTexture(renderer, imageFile);

    if(Image == NULL)
    {
        std::cerr << "IMG_LoadTexture failed: " << SDL_GetError() << std::endl; 
    }

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

void App::imagePos(SDL_Texture* image, int x, int y)
{
    SDL_Rect dest;

    //Where the image goes
    dest.x = x;
    dest.y = y;

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



Messages::Messages(const char* message, int x, int y, int w, int h, App& app)
{
    //this opens a font style and sets a size
    font = TTF_OpenFont("images/GoogleSans-Bold-v3.003.ttf", 24);

    if(font == NULL)
    {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
    }

    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    surfaceMessage = TTF_RenderText_Solid(font, message, White);

    if(surfaceMessage == NULL)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
    }

    // now you can convert it into a texture
    Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);

    if(Message == NULL)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    Message_rect.x = x;  //controls the rect's x coordinate
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = w; // controls the width of the rect
    Message_rect.h = h; // controls the height of the rect
}

Messages::Messages(const char* message, int x, int y, int w, int h, SDL_Color color, App& app)
{
    //this opens a font style and sets a size
    font = TTF_OpenFont("images/GoogleSans-Bold-v3.003.ttf", 24);

    if(font == NULL)
    {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
    }

    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    surfaceMessage = TTF_RenderText_Solid(font, message, color);

    if(surfaceMessage == NULL)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
    }

    // now you can convert it into a texture
    Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);

    if(Message == NULL)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    Message_rect.x = x;  //controls the rect's x coordinate
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = w; // controls the width of the rect
    Message_rect.h = h; // controls the height of the rect
}

Messages::~Messages()
{
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void Messages::newMessage(const char* message, int x, int y, int w, int h, App& app)
{
    SDL_FreeSurface(surfaceMessage);

    surfaceMessage = TTF_RenderText_Solid(font, message, White);

    if(surfaceMessage == NULL)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
    }

    SDL_DestroyTexture(Message);

    Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);

    if(Message == NULL)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;
}

void Messages::newMessage(const char* message, int x, int y, int w, int h, SDL_Color color, App& app)
{
    SDL_FreeSurface(surfaceMessage);

    surfaceMessage = TTF_RenderText_Solid(font, message, color);

    if(surfaceMessage == NULL)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
    }

    SDL_DestroyTexture(Message);

    Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);

    if(Message == NULL)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;
}

void Messages::drawMessage(App& app)
{
    SDL_RenderCopy(app.renderer, Message, NULL, &Message_rect);
}

