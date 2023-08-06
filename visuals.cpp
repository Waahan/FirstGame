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

#include "headerVisuals.h"

App::App(int iSCREEN_WIDTH, int iSCREEN_HEIGHT)
 : SCREEN_WIDTH(iSCREEN_WIDTH), SCREEN_HEIGHT(iSCREEN_HEIGHT),
 renderer(nullptr), 
 //https://wiki.libsdl.org/SDL2/SDL_CreateWindow
 window(SDL_CreateWindow("Amongus 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE)), 
 windowIcon(SDL_LoadBMP("images/Player.bmp"))
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

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    
    //https://wiki.libsdl.org/SDL2_image/IMG_Init
    if((initted & flags) != flags) 
    {   
        std::cerr << "IMG_Init: Failed to init required jpg and png support!" << std::endl;
        std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
    }  
    
    //https://wiki.libsdl.org/SDL2_ttf/TTF_Init
    if(TTF_Init() < 0)
        std::cerr << "Could not start SDL ttf:" << TTF_GetError() << std::endl;
        
    //https://wiki.libsdl.org/SDL2_mixer/Mix_Init
    if(Mix_Init(MIX_INIT_MP3) < 0)
        std::cerr << "Could not init SDL mixer: " << Mix_GetError() << std::endl;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
        std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
    
    if(!windowIcon)
        std::cerr << "SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
        
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
    
    // Sus invaders

/*
 ______
|_    _|
  |  |
  |  |
 _|  |_
|______|

 ______
| _____|
| |____
|_____ |
 ____| |
|______|

 _   _
| | | |
| |_| |
|_____|
 
 ______
|  __  |
|  | | |
|__| |_|

|\    /|
 \\  //
  \\//
   \/

    /\
   /  \
  /  ^ \
 /  __  \
/__|  |__\

 ______
|
|
|
|
*/
}

App::App(App&& moveFromApp)
 : SCREEN_WIDTH{moveFromApp.SCREEN_WIDTH}, SCREEN_HEIGHT{moveFromApp.SCREEN_HEIGHT}, 
 renderer(std::move(moveFromApp.renderer)), window{std::move(moveFromApp.window)}, 
 windowIcon{std::move(moveFromApp.windowIcon)}
{
}

App& App::operator=(App&& moveFromApp)
{
    if(this != &moveFromApp)
    {
        window = std::move(moveFromApp.window);
        renderer = std::move(moveFromApp.renderer);
        windowIcon = std::move(moveFromApp.windowIcon);
    }

    return *this;
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

SDL_Texture* App::loadImages(std::string_view imageFile)
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
        dest.w = image.getCurrentImageSrc().w;
        dest.h = image.getCurrentImageSrc().h;
    }

    if(SDL_RenderCopy(renderer.get(), image.getImageTexture(), &image.getCurrentImageSrc(), &dest) < 0)
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



Image::Image(std::string_view path, App& app, int x, int y, int w, int h)
: imageTexture{app.loadImages(path.data())}
{
/*
 * Image::Image construct an image with one frame and a texture
 *
 * path must be NULL terminated
*/
    SDL_Rect temp{x, y, w, h};
    
    images.push_back(temp);    
}

Image::Image(Image&& moveFromImage)
 : imageTexture{std::move(moveFromImage.imageTexture)}, 
 images{std::move(moveFromImage.images)}, 
 currentImageNum{moveFromImage.currentImageNum}
{
/*
 * Image::Image move image
*/
    moveFromImage.currentImageNum = 0;
}

Image& Image::operator=(Image&& moveFromImage)
{
/*
 * Image::operator= move image to an existing image
 *
 * Precondition no self assigment
*/
    if(this != &moveFromImage)
    {
        imageTexture = std::move(moveFromImage.imageTexture);
        images = std::move(moveFromImage.images);
        currentImageNum = moveFromImage.currentImageNum;

        moveFromImage.currentImageNum = 0;
    }

    return *this;
}

SDL_Texture* Image::getImageTexture() const
{
/*
 * Image::getImageTexture return Image's texture
 *
 * Postcondition does not return a invalid texture without warning
*/
    if(imageTexture)
        return imageTexture.get();

    std::cerr << "Image texture not found" << std::endl;
    
    return NULL;
}

Image& Image::operator++(int)
{
/*
 * Image::operator++(int) go to next frame if there is one. If not go back to zero
 *
 * Postcondition currentImageNum does not go out of vectors range
*/
    currentImageNum < int(images.size()-1) ? currentImageNum++ : currentImageNum = 0;

    return *this;
}

inline Image& Image::operator+=(SDL_Rect&& addFrame)
{
/*
 * Image::operator+= add SDL_Rect to Image
*/
    images.push_back(addFrame);
    
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
        SDL_Rect temp{*currentInt, *++currentInt, *++currentInt, *++currentInt};
        images.push_back(temp);
    }
    
    return *this;
}

inline Image& Image::reset()
{
/*
 * Image::reset set currentImageNum to zero from whatever value it was before
*/
    currentImageNum = 0;

    return *this;
}



Messages::Messages(std::string message, int x, int y, int w, int h, App* app, color newColor)
 : font(TTF_OpenFont("images/GoogleSans-Bold-v3.003.ttf", 24)), 
 surfaceMessage(nullptr), Message(nullptr), currentMessage{std::move(message)},
 Message_rect{x, y, w, h}
{
/*
 * Messages::Messages open a font for font pointer and make a surface and texture for renderering to the screen
 * 
 * Precondition w and h are not less than or equal to zero
 * Precondition app is not NULL
*/
    if(!app)
        std::cerr << "app can not be NULL" << std::endl;

    if(w <= 0 || h <= 0)
        std::cerr << "h and w can not be less than or equal to zero" << std::endl;
    
    if(!font)
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    
    currentColor = newColor;
    appPointer = app;      
    
    updateMessage();
}

Messages::Messages(Messages&& moveFromMessage)
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

Messages& Messages::operator=(Messages&& moveFromMessage)
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

Messages& Messages::newMessage(std::string message, int x, int y, int w, int h, color newColor)
{
/*
 * Messages::newMessage creates a new message from x y w h and message 
*/
    setCurrentMessage(message);
    setCurrentCoodinates(x, y, w, h);
    setCurrentColor(newColor);
    
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

Messages& Messages::setNewFont(std::string_view fontPath, int size)
{
/*
 * Messages::setNewFont set font to a new font
 *
 * Postcondition font is checked
*/
    font.reset(TTF_OpenFont(fontPath.data(), size));
    
    if(!font)
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;

    return *this;
}

Messages& Messages::nextRainbowColor()
{
/*
 * Messages::nextRainbowColor change the color to the next color in the rainbow
*/

    nextInTheRainbow(currentColor);
    
    updateMessage();
        
    return *this;
}

Messages& Messages::show()
{
/*
 * Message::drawMessage draws message to screen
*/
    appPointer->imagePos(*this);

    return *this;
}

void Messages::updateMessage()
{
/*
 * Messages::updateMessage take the current message and update the texture
*/
    SDL_Color messageColor = colorToSDLColor(currentColor);
    
    surfaceMessage.reset(TTF_RenderText_Solid(font.get(), currentMessage.c_str(), messageColor));
    
    if(!surfaceMessage)
        std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;

    Message.reset(appPointer->createTextureFromSurface(surfaceMessage.get()));
    
    if(!Message)
        std::cerr << "App::createTextureFromSurface failed" << std::endl;
}

void Messages::setCurrentColor(color newColor)
{
/*
 * setCurrentColor change the current color
 * 
 * Precondition newColor is not none
*/
    if(newColor == color::none)
        std::cerr << "newColor cannot be color::none" << std::endl;
    
    currentColor = newColor;
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

SDL_Color colorToSDLColor(color currentColor)
{
/*
 * colorToSDLColor return the color of currentColor as a SDL_Color
*/
    const SDL_Color White = {255, 255, 255, 225};
    const SDL_Color Red = {255, 0, 0, 255};
    const SDL_Color Orange = {255, 165, 0, 255};
    const SDL_Color Yellow = {255, 255, 0, 255};
    const SDL_Color Green = {0, 128, 0, 255};
    const SDL_Color Blue = {0, 0, 255, 255};
    const SDL_Color Indigo = {75, 0, 130, 255};
    const SDL_Color Violet = {238, 130, 238, 255};

    SDL_Color messageColor; 

    switch (currentColor)
    {
        case color::red:
            messageColor = Red;
            break;

        case color::orange:
            messageColor = Orange;
            break;

        case color::yellow:
            messageColor = Yellow;
            break;

        case color::green:
            messageColor = Green;
            break;

        case color::blue:
            messageColor = Blue;
            break;

        case color::indigo:
            messageColor = Indigo;
            break;

        case color::violet:
            messageColor = Violet;
            break;

        default:
            messageColor = White;
            break;
    }

    return messageColor;
}

void nextInTheRainbow(color& currentColor)
{
/*
 * nextInTheRainbow loop over the colors in rainbow order
*/
    switch(currentColor)
    {
        case color::red:
            currentColor = color::orange;
            break;

        case color::orange:
            currentColor = color::yellow;
            break;

        case color::yellow:
            currentColor = color::green;
            break;

        case color::green:
            currentColor = color::blue;
            break;

        case color::blue:
            currentColor = color::indigo;
            break;

        case color::indigo:
            currentColor = color::violet;
            break;

        case color::violet:
            currentColor = color::red;
            break;

        default:
            currentColor = color::red;
            break;
    }
}



audio::audio(std::string_view path, double iduration)
 : currentMusic{Mix_LoadMUS(path.data())}, duration{iduration} 
{
/*
 * audio::audio load audio from path
 *
 * Precondition path is NULL terminated
 * Precondition duration is time in seconds the audio will play
 *
 * Postcondition currentMusic is vaild
*/
    if(!currentMusic)
        std::cerr << "Mix_LoadMUS failed: " << Mix_GetError() << std::endl;
}

audio::audio(audio&& moveFromAudio)
 : currentMusic{std::move(moveFromAudio.currentMusic)}
{
/*
 * audio::audio move moveFromAudio.currentMusic to new audio
*/
}

audio& audio::operator=(audio&& moveFromAudio)
{
/*
* audio::operator= move audio to existing audio
*
* Precondition no self assigment
*/
    if(this != &moveFromAudio)
        currentMusic = std::move(moveFromAudio.currentMusic);

    return *this;
}

inline audio& audio::play(int loops)
{
/*
 * audio::play play music(negative to play forever)
*/
    if(Mix_PlayMusic(currentMusic.get(), loops) < 0)
        std::cerr << "Mix_PlayMusic failed: " << Mix_GetError() << std::endl;

    start = std::chrono::steady_clock::now();
    
    return *this;
}

inline bool audio::done()
{
/*
 * audio::done find out if music is done using chrono
*/
    std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();

    std::chrono::duration<double> timeSinceStart = std::chrono::duration_cast<std::chrono::seconds>(currentTime - start);

    return (timeSinceStart.count() >= duration);
}

inline void audio::stopAllMusic()
{
/*
 * audio stopAllMusic stop all currently playing music
*/
    Mix_HaltMusic();
}



randomGen randomGen::randomInstance;
