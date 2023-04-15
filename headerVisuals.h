//Header for visuals.cpp
#pragma once
#ifndef headerVisuals_H
#define headerVisuals_H

class App
{
    public:
    App(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    SDL_Texture *loadImages(char* imageFile);
    void imagePos(SDL_Texture* image, int x, int y, int w, int  h);
    void makeVisuals();
    void showVisuals();
    protected:
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
};
#endif
