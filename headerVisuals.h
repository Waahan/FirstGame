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
#include <SDL2/SDL_mixer.h>

class App;
class Image;
class Messages;
class Audio;

class App
{
    friend Messages;

    public:
    explicit App(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    
    App(const App& copyFromApp) = delete;
    App& operator=(const App& copyFromApp) = delete;  
    
    App(App&& moveFromApp) = default;
    App& operator=(App&& moveFromApp) = default; 

    ~App();

    SDL_Texture *loadImages(const char* imageFile);
    
    App& imagePos(SDL_Texture* image, int x, int y, int w, int h);
    App& imagePos(SDL_Texture* image, int x, int y);
    App& imagePos(const Image& image, int x, int y, int w = 0, int h = 0);

    App& makeVisuals();
    App& showVisuals();

    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;

    private:
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    SDL_Surface* windowIcon = NULL;
};

class Image
{
    public:
    Image(std::string imagePath, App& app, int x, int y, int w, int h);
    
    Image(const Image& copyFromImage) = delete;
    Image& operator=(const Image& copyFromImage) = delete;

    Image(Image&& moveFromImage) = default;
    Image& operator=(Image&& moveFromImage) = default;

    ~Image() { SDL_DestroyTexture(imageTexture); }

    SDL_Texture* getImageTexture() const { return imageTexture; }
    const SDL_Rect& getCurrentImageSrc() const { return images[currentImageNum]; }
    SDL_Rect& getCurrentImageSrc() { return images[currentImageNum]; }

    SDL_Rect operator[](int index) const { return images[index]; }
    Image& operator++(int){ if(currentImageNum < images.size()-1)currentImageNum++; else currentImageNum = 0; return *this; }
    Image& operator+=(SDL_Rect&& addFrame) { images.push_back(addFrame); return *this; }

    bool done() const { return images.size()-1 == currentImageNum; }
    Image& reset() { currentImageNum = 0; return *this; }

    private:
    SDL_Texture* imageTexture = nullptr;
    std::vector<SDL_Rect> images;
    int currentImageNum = 0;
};

class Messages
{
    public:
    explicit Messages(const char* message, int x, int y, int w, int h, const App& app, const SDL_Color& color = {255, 255, 255} );
    
    Messages(const Messages& copyFromMessage) = delete;
    Messages& operator=(const Messages& copyFromMessage) = delete;

    Messages(Messages&& moveFromMessage) = default;
    Messages& operator=(Messages&& moveFromMessage) = default;
    
    ~Messages();

    Messages& newMessage(const char* message, int x, int y, int w, int h, const App& app, const SDL_Color& color = {255, 255, 255} );
    
    Messages& drawMessage(const App& app);

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

class audio
{
    public:
    audio(std::string path);

    audio(const audio& copyFromAudio) = delete;
    audio& operator=(const audio& copyFromAudio) = delete;

    audio(audio&& moveFromAudio) = default;
    audio& operator=(audio&& moveFromAudio) = default;

    ~audio(){ Mix_FreeMusic(currentMusic); }

    inline audio& play(int loops = 0);

    private:
    Mix_Music* currentMusic = nullptr;
};
