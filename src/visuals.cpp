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

#include "headerVisuals.h"

App App::onlyApp{};

App::App(int iSCREEN_WIDTH, int iSCREEN_HEIGHT)
 : SCREEN_WIDTH(iSCREEN_WIDTH), SCREEN_HEIGHT(iSCREEN_HEIGHT),
 renderer(nullptr), window(nullptr), windowIcon(nullptr)
{
/*
 * App::App init window, renderer and libraries 
 * 
 * Precondition SCREEN_WIDTH and SCREEN_HEIGHT must be greater than zero
 *
 * Postcondition all SDL libraries init functions are called
 * Postcondition renderer, window, and window icon are created
 * Postcondition SCREEN_WIDTH and SCREEN_HEIGHT are initalized
*/
    if(SCREEN_WIDTH <= 0 || SCREEN_HEIGHT <= 0)
        std::cerr << "SCREEN_WIDTH and SCREEN_HEIGHT can not be less than or equal to zero" << std::endl;
        
    //https://wiki.libsdl.org/SDL2/SDL_Init
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cerr << "Could not start SDL:" << SDL_GetError() << std::endl;

    int flags = IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    
    //https://wiki.libsdl.org/SDL2_image/IMG_Init
    if((initted & flags) != flags) 
    {   
        std::cerr << "IMG_Init: Failed to init png support!" << std::endl;
        std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
    }  
    
    //https://wiki.libsdl.org/SDL2_ttf/TTF_Init
    if(TTF_Init() < 0)
        std::cerr << "Could not start SDL ttf:" << TTF_GetError() << std::endl;
        
    //https://wiki.libsdl.org/SDL2_mixer/Mix_Init
    if(Mix_Init(MIX_INIT_MP3) < 0)
        std::cerr << "Could not init SDL mixer: " << Mix_GetError() << std::endl;

    if(Mix_OpenAudioDevice(44100, MIX_DEFAULT_FORMAT, 2, 2048, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE) < 0)
        std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
    
    windowIcon.reset(SDL_LoadBMP("images/Player.bmp"));

    if(!windowIcon)
        std::cerr << "SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
        
    //https://wiki.libsdl.org/SDL2/SDL_CreateWindow
    int windowFlags = SDL_WINDOW_RESIZABLE;

    window.reset(SDL_CreateWindow("Amongus 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags));

    if(!window)
        std::cerr << "Failed to open window: " << SDL_GetError() << std::endl;

    windowIcon.reset(SDL_ConvertSurfaceFormat(windowIcon.get(), SDL_PIXELFORMAT_ARGB8888, 0));
        
    if(!windowIcon)
        std::cerr << "SDL_ConvertSurfaceFormat failed: " << SDL_GetError() << std::endl;

    //https://wiki.libsdl.org/SDL2/SDL_SetWindowIcon
    SDL_SetWindowIcon(window.get(), windowIcon.get());   

    //https://wiki.libsdl.org/SDL2/SDL_RendererFlags
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    //-1 so SDL use the first graphics acceleration device it finds
    //https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
    renderer.reset(SDL_CreateRenderer(window.get(), -1, rendererFlags));

    if(!renderer)
        std::cerr << "Renderer failed: " << SDL_GetError() << std::endl;

    if(SDL_RenderSetLogicalSize(renderer.get(), SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
        std::cerr << "SDL_RenderSetLogicalSize failed: " << SDL_GetError() << std::endl;
    
    std::cout << std::string{R"( ______           ______        ______                              |\   ______  ______     ______  )"} << std::endl
              << std::string{R"(| _____|         | _____|      |_    _|                      /\     | \  | ____| | ___ |   | _____| )"} << std::endl
              << std::string{R"(| |____   _   _  | |____         |  |    _____  |\    /|    /  \    |  \ | |_    | |_| |   | |____  )"} << std::endl
              << std::string{R"(|_____ | | | | | |_____ |        |  |   | __  |  \\  //    /  ^ \   |  / | __|   |  __ \   |_____ | )"} << std::endl
              << std::string{R"( ____| | | |_| |  ____| |       _|  |_  | | | |   \\//    /  __  \  | /  | |___  | |  \ \   ____| | )"} << std::endl
              << std::string{R"(|______| |_____| |______|      |______| |_| |_|    \/    /__|  |__\ |/   |_____| |_|   \_\ |______| )"} << std::endl;
}

App::~App()
{
/*
 * App::~App quit libraries 
 * 
 * Postcondition all libraries close and quit functions are called
*/
    Mix_CloseAudio();    
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

SDL_Texture* App::loadImages(const std::string& imageFile)
{
/*
 * App::loadImage return a SDL_Texture pointer
 * 
 * Precondition imageFile must be NULL terminated 
 * Postcondition return a valid SDL_Texture pointer
*/
    SDL_Texture* Image = IMG_LoadTexture(renderer.get(), imageFile.data());

    if(!Image)
        std::cerr << "IMG_LoadTexture failed: " << IMG_GetError() << std::endl; 

    return Image;
}

SDL_Texture* App::createTextureFromSurface(SDL_Surface* convertSurface)
{
/*
 * App::createTextureFromSurface return a texture of convertSurface 
*/
    SDL_Texture* surfaceTexture = SDL_CreateTextureFromSurface(renderer.get(), convertSurface);

    if(!surfaceTexture)
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;

    return surfaceTexture;
}

App& App::imagePos(SDL_Texture* image, int x, int y, int w, int h)
{
/*
 * App::imagePos renders image to window
 *
 * Precondition image is not NULL
 * Postcondition w and h are not less than or equal to zero 
*/
    if(!image)
        std::cerr << "image can not be NULL" << std::endl;

    if(w <= 0 || h <= 0)
    {
        //Query the attributes of a texture
        //Takes image, Format, Access, width and height
        if(SDL_QueryTexture(image, NULL, NULL, &w, &h) < 0)
            std::cerr << "SDL_QueryTexture failed: " << SDL_GetError() << std::endl;
    }

    SDL_Rect dest{x, y, w, h};

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image
    if(SDL_RenderCopy(renderer.get(), image, NULL, &dest) < 0)
        std::cerr << "SDL_RenderCopy failed: " << SDL_GetError() << std::endl;

    return *this;
}

App& App::imagePos(const Image& image, int x, int y, int w, int h)
{
/*
 * App::imagePos take a Image an render it to screen 
 *
 * Postcondition w and h are not less than or equal to zero
*/
    SDL_Rect dest{x, y, w, h};
    
    if(h <= 0 || w <= 0)
    {
        dest.w = image.getImageSrc().w;
        dest.h = image.getImageSrc().h;
    }

    if(SDL_RenderCopy(renderer.get(), image.getImageTexture(), &image.getImageSrc(), &dest) < 0)
        std::cerr << "SDL_RenderCopy failed: " << SDL_GetError() << std::endl;
    
    return *this;
}

App& App::imagePos(const Messages& message)
{
/*
 * App::imagePos take a message and render it to the screen
*/
    if(SDL_RenderCopy(renderer.get(), message.getTexture(), NULL, &message.getDestination()) < 0)
        std::cerr << "SDL_RenderCopy failed: " << SDL_GetError() << std::endl;

    return *this;
}

App& App::makeVisuals()
{
/*
 * App::makeVisuals draw a black background then clear the renderer
*/
    //Color for background         0, 0, 0, 0, is for black
    if(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0) < 0)
        std::cerr << "SDL_SetRenderDrawColor failed: " << SDL_GetError() << std::endl;

    if(SDL_RenderClear(renderer.get()) < 0)
        std::cerr << "SDL_RenderClear failed: " << SDL_GetError() << std::endl;

    return *this;
}

void App::showVisuals() const
{
/*
 * App::showVisuals present the renderer
*/
    SDL_RenderPresent(renderer.get());
}



Image::Image(const std::string& path, int x, int y, int w, int h)
: imageTexture{App::get().loadImages(path.c_str())}
{
/*
 * Image::Image construct an image with one frame and a texture
 *
 * path must be NULL terminated
*/
    *this += SDL_Rect{x, y, w, h};
}

Image::Image(Image&& moveFromImage) noexcept
{
/*
 * Image::Image move image
*/
    moveFrom(moveFromImage);
}

Image& Image::operator=(Image&& moveFromImage) noexcept
{
/*
 * Image::operator= move image to an existing image
 *
 * Precondition no self assigment
*/
    if(this != &moveFromImage)
        moveFrom(moveFromImage);

    return *this;
}
    
Image& Image::operator++(int)
{ 
/*
 * Image::operator++ make currentFrame loop around frames
*/
    if(frames.size() > 1)
        (currentFrame < int(frames.size())) ? currentFrame++ : currentFrame = 0; 
    
    return *this; 
}

inline Image& Image::operator+=(SDL_Rect&& addFrame)
{
/*
 * Image::operator+= add SDL_Rect to Image
*/
    frames.push_back(addFrame);
    
    return *this;
}

Image& Image::operator+=(std::initializer_list<int> addFrames)
{
/*
 * Image::operator+= add frames to Image form ints
 *
 * Precondition addFrames must be divisable by 4
*/
    if(addFrames.size() % 4)
        std::cerr << "addFrames must be divisable by 4" << std::endl;

    int loops = int(addFrames.size()/4);
    auto currentInt = addFrames.begin();

    for(int i = 0; i < loops; i++)
    {
        int x = *currentInt;
        currentInt++;

        int y = *currentInt;
        currentInt++;

        int w = *currentInt;
        currentInt++;
        
        int h = *currentInt;
        currentInt++;
        
        frames.push_back(SDL_Rect{x, y, w, h});
    }
    
    return *this;
}
    
void Image::moveFrom(Image& moveFromImage)
{
/*
 * Image::moveFrom move from moveFromImage
*/
    imageTexture = std::move(moveFromImage.imageTexture);
    frames = std::move(moveFromImage.frames);
    currentFrame = moveFromImage.currentFrame;
}



SDL_Colour toSDLColour(const colour currentColour)
/*
 * toSDLColour return the enum currentColour as a SDL_Colour
*/
{
    const SDL_Colour White = {255, 255, 255, 225};
    const SDL_Colour Red = {255, 0, 0, 255};
    const SDL_Colour Orange = {255, 165, 0, 255};
    const SDL_Colour Yellow = {255, 255, 0, 255};
    const SDL_Colour Green = {0, 128, 0, 255};
    const SDL_Colour Blue = {0, 0, 255, 255};
    const SDL_Colour Indigo = {75, 0, 130, 255};
    const SDL_Colour Violet = {238, 130, 238, 255};

    SDL_Colour convertedColour;

    switch (currentColour)
    {
        case colour::red:
            convertedColour = Red;
            break;

        case colour::orange:
            convertedColour = Orange;
            break;

        case colour::yellow:
            convertedColour = Yellow;
            break;

        case colour::green:
            convertedColour = Green;
            break;

        case colour::blue:
            convertedColour = Blue;
            break;

        case colour::indigo:
            convertedColour = Indigo;
            break;

        case colour::violet:
            convertedColour = Violet;
            break;

        default:
            convertedColour = White;
            break;
    }

    return convertedColour;
}

void nextRainbowColour(colour& currentColour)
{
/*
 * colour::nextRainbowColour loop over the colours in rainbow order
*/
    switch(currentColour)
    {
        case colour::red:
            currentColour = colour::orange;
            break;

        case colour::orange:
            currentColour = colour::yellow;
            break;

        case colour::yellow:
            currentColour = colour::green;
            break;

        case colour::green:
            currentColour = colour::blue;
            break;

        case colour::blue:
            currentColour = colour::indigo;
            break;

        case colour::indigo:
            currentColour = colour::violet;
            break;

        default:
            currentColour = colour::red;
            break;
    }
}



Messages::Messages(std::string&& message, int x, int y, int w, int h, colour newColour)
 : font(TTF_OpenFont("images/GoogleSans-Bold-v3.003.ttf", 24)),
 surfaceMessage(nullptr), Message(nullptr), currentMessage{message},
 Message_rect{x, y, w, h}
{
/*
 * Messages::Messages open a font for font pointer and make a surface and texture for renderering to the screen
 * 
 * Precondition w and h are not less than or equal to zero
*/
    if(w <= 0 || h <= 0)
        std::cerr << "h and w can not be less than or equal to zero" << std::endl;

    if(!font)
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    
    currentColour = newColour;

    updateMessage();
}

Messages::Messages(Messages&& moveFromMessage) noexcept
 : font{std::move(moveFromMessage.font)}, 
 surfaceMessage{std::move(moveFromMessage.surfaceMessage)}, Message{std::move(moveFromMessage.Message)},
 Message_rect{moveFromMessage.Message_rect.x, moveFromMessage.Message_rect.y, moveFromMessage.Message_rect.w, moveFromMessage.Message_rect.h}
{
/*
 * Messages::Messages move moveFromMessage to new message
*/
    moveFromMessage.Message_rect.x = 0;
    moveFromMessage.Message_rect.y = 0;
    moveFromMessage.Message_rect.w = 0;
    moveFromMessage.Message_rect.h = 0;
}

Messages& Messages::operator=(Messages&& moveFromMessage) noexcept
{
/*
 * Messages::operator= move a message to an existing message
 *
 * Precondition no self assigment
*/
    if(this != &moveFromMessage)
    {
        font = std::move(moveFromMessage.font);
        surfaceMessage = std::move(moveFromMessage.surfaceMessage);
        Message = std::move(moveFromMessage.Message);

        Message_rect.x = moveFromMessage.Message_rect.x;
        Message_rect.y = moveFromMessage.Message_rect.y;
        Message_rect.w = moveFromMessage.Message_rect.w;
        Message_rect.h = moveFromMessage.Message_rect.h;

        moveFromMessage.Message_rect.x = 0;
        moveFromMessage.Message_rect.y = 0;
        moveFromMessage.Message_rect.w = 0;
        moveFromMessage.Message_rect.h = 0;
    }

    return *this;
}

Messages& Messages::newMessage(std::string&& message, int x, int y, int w, int h, colour newColour)
{
/*
 * Messages::newMessage creates a new message from x y w h and message 
*/
    setCurrentMessage(std::move(message));
    setCurrentCoodinates(x, y, w, h);
    setCurrentColour(newColour);
    
    updateMessage();

    return *this;
}

Messages& Messages::setCoodinates(int x, int y, int w, int h)
{
/*
 * Messages::setCoodinates change the x y w and h of message
 *
 * Precondition w and h are not less than or equal to zero
*/
    if(w <= 0)
        w = Message_rect.w;

    if(h <= 0)
        h = Message_rect.h;

    setCurrentCoodinates(x, y, w, h);

    return *this;
}

Messages& Messages::setNewFont(const std::string& fontPath, int size)
{
/*
 * Messages::setNewFont set font to a new font
 *
 * Postcondition font is checked
*/
    font.reset(TTF_OpenFont(fontPath.c_str(), size));
    
    if(!font)
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;

    return *this;
}

Messages& Messages::nextInRainbow()
{
/*
 * Messages::nextRainbowColour change the colour to the next colour in the rainbow
*/

    nextRainbowColour(currentColour);
    
    updateMessage();
        
    return *this;
}

void Messages::show() const
{
/*
 * Message::drawMessage draws message to screen
*/
    App::get().imagePos(*this);
}

void Messages::updateMessage()
{
/*
 * Messages::updateMessage take the current message and update the texture
*/
    SDL_Colour messageColour = toSDLColour(currentColour);
    
    surfaceMessage.reset(TTF_RenderText_Solid(font.get(), currentMessage.c_str(), messageColour));
    
    if(!surfaceMessage)
        std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;

    Message.reset(App::get().createTextureFromSurface(surfaceMessage.get()));
    
    if(!Message)
        std::cerr << "App::createTextureFromSurface failed" << std::endl;
}

void Messages::setCurrentColour(colour newColour)
{
/*
 * setCurrentColour change the current colour
*/
    currentColour = newColour;
}

void Messages::setCurrentCoodinates(int x, int y, int w, int h)
{
/*
 * Messages::setCurrentCoodinates update Message_rect
*/
    if(w <= 0)
        std::cerr << "Message width cannot be less than or equal to zero" << std::endl;
    
    if(h <= 0)
        std::cerr << "Message height cannot be less than or equal to zero" << std::endl;

    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;
}



Music::Music(const std::string& path)
 : currentMusic{Mix_LoadMUS( path.c_str() )}
{
/*
 * Music::Music load audio from path
 *
 * Precondition path is NULL terminated
 * Precondition duration is time in seconds the audio will play
 *
 * Postcondition currentMusic is vaild
*/
    if(!currentMusic)
        std::cerr << "Mix_LoadMUS failed: " << Mix_GetError() << std::endl;
}

Music::Music(Music&& moveFromMusic) noexcept
 : currentMusic{std::move(moveFromMusic.currentMusic)}
{
/*
 * Music::Music move moveFromAudio.currentMusic to new audio
*/
}

Music& Music::operator=(Music&& moveFromMusic) noexcept
{
/*
* Music::operator= move audio to existing audio
*
* Precondition no self assigment
*/
    if(this != &moveFromMusic)
        currentMusic = std::move(moveFromMusic.currentMusic);

    return *this;
}

Music& Music::play(int loops) 
{
/*
 * Music::play play currentMusic
*/
    if(Mix_PlayMusic(currentMusic.get(), loops) < 0)
        std::cerr << "Mix_PlayMusic failed: " << Mix_GetError() << std::endl; 
    
    return *this; 
}

Music& Music::fadeIn(std::chrono::milliseconds fadeInFor, int loops)
{
/*
 * Music::fadeIn fade in the music in milliseconds time at position for loops many times
*/
    if(Mix_FadeInMusic(currentMusic.get(), loops, fadeInFor.count()) < 0)
        std::cerr << "Mix_FadeInMusicPos failed: " << Mix_GetError() << std::endl;

    return *this;
}

Music& Music::setVolume(int newVolume)
{
/*
 * Music::setVolume change the current music volume
 *
 * Precondition newVolume must be between 0(silent) to MIX_MAX_VOLUME(128)
 * Precondition music is not fading
*/
    if(newVolume < 0 || newVolume > MIX_MAX_VOLUME)
        std::cerr << "newVolume invailed" << std::endl;

    Mix_Fading isFading = fading();

    if((isFading == MIX_FADING_OUT || isFading == MIX_FADING_IN))
        std::cerr << "Music is fading. Do not call setVolume during this time" << std::endl;

    Mix_VolumeMusic(newVolume);
    
    return *this;
}

Music& Music::fadeOut(std::chrono::milliseconds fadeOutFor)
{ 
/*
 * Music::fadeOut fade out the music for fadeOutFor milliseconds
*/
    Mix_FadeOutMusic(fadeOutFor.count());

    return *this; 
}



Chunk::Chunk(const std::string& path, int setChannel)
 : currentChunk{Mix_LoadWAV(path.c_str())}, channel{setChannel}
{
/*
 * Chunk::Chunk create a new chunk and set the channel(-1 for first useable channel)
*/
    if(!currentChunk)
        std::cerr << "Mix_LoadWAV failed: " << Mix_GetError() << std::endl;
}

Chunk::Chunk(Chunk&& moveFrom) noexcept
 : currentChunk{std::move(moveFrom.currentChunk)}
{
/*
 * Chunk::Chunk move from move from
*/
}

Chunk& Chunk::operator=(Chunk&& moveFrom) noexcept
{
/*
 * Chunk::operator= move from move from to existing Chunk
 *
 * Precondition no self assigment
*/
    if(this != &moveFrom)
        currentChunk = std::move(moveFrom.currentChunk);
    
    return *this;
}

Mix_Fading Chunk::fading() const 
{ 
/*
 * Chunk::fading check if chunk is fading 
 *
 * Precondition check if owns channel if not return MIX_NO_FADING
*/
    return ownChannel() ? Mix_FadingChannel(channel) : MIX_NO_FADING; 
}

bool Chunk::paused() const 
{ 
/*
 * Chunk::paused check if chunk is paused 
 *
 * Precondition check if owns channel if not false
*/
    return ownChannel() ? Mix_Paused(channel) : false;
}

bool Chunk::playing() const 
{ 
/*
 * Chunk::playing check if chunk is playing
 *
 * Precondition check if owns channel if not false
*/
    return ownChannel() ? Mix_Playing(channel) : false; 
}

Chunk& Chunk::play(int loops)
{
/*
 * Chunk::play play chunk on channel and get channel
*/
    channel = Mix_PlayChannel(channel, currentChunk.get(), loops);

    if(channel < 0)
        std::cerr << "Mix_PlayChannel failed: " << Mix_GetError() << std::endl;

    return *this;
}

Chunk& Chunk::fadeIn(std::chrono::milliseconds fadeInFor, int loops)
{
/*
 * Chunk::fadeIn fade in the chunk and get the channel
*/
    channel = Mix_FadeInChannel(channel, currentChunk.get(), loops, fadeInFor.count());

    if(channel < 0)
        std::cerr << "Mix_FadeInChannel failed: " << Mix_GetError() << std::endl;

    return *this;
}

Chunk& Chunk::setVolume(int newVolume)
{
/*
 * Chunk::setVolume set the current volume
 * 
 * Precondition owns channel
 * Precondition newVolume must be between 0(silent) to MIX_MAX_VOLUME(128)
 * Precondition music is not fading
*/
    if(newVolume < 0 || newVolume > MIX_MAX_VOLUME)
        std::cerr << "newVolume must be between 0 and MIX_MAX_VOLUME" << std::endl;

    Mix_Fading isFading = fading();

    if((isFading == MIX_FADING_OUT || isFading == MIX_FADING_IN))
        std::cerr << "Do not call setVolume when chunk is fading" << std::endl;

    if(ownChannel())
        Mix_Volume(channel, newVolume);

    return *this;
}

Chunk& Chunk::pause()
{
/*
 * Chunk::pause pause current channel
 * 
 * Precondition owns channel
*/
    if(ownChannel())
        Mix_Pause(channel);

    return *this;
}

Chunk& Chunk::resume()
{
/*
 * Chunk::resume resume channel
 * 
 * Precondition owns channel
*/
    if(ownChannel())
        Mix_Resume(channel);

    return *this;
}

Chunk& Chunk::fadeOut(std::chrono::milliseconds fadeOutFor)
{
/*
 * Chunk::fadeOut fade out channel
 *
 * Precondition owns channel
*/
    if(ownChannel())
        Mix_FadeOutChannel(channel, fadeOutFor.count());

    return *this;
}

Chunk& Chunk::stop()
{
/*
 * Chunk::stop halt channel
 * 
 * Precondition owns channel
*/
    if(ownChannel())
    {
        if(Mix_HaltChannel(channel) < 0)
            std::cerr << "Mix_HaltChannel failed: " << Mix_GetError() << std::endl;      
    }

    return *this;
}

bool Chunk::ownChannel() const
{
/*
 * Chunk::ownChannel check if currentChunk is on channel
*/
    return (!(channel < 0) && currentChunk.get() == Mix_GetChunk(channel));
}



randomGen randomGen::randomInstance;
