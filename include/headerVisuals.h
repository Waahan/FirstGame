//Header for visuals.cpp
#pragma once

#include <iostream>
#include <string>
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
class Music;
class Chunk;
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
    static App& get(){ return onlyApp; }

    App(const App& copyFromApp) = delete;
    App& operator=(const App& copyFromApp) = delete;  
    
    App(App&& moveFromApp) = delete;
    App& operator=(App&& moveFromApp) = delete; 

    ~App();

    SDL_Texture *loadImages(const std::string& imageFile);
    SDL_Texture* createTextureFromSurface(SDL_Surface* currentSurface);

    App& imagePos(SDL_Texture* image, int x, int y, int w = 0, int h = 0);
    App& imagePos(const Image& image, int x, int y, int w = 0, int h = 0);
    App& imagePos(const Messages& message);

    App& makeVisuals();
    void showVisuals() const;

    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;

    private:
    explicit App(int screenWidth = 1280, int screenHeight = 720);

    static App onlyApp;

    SDL_Pointer<SDL_Renderer, SDL_DestroyRenderer> renderer;
    SDL_Pointer<SDL_Window, SDL_DestroyWindow> window;
    SDL_Pointer<SDL_Surface, SDL_FreeSurface> windowIcon;
};

class Image
{
    public:
    explicit Image(const std::string& imagePath, int x, int y, int w, int h);
    Image() : imageTexture{}, frames{} {}
    
    Image(const Image& copyFromImage) = delete;
    Image& operator=(const Image& copyFromImage) = delete;

    Image(Image&& moveFromImage) noexcept;
    Image& operator=(Image&& moveFromImage) noexcept;
    
    ~Image() = default;

    SDL_Texture* getImageTexture() const { return imageTexture.get(); }
    const SDL_Rect& getImageSrc() const { return frames[currentFrame]; }

    Image& operator++(int);
    inline Image& operator+=(SDL_Rect&& addFrame);
    Image& operator+=(std::initializer_list<int> addFrames);

    bool done() const { return int(frames.size()-1) == currentFrame; }; 

    private:
    void moveFrom(Image& moveFromImage);

    SDL_Pointer<SDL_Texture, SDL_DestroyTexture> imageTexture;
    std::vector<SDL_Rect> frames; 
    int currentFrame = 0;
};

enum class colour : unsigned char { red, orange, yellow, green, blue, indigo, violet, white, none };
SDL_Colour toSDLColour(const colour currentColour);
void nextRainbowColour(colour& currentColour);

class Messages
{
    public:
    explicit Messages(std::string&& message, int x, int y, int w, int h, colour newColour = colour::none);
    
    Messages(const Messages& copyFromMessage) = delete;
    Messages& operator=(const Messages& copyFromMessage) = delete;

    Messages(Messages&& moveFromMessage) noexcept;
    Messages& operator=(Messages&& moveFromMessage) noexcept;

    ~Messages() = default;

    Messages& newMessage(std::string&& message, int x, int y, int w, int h, colour newColour);
    
    SDL_Texture* getTexture() const{ return Message.get(); }
    const SDL_Rect& getDestination() const{ return Message_rect; } 

    Messages& setColor(colour newColour){ setCurrentColour(newColour); updateMessage(); return *this; }
    Messages& setMessage(std::string&& newMessage){ setCurrentMessage(std::move(newMessage)); updateMessage(); return *this; }
    Messages& setCoodinates(int x, int y, int w = 0, int h = 0);

    Messages& setNewFont(const std::string& fontPath, int size = 24);
    Messages& setFontStyle(int style = TTF_STYLE_NORMAL){ TTF_SetFontStyle(font.get(), style); return *this; }
    Messages& setFontHinting(int style = TTF_HINTING_NORMAL);

    Messages& nextInRainbow();
    
    void show() const;

    private:
    void updateMessage();
    void setCurrentMessage(std::string&& newMessage){ currentMessage = newMessage; }
    void setCurrentColour(colour newColour);
    void setCurrentCoodinates(int x, int y, int w, int h);

    SDL_Pointer<TTF_Font, TTF_CloseFont> font;
    SDL_Pointer<SDL_Surface, SDL_FreeSurface> surfaceMessage;
    SDL_Pointer<SDL_Texture, SDL_DestroyTexture> Message;

    std::string currentMessage;
    SDL_Rect Message_rect;
    colour currentColour;
};

class Audio
{
    public:
    Audio() = default;

    Audio(const Audio& copyFrom) = delete;
    Audio& operator=(const Audio& copyFrom) = delete;

    Audio(Audio&& moveFrom) = default;
    Audio& operator=(Audio&& moveFrom) = default;

    virtual ~Audio(){}
    
    virtual Mix_Fading fading() const =0;
    virtual bool paused() const =0;
    virtual bool playing() const =0;

    virtual Audio& play(int loops = 0) =0;
    virtual Audio& fadeIn(std::chrono::milliseconds fadeInFor, int loops = 0) =0;

    virtual Audio& setVolume(int newVolume = MIX_MAX_VOLUME) =0;
    virtual Audio& pause() =0;
    virtual Audio& resume() =0;

    virtual Audio& fadeOut(std::chrono::milliseconds fadeOutFor) =0;
    virtual Audio& stop() =0;
};

class Music : public Audio
{
    public:
    explicit Music(const std::string& path);

    Music(const Music& copyFromMusic) = delete;
    Music& operator=(const Music& copyFromMusic) = delete;

    Music(Music&& moveFromMusic) noexcept;
    Music& operator=(Music&& moveFromMusic) noexcept;
    
    ~Music() = default;

    Mix_Fading fading() const override { return Mix_FadingMusic(); }
    bool paused() const override { return Mix_PausedMusic(); }
    bool playing() const override { return Mix_PlayingMusic(); }

    Music& play(int loops = 0) override; 
    Music& fadeIn(std::chrono::milliseconds fadeInFor, int loops = 0) override; 
    
    Music& setVolume(int newVolume = MIX_MAX_VOLUME) override;
    Music& pause() override { Mix_PauseMusic(); return *this; }
    Music& resume() override { Mix_ResumeMusic(); return *this; }

    Music& fadeOut(std::chrono::milliseconds fadeOutFor) override;
    Music& stop() override { Mix_HaltMusic(); return *this; };
    
    private:
    SDL_Pointer<Mix_Music, Mix_FreeMusic> currentMusic;
};

class Chunk : public Audio
{
    public:
    explicit Chunk(const std::string& path, int setChannel = -1);
    
    Chunk(const Chunk& copyFrom) = delete;
    Chunk& operator=(const Chunk& copyFrom) = delete;

    Chunk(Chunk&& moveFrom) noexcept;
    Chunk& operator=(Chunk&& moveFrom) noexcept;

    ~Chunk() = default;
    
    static void allocateChannels(int numberOfChannels){ Mix_AllocateChannels(numberOfChannels); }

    Mix_Fading fading() const override; 
    bool paused() const override; 
    bool playing() const override;

    Chunk& play(int loops = 0) override;
    Chunk& fadeIn(std::chrono::milliseconds fadeInFor, int loops = 0) override;
    
    Chunk& setVolume(int newVolume = MIX_MAX_VOLUME) override;
    Chunk& pause() override;
    Chunk& resume() override;
    
    Chunk& fadeOut(std::chrono::milliseconds fadeOutFor) override;
    Chunk& stop() override;

    private:
    bool ownChannel() const;

    SDL_Pointer<Mix_Chunk, Mix_FreeChunk> currentChunk;
    int channel;
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
    requires std::is_arithmetic_v<number>
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
        assert((from < to) && "randomGen operator() must have a vaild range");

        using distribution = std::conditional_t<std::is_floating_point_v<number>, std::uniform_real_distribution<number>, std::uniform_int_distribution<number>>;

        distribution range(from, to);

        return range(randomGenerator);
    }

    private:
    randomGen() : randomGenerator{randomDevice()} {}

    static randomGen randomInstance;

    std::random_device randomDevice;
    std::default_random_engine randomGenerator;
};
