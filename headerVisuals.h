//Header for visuals.cpp
#pragma once
#include <string.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

class Messages;

class App
{
    friend Messages;

    public:
    App(int SCREEN_WIDTH, int SCREEN_HEIGHT);

    SDL_Texture *loadImages(const char* imageFile);
    void imagePos(SDL_Texture* image, int x, int y, int w, int  h);
    void makeVisuals();
    void showVisuals();

    protected:
    SDL_Renderer* renderer = NULL;

    private:
    SDL_Window* window = NULL;
    SDL_Texture* textureList[100];
    int textureNumber = 0;
};

class Messages
{
    public:
    Messages(const char* message, int x, int y, int w, int h, App app);
    ~Messages();

    void newMessage(const char* message, int x, int y, int w, int h, App app);
    void drawMessage(App app);

    private:
    TTF_Font* font = NULL;
    SDL_Color White = {255, 255, 255};
    SDL_Surface* surfaceMessage = NULL;
    SDL_Texture* Message = NULL;
    SDL_Rect Message_rect;
};
