//Header for visuals.cpp
#pragma once
#include <string.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

class App;
class Messages;

class App
{
    friend Messages;

    public:
    App(int SCREEN_WIDTH, int SCREEN_HEIGHT);

    SDL_Texture *loadImages(const char* imageFile);
    void imagePos(SDL_Texture* image, int x, int y, int w, int h);
    void makeVisuals();
    void showVisuals();

    protected:
    SDL_Renderer* renderer = NULL;

    private:
    SDL_Window* window = NULL;
};

class Messages
{
    public:
    Messages(const char* message, int x, int y, int w, int h, App app);
    Messages(const char* message, int x, int y, int w, int h, SDL_Color color, App app);
    ~Messages();

    void newMessage(const char* message, int x, int y, int w, int h, App app);
    void newMessage(const char* message, int x, int y, int w, int h, SDL_Color color, App app);
    void drawMessage(App app);

    private:
    TTF_Font* font = NULL;
    
    public:
    SDL_Color White = {255, 255, 255};
    SDL_Color Red = {255, 0, 0};
    SDL_Color Orange = {255, 165, 0};
    SDL_Color Yellow = {255, 255, 0};
    SDL_Color Green = {0, 128, 0};
    SDL_Color Blue = {0, 0, 255};
    SDL_Color Indigo = {75, 0, 130};
    SDL_Color Violet = {238, 130, 238};

    private:
    SDL_Surface* surfaceMessage = NULL;
    SDL_Texture* Message = NULL;

    SDL_Rect Message_rect;
};
