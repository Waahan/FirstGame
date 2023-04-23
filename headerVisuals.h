//Header for visuals.cpp
#pragma once
#include <string.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

class App
{
    public:
    App(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    
    void initFont(char* message);
    void newMessage(const char* message);
    void drawText();

    SDL_Texture *loadImages(char* imageFile);

    void imagePos(SDL_Texture* image, int x, int y, int w, int  h);

    void makeVisuals();
    void showVisuals();

    void quit();

    protected:
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    //For the text
    TTF_Font* font = NULL;
    SDL_Color White = {255, 255, 255};
    SDL_Surface* surfaceMessage = NULL;
    SDL_Texture* Message = NULL;
    SDL_Rect Message_rect;
};
