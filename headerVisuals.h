//Header for visuals.cpp
#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <climits>
#include <future>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class App;
class Messages;

class App
{
    friend Messages;

    public:
    explicit App(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    App(App& copyApp) = delete;
    ~App();

    SDL_Texture *loadImages(const char* imageFile);
    
    void imagePos(SDL_Texture* image, int x, int y, int w, int h);
    void imagePos(SDL_Texture* image, int x, int y);

    void makeVisuals();
    void showVisuals();

    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;

    protected:
    SDL_Renderer* renderer = NULL;

    private:
    SDL_Window* window = NULL;
    SDL_Surface* windowIcon = NULL;
};

class Messages
{
    public:
    explicit Messages(const char* message, int x, int y, int w, int h, const App& app, const SDL_Color& color = {255, 255, 255} );
    Messages(Messages& copyMessage) = delete;
    ~Messages();

    void newMessage(const char* message, int x, int y, int w, int h, const App& app, const SDL_Color& color = {255, 255, 255} );
    
    void drawMessage(const App& app);

    const SDL_Color White = {255, 255, 255};
    const SDL_Color Red = {255, 0, 0};
    const SDL_Color Orange = {255, 165, 0};
    const SDL_Color Yellow = {255, 255, 0};
    const SDL_Color Green = {0, 128, 0};
    const SDL_Color Blue = {0, 0, 255};
    const SDL_Color Indigo = {75, 0, 130};
    const SDL_Color Violet = {238, 130, 238};

    private:
    TTF_Font* font = NULL;
    
    SDL_Surface* surfaceMessage = NULL;
    SDL_Texture* Message = NULL;

    SDL_Rect Message_rect;
};
