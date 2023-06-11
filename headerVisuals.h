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
    App(const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT);
    App(App& copyApp) = delete;
    ~App();

    SDL_Texture *loadImages(const char* imageFile);
    void imagePos(SDL_Texture* image, int x, int y, int w, int h);
    void imagePos(SDL_Texture* image, int x, int y);
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
    Messages(const char* message, int x, int y, int w, int h, App& app);
    Messages(const char* message, int x, int y, int w, int h, const SDL_Color& color, App& app);
    ~Messages();

    void newMessage(const char* message, int x, int y, int w, int h, App& app);
    void newMessage(const char* message, int x, int y, int w, int h, const SDL_Color& color, App& app);
    void drawMessage(App& app);

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
