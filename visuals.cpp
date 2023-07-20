#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <future>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "headerVisuals.h"
#include "headerPlayer.h"

App::App(int iSCREEN_WIDTH, int iSCREEN_HEIGHT)
 : SCREEN_WIDTH(iSCREEN_WIDTH), SCREEN_HEIGHT(iSCREEN_HEIGHT), 
 //https://wiki.libsdl.org/SDL2/SDL_CreateWindow
 window(SDL_CreateWindow("Amongus 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE)), 
 renderer(nullptr), windowIcon(SDL_LoadBMP("images/Player.bmp"))
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
    {
        std::cerr << "SCREEN_WIDTH and SCREEN_HEIGHT can not be less than or equal to zero" << std::endl;
    }
        
    //https://wiki.libsdl.org/SDL2/SDL_Init
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Could not start SDL:" << SDL_GetError() << std::endl;
    }

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
    {
        std::cerr << "Could not start SDL ttf:" << TTF_GetError() << std::endl;
    }
        
    //https://wiki.libsdl.org/SDL2_mixer/Mix_Init
    if(Mix_Init(MIX_INIT_MP3) < 0)
    {
        std::cerr << "Could not init SDL mixer: " << Mix_GetError() << std::endl;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
    }
    
    if(!windowIcon)
    {
        std::cerr << "SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
    } 
        
    if(!window)
    {
        std::cerr << "Failed to open window: " << SDL_GetError() << std::endl;
    }

    windowIcon.reset(SDL_ConvertSurfaceFormat(windowIcon.get(), SDL_PIXELFORMAT_ARGB8888, 0));
        
    if(!windowIcon)
    {
        std::cerr << "SDL_ConvertSurfaceFormat failed: " << SDL_GetError() << std::endl;
    }

    //https://wiki.libsdl.org/SDL2/SDL_SetWindowIcon
    SDL_SetWindowIcon(window.get(), windowIcon.get());   

    //https://wiki.libsdl.org/SDL2/SDL_RendererFlags
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    //-1 so SDL use the first graphics acceleration device it finds
    //https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
    renderer.reset(SDL_CreateRenderer(window.get(), -1, rendererFlags));

    if(!renderer)
    {
        std::cerr << "Renderer failed: " << SDL_GetError() << std::endl;
    }

    if(SDL_RenderSetLogicalSize(renderer.get(), SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
    {
        std::cerr << "SDL_RenderSetLogicalSize failed: " << SDL_GetError() << std::endl;
    }
}

App::App(App&& moveFromApp)
 : SCREEN_WIDTH{moveFromApp.SCREEN_WIDTH}, SCREEN_HEIGHT{moveFromApp.SCREEN_HEIGHT}, 
 window{std::move(moveFromApp.window)}, renderer{std::move(moveFromApp.renderer)}, windowIcon{std::move(moveFromApp.windowIcon)}
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

SDL_Texture* App::loadImages(std::string imageFile)
{
/*
 * App::loadImage return a SDL_Texture pointer
 * 
 * Postcondition return a valid SDL_Texture pointer
*/
    SDL_Texture* Image = Image = IMG_LoadTexture(renderer.get(), imageFile.c_str());

    if(Image == NULL)
    {
        std::cerr << "IMG_LoadTexture failed: " << IMG_GetError() << std::endl; 
    }

    return Image;
}

App& App::imagePos(SDL_Texture* image, int x, int y, int w, int h)
{
/*
 * App::imagePos render image onto the window
 *
 * Precondition image is not NULL
 * Precondition w and h must be not be less than or equal to 0
*/
    if(!image)
    {
        std::cerr << "image pointer can not be NULL" << std::endl;
    }
    else if(w <= 0 || h <= 0)
    {
        std::cerr << "width and height can not be less than or equal to 0. App::imagePos" << std::endl;
    }

    SDL_Rect dest{x, y, w, h};

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image
    if(SDL_RenderCopy(renderer.get(), image, NULL, &dest) < 0)
    {
        std::cerr << "SDL_RenderCopy failed: " << SDL_GetError() << std::endl;
    }

    return *this;
}

App& App::imagePos(SDL_Texture* image, int x, int y)
{
/*
 * App::imagePos renders image to window
 *
 * Precondition image is not NULL
*/
    if(!image)
    {
        std::cerr << "image can not be NULL" << std::endl;
    }

    SDL_Rect dest{x, y};

    //Query the attributes of a texture
    //Takes image, Format, Access, width and height
    if(SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h) < 0)
    {
        std::cerr << "SDL_QueryTexture failed: " << SDL_GetError() << std::endl;
    }

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image
    if(SDL_RenderCopy(renderer.get(), image, NULL, &dest) < 0)
    {
        std::cerr << "SDL_RenderCopy failed: " << SDL_GetError() << std::endl;
    }

    return *this;
}

App& App::imagePos(Image& image, int x, int y, int w, int h)
{
/*
* App::imagePos take a Image an render it to screen 
*/
    SDL_Rect dest{x, y, w, h};
    
    if(h == 0 || w == 0)
    {
        dest.w = image.getCurrentImageSrc().w;
        dest.h = image.getCurrentImageSrc().h;
    }

    if(SDL_RenderCopy(renderer.get(), image.getImageTexture(), &image.getCurrentImageSrc(), &dest) < 0)
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



Image::Image(std::string path, App& app, int x, int y, int w, int h)
: imageTexture{app.loadImages(path.c_str())}
{
/*
* Image::Image construct an image with one frame and a texture
*/
    SDL_Rect temp{x, y, w, h};
    
    images.push_back(temp);    
}

Image::Image(Image&& moveFromImage)
 : imageTexture{std::move(moveFromImage.imageTexture)}, images{std::move(moveFromImage.images)}, currentImageNum{moveFromImage.currentImageNum}
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
    currentImageNum < images.size()-1 ? currentImageNum++ : currentImageNum = 0;

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
    {
        std::cerr << "addFrames must be divisable by 4" << std::endl;
    }

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
 surfaceMessage(nullptr), 
 Message(nullptr)
{
/*
 * Messages::Messages open a font for font pointer and make a surface and texture for renderering to the screen
 * 
 * Precondition w and h are not less than or equal to zero
 * Precondition app is not NULL
*/
    if(!app)
    {
        std::cerr << "app can not be NULL" << std::endl;
    }
    else if(w <= 0 || h <= 0)
    {
        std::cerr << "h and w can not be less than or equal to zero" << std::endl;
    }
    
    //this opens a font style and sets a size
    font.reset(TTF_OpenFont("images/GoogleSans-Bold-v3.003.ttf", 24));

    if(!font)
    {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    }
    
    SDL_Color messageColor;

    colorToSDLColor(messageColor, newColor);
        
    // You have to create the surface first
    surfaceMessage.reset(TTF_RenderText_Solid(font.get(), message.c_str(), messageColor));

    if(!surfaceMessage)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
    }
    
    // now you can convert it into a texture
    Message.reset(SDL_CreateTextureFromSurface(app->renderer.get(), surfaceMessage.get()));

    if(!Message)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    currentMessage = std::move(message);
    currentColor = newColor;
    appPointer = app;      
    Message_rect.x = x;  //controls the rect's x coordinate
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = w; // controls the width of the rect
    Message_rect.h = h; // controls the height of the rect
}

Messages::Messages(Messages&& moveFromMessage)
 : font{std::move(moveFromMessage.font)}, 
 surfaceMessage{std::move(moveFromMessage.surfaceMessage)}, Message{std::move(moveFromMessage.Message)}
{
/*
* Messages::Messages move moveFromMessage to new message
*/
    Message_rect.x = moveFromMessage.Message_rect.x;
    Message_rect.y = moveFromMessage.Message_rect.y;
    Message_rect.w = moveFromMessage.Message_rect.w;
    Message_rect.h = moveFromMessage.Message_rect.h;
    
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
 * 
 * Precondition w and h can not be less than zero
 * Precondition set message to current message if it is invalidMessage
 * Precondition set newColor to currentColor if color is color::none
*/
    if(message == "invalidMessage")
    {
        message = currentMessage;
    }
    
    if(newColor == color::none)
    {
        newColor = currentColor;
    }

    if(w < 0 || h < 0)
    {
        std::cerr << "new message h and w can not be less than zero" << std::endl;
    }

    SDL_Color messageColor;

    colorToSDLColor(messageColor, newColor);

    surfaceMessage.reset(TTF_RenderText_Solid(font.get(), message.c_str(), messageColor));

    if(!surfaceMessage)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
    }
    
    Message.reset(SDL_CreateTextureFromSurface(appPointer->renderer.get(), surfaceMessage.get()));

    if(!Message)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    if(currentMessage != message)
    {
        currentMessage = std::move(message);
    }

    if(x != 2345)
    {
        Message_rect.x = x;
    }
    
    if(y != 2345)
    {
        Message_rect.y = y;
    }
    
    if(w != 0)
    {
        Message_rect.w = w;
    }
    
    if(h != 0)
    {
        Message_rect.h = h;
    }

    return *this;
}

Messages& Messages::drawMessage()
{
/*
 * Message::drawMessage draws message to screen
*/
    if(SDL_RenderCopy(appPointer->renderer.get(), Message.get(), NULL, &Message_rect) < 0)
        std::cerr << "SDL_RenderCopy failed: " << SDL_GetError() << std::endl;

    return *this;
}

//Make helper function soon
void Messages::colorToSDLColor(SDL_Color& messageColor, color newColor)
{
/*
* Messages::colorToSDLColor set messageColor to newColor
*/
    switch (newColor)
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
}

Messages& Messages::nextColor()
{
/*
* Messages::nextColor go to next color in rainbow order
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

    return *this;
}

Messages& Messages::rainbowColorSwitch()
{
/*
* Messages::rainbowColorSwitch make message color the next color in the rainbow
*/
    nextColor();

    SDL_Color messageColor;

    colorToSDLColor(messageColor, currentColor);

    surfaceMessage.reset(TTF_RenderText_Solid(font.get(), currentMessage.c_str(), messageColor));

    if(!surfaceMessage)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
    }

    Message.reset(SDL_CreateTextureFromSurface(appPointer->renderer.get(), surfaceMessage.get()));

    if(!Message)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    return *this;
}



audio::audio(std::string path, double iduration)
 : currentMusic{Mix_LoadMUS(path.c_str())}, duration{iduration} 
{
/*
* audio::audio load audio from path
*
* Postcondition currentMusic is vaild
*/
    if(!currentMusic)
    {
        std::cerr << "Mix_LoadMUS failed: " << Mix_GetError() << std::endl;
    }
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
    {
        currentMusic = std::move(moveFromAudio.currentMusic);
    }

    return *this;
}

inline audio& audio::play(int loops)
{
/*
* audio::play play music(negative to play forever)
*/
    if(Mix_PlayMusic(currentMusic.get(), loops) < 0)
    {
        std::cerr << "Mix_PlayMusic failed: " << Mix_GetError() << std::endl;
    }

    start = std::chrono::steady_clock::now();
    
    return *this;
}

inline bool audio::done()
{
/*
* audio::done find out if music is done using chrono
*/
    std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();

    std::chrono::duration<double> timeSinceStart = currentTime - start;

    return (timeSinceStart.count() >= duration);
}

inline void audio::stopAllMusic()
{
/*
* audio stopAllMusic stop all currently playing music
*/
    Mix_HaltMusic();
}
