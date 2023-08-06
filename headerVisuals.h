//Header for visuals.cpp
#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <algorithm>
#include <chrono>
#include <random>

#include <cassert>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class App;
class Image;
class Messages;
class Audio;
class randomGen;

template<typename T, void (*destroyFunc)(T*)>
struct SDL_Delete
{
    void operator()(T* SDL_pointer){ destroyFunc(SDL_pointer); }
};

template<typename T, void (*destroyFunc)(T*)>
using SDL_Pointer = std::unique_ptr<T, SDL_Delete<T, destroyFunc>>;

class App
{
    public:
    explicit App(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    
    App(const App& copyFromApp) = delete;
    App& operator=(const App& copyFromApp) = delete;  
    
    App(App&& moveFromApp);
    App& operator=(App&& moveFromApp); 

    ~App();

    SDL_Texture *loadImages(std::string_view imageFile);
    SDL_Texture* createTextureFromSurface(SDL_Surface* currentSurface);

    App& imagePos(SDL_Texture* image, int x, int y, int w = 0, int h = 0);
    App& imagePos(const Image& image, int x, int y, int w = 0, int h = 0);
    App& imagePos(const Messages& message);

    App& makeVisuals();
    void showVisuals() const;

    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;

    private:
    SDL_Pointer<SDL_Renderer, SDL_DestroyRenderer> renderer;
    SDL_Pointer<SDL_Window, SDL_DestroyWindow> window;
    SDL_Pointer<SDL_Surface, SDL_FreeSurface> windowIcon;
};

class Image
{
    public:
    explicit Image(std::string_view imagePath, App& app, int x, int y, int w, int h);
    Image() : imageTexture{}, images{} {}
    
    Image(const Image& copyFromImage) = delete;
    Image& operator=(const Image& copyFromImage) = delete;

    Image(Image&& moveFromImage);
    Image& operator=(Image&& moveFromImage);
    
    ~Image() = default;

    SDL_Texture* getImageTexture() const;
    const SDL_Rect& getCurrentImageSrc() const { return images[currentImageNum]; }
    SDL_Rect& getCurrentImageSrc() { return images[currentImageNum]; }

    SDL_Rect operator[](int index) const { return images[index]; }
    Image& operator++(int);
    inline Image& operator+=(SDL_Rect&& addFrame);
    Image& operator+=(std::initializer_list<int> addFrames);

    bool done() const { return int(images.size()-1) == currentImageNum; }
    inline Image& reset();

    private:
    SDL_Pointer<SDL_Texture, SDL_DestroyTexture> imageTexture;
    std::vector<SDL_Rect> images;    
    int currentImageNum = 0;
};

enum class color { red, orange, yellow, green, blue, indigo, violet, none};

class Messages
{
    public:
    explicit Messages(std::string message, int x, int y, int w, int h, App* app, color newColor = color::none);
    
    Messages(const Messages& copyFromMessage) = delete;
    Messages& operator=(const Messages& copyFromMessage) = delete;

    Messages(Messages&& moveFromMessage);
    Messages& operator=(Messages&& moveFromMessage);

    ~Messages() = default;

    Messages& newMessage(std::string message, int x, int y, int, int, color newColor);
    
    SDL_Texture* getTexture() const{ return Message.get(); }
    const SDL_Rect& getDestination() const{ return Message_rect; } 

    Messages& setColor(color newColor){ setCurrentColor(newColor); updateMessage(); return *this; }
    Messages& setMessage(std::string newMessage){ setCurrentMessage(newMessage); updateMessage(); return *this; }
    Messages& setCoodinates(int x, int y, int w = 0, int h = 0);

    Messages& setNewFont(std::string_view fontPath, int size = 24);
    Messages& setFontStyle(int style = 0){ TTF_SetFontStyle(font.get(), style); return *this; }

    Messages& nextRainbowColor();
    
    Messages& show();

    private:
    void updateMessage();
    void setCurrentMessage(std::string newMessage){ currentMessage = std::move(newMessage); }
    void setCurrentColor(color newColor);
    void setCurrentCoodinates(int x, int y, int w, int h);

    SDL_Pointer<TTF_Font, TTF_CloseFont> font;
    SDL_Pointer<SDL_Surface, SDL_FreeSurface> surfaceMessage;
    SDL_Pointer<SDL_Texture, SDL_DestroyTexture> Message;

    std::string currentMessage;
    SDL_Rect Message_rect;
    color currentColor;
    App* appPointer = nullptr;
};

SDL_Color colorToSDLColor(color currentColor);
void nextInTheRainbow(color& currentColor);

class audio
{
    public:
    explicit audio(std::string_view path, double iduration);

    audio(const audio& copyFromAudio) = delete;
    audio& operator=(const audio& copyFromAudio) = delete;

    audio(audio&& moveFromAudio);
    audio& operator=(audio&& moveFromAudio);
    
    ~audio() = default;

    inline audio& play(int loops = 0);
    inline bool done();
    static inline void stopAllMusic();

    private:
    SDL_Pointer<Mix_Music, Mix_FreeMusic> currentMusic;
    
    double duration;
    std::chrono::time_point<std::chrono::steady_clock> start;
};

class randomGen
{
    public:
    randomGen(const randomGen& copyFrom) = delete;
    randomGen& operator=(const randomGen& copyFrom) = delete;

    randomGen(randomGen&& moveFrom) = delete;
    randomGen& operator=(randomGen&& moveFrom) = delete;
    
    ~randomGen() = default;
    
    static randomGen& get() { return randomInstance; }
    
    template<typename number>
    number operator()(number from, number to)
    {
    /*
     * randomGen::operator()(int, int) generate and random number from from to to
     *
     * Precondition from can not be greater than to
     *
     * Concept number is floating or integral 
     * Concept number has less than operator 
    */
        static_assert(std::is_floating_point<number>::value || std::is_integral<number>::value, "randomGen operator must have number input");

        assert((from < to) && "randomGen operator() must have a vaild range");

        using distribution = std::conditional<std::is_floating_point<number>::value, std::uniform_real_distribution<number>, std::uniform_int_distribution<number>>::type;

        distribution range(from, to);

        return range(randomGenerator);
    }

    private:
    randomGen() : randomGenerator{randomDevice()} {}

    static randomGen randomInstance;

    std::random_device randomDevice;
    std::default_random_engine randomGenerator;
};
