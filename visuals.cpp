#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <future>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "headerVisuals.h"
#include "headerPlayer.h"

App::App(int iSCREEN_WIDTH, int iSCREEN_HEIGHT) : SCREEN_WIDTH(iSCREEN_WIDTH), SCREEN_HEIGHT(iSCREEN_HEIGHT)
{
/*
 * App::App init window, renderer and libraries 
 * 
 * pre and postconditions:
 * 
 * Precondition SCREEN_WIDTH and SCREEN_HEIGHT must be greater than zero
*/
    if(SCREEN_WIDTH <= 0 || SCREEN_HEIGHT <= 0)
    {
        throw std::invalid_argument("SCREEN_WIDTH and SCREEN_HEIGHT can not be less than or equal to zero");
    }
    
    std::string errorMessage = "";
    bool failedSDL = false;
    bool failedIMG = false;
    bool failedTTF = false;
    bool failedMix = false;
    bool failedOpenAudio = false;
    bool failedWindowIcon = false;
    bool failedWindow = false;
    bool failedRenderer = false;

    try
    {
        int rendererFlags = SDL_RENDERER_ACCELERATED, windowFlags = 0;

        //https://wiki.libsdl.org/SDL2/SDL_RendererFlags
        rendererFlags = SDL_RENDERER_ACCELERATED;
        
        //https://wiki.libsdl.org/SDL2/SDL_WindowFlags
        windowFlags = 0;
     
        //https://wiki.libsdl.org/SDL2/SDL_Init
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cerr << "Could not start SDL:" << SDL_GetError() << std::endl;
            errorMessage = errorMessage+"SDL_Init failed";
            failedSDL = true;
        }

        int flags = IMG_INIT_JPG | IMG_INIT_PNG;
        int initted = IMG_Init(flags);
    
        //https://wiki.libsdl.org/SDL2_image/IMG_Init
        if((initted & flags) != flags) 
        {   
            std::cerr << "IMG_Init: Failed to init required jpg and png support!" << std::endl;
            std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
            errorMessage = errorMessage+"IMG_Init failed";
            failedIMG = true;
        }  
    
        //https://wiki.libsdl.org/SDL2_ttf/TTF_Init
        if(TTF_Init() < 0)
        {
            std::cerr << "Could not start SDL ttf:" << TTF_GetError() << std::endl;
	    errorMessage = errorMessage+"TTF_Init failed";
            failedTTF = true;
        }
        
        //https://wiki.libsdl.org/SDL2_mixer/Mix_Init
        if(Mix_Init(MIX_INIT_MP3) < 0)
        {
            std::cerr << "Could not init SDL mixer: " << Mix_GetError() << std::endl;
            errorMessage = errorMessage+"Mix_Init failed";
            failedMix = true;
        }

        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
        {
            std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
            errorMessage = errorMessage+"Mix_OpenAudio Failed";
            failedOpenAudio = true;
        }
    
        windowIcon = SDL_LoadBMP("images/Player.bmp");

        if(windowIcon == NULL)
        {
            std::cerr << "SDL_LoadBMP failed: " << SDL_GetError() << std::endl;
            errorMessage = errorMessage+"SDL_LoadBMP failed";
            failedWindowIcon = true;
        } 
        
        //https://wiki.libsdl.org/SDL2/SDL_CreateWindow
        window = SDL_CreateWindow("Amongus 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

        if(!window)
        {
            std::cerr << "Failed to open window: " << SDL_GetError() << std::endl;
	    errorMessage = errorMessage+"SDL_CreateWindow failed";
            failedWindow = true;
        }

        windowIcon = SDL_ConvertSurfaceFormat(windowIcon, SDL_PIXELFORMAT_ARGB8888, 0);
        
        if(windowIcon == NULL)
        {
            std::cerr << "SDL_ConvertSurfaceFormat failed: " << SDL_GetError() << std::endl;
            errorMessage = errorMessage+"SDL_ConvertSurfaceFormat failed";
            failedWindowIcon = true;
        }

        //https://wiki.libsdl.org/SDL2/SDL_SetWindowIcon
        SDL_SetWindowIcon(window, windowIcon);   

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        //-1 so SDL use the first graphics acceleration device it finds
        //https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
        renderer = SDL_CreateRenderer(window, -1, rendererFlags);

        //Check if renderer is working
        if(!renderer)
        {
            std::cerr << "Renderer failed: " << SDL_GetError() << std::endl;
            errorMessage = errorMessage+"SDL_CreateRenderer failed";
            failedRenderer = true;
        }

        if(errorMessage != "")
        {
            throw std::runtime_error(errorMessage);
        }
    }
    catch(...)
    {
        if(!failedRenderer)
        {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        if(!failedWindow)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        if(!failedWindowIcon)
        {
            SDL_FreeSurface(windowIcon);
            windowIcon = nullptr;
        }

        if(!failedOpenAudio)
            Mix_CloseAudio();

        if(!failedMix)
            Mix_Quit();

        if(!failedTTF)
            TTF_Quit();

        if(!failedIMG)
            IMG_Quit();

        if(!failedSDL)
            SDL_Quit();

        throw;
    }
}

App::App(App&& moveFromApp)
 : SCREEN_WIDTH{moveFromApp.SCREEN_WIDTH}, SCREEN_HEIGHT{moveFromApp.SCREEN_HEIGHT}, window{moveFromApp.window}, renderer{moveFromApp.renderer}, windowIcon{moveFromApp.windowIcon}
{
    moveFromApp.window = nullptr;
    moveFromApp.renderer = nullptr;
    moveFromApp.windowIcon = nullptr;
}

App& App::operator=(App&& moveFromApp)
{
    if(this != &moveFromApp)
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_FreeSurface(windowIcon);

        window = moveFromApp.window;
        renderer = moveFromApp.renderer;
        windowIcon = moveFromApp.windowIcon;

        moveFromApp.window = nullptr;
        moveFromApp.renderer = nullptr;
        moveFromApp.windowIcon = nullptr;
    }

    return *this;
}

App::~App()
{
/*
 * App::~App quit libraries and destroy window and renderer
 * 
 * pre and postconditions:
 *
 * Postcondition renderer and window pointers should be NULL to avoid errors
*/
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_FreeSurface(windowIcon);
    windowIcon = nullptr;

    Mix_CloseAudio();    

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

SDL_Texture* App::loadImages(const char* imageFile)
{
/*
 * App::loadImage return a SDL_Texture pointer
 * 
 * pre and postconditions:
 *
 * Precondition const char* imageFile is not NULL
 *
 * Postcondition return a valid SDL_Texture pointer
*/
    if(imageFile == NULL || imageFile == nullptr)
    {
        throw std::invalid_argument("imageFile can not be NULL");
    }

    SDL_Texture* Image = Image = IMG_LoadTexture(renderer, imageFile);

    if(Image == NULL)
    {
        std::cerr << "IMG_LoadTexture failed: " << IMG_GetError() << std::endl; 
	throw std::runtime_error("IMG_LoadTexture failed");
    }

    return Image;
}

App& App::imagePos(SDL_Texture* image, int x, int y, int w, int h)
{
/*
 * App::imagePos render image onto the window
 *
 * pre and postconditions:
 *
 * Precondition image is not NULL
 * Precondition w and h must be not be less than or equal to 0
*/
    if(image == NULL || image == nullptr)
    {
        throw std::invalid_argument("image pointer can not be NULL");
    }
    else if(w <= 0 || h <= 0)
    {
        throw std::invalid_argument("width and height can not be less than or equal to 0");
    }

    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image
    SDL_RenderCopy(renderer, image, NULL, &dest);

    return *this;
}

App& App::imagePos(SDL_Texture* image, int x, int y)
{
/*
 * App::imagePos renders image to window
 *
 * pre and postconditions:
 *
 * Precondition image is not NULL
*/
    if(image == NULL || image == nullptr)
    {
        throw std::invalid_argument("image can not be NULL");
    }

    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    //Query the attributes of a texture
    //Takes image, Format(just set to NULL), Access(Also set to NULL), width and height
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image
    SDL_RenderCopy(renderer, image, NULL, &dest);

    return *this;
}

App& App::imagePos(const Image& image, int x, int y, int w, int h)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    if(h == 0 || w == 0)
    {
        dest.w = image.getCurrentImageSrc().w;
        dest.h = image.getCurrentImageSrc().h;
    }

    SDL_RenderCopy(renderer, image.getImageTexture(), &image.getCurrentImageSrc(), &dest);
    
    return *this;
}

App& App::makeVisuals()
{
/*
 * App::makeVisuals draw a black background then clear the renderer
*/
    //Color for background         0, 0, 0, 0, is for black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    return *this;
}

App& App::showVisuals()
{
/*
 * App::showVisuals present the renderer
*/
    SDL_RenderPresent(renderer);

    return *this;
}



Image::Image(std::string path, App& app, int x, int y, int w, int h)
: imageTexture{app.loadImages(path.c_str())}
{
    SDL_Rect temp;
    temp.x = x;
    temp.y = y;
    temp.w = w;
    temp.h = h;

    images.push_back(temp);
}

Image::Image(Image&& moveFromImage)
 : imageTexture{moveFromImage.imageTexture}, images{std::move(moveFromImage.images)}, currentImageNum{moveFromImage.currentImageNum}
{
    moveFromImage.imageTexture = nullptr;
    moveFromImage.currentImageNum = 0;
}

Image& Image::operator=(Image&& moveFromImage)
{
    if(this != &moveFromImage)
    {
        SDL_DestroyTexture(imageTexture);

        imageTexture = moveFromImage.imageTexture;
        images = std::move(moveFromImage.images);
        currentImageNum = moveFromImage.currentImageNum;

        moveFromImage.imageTexture = nullptr;
        moveFromImage.currentImageNum = 0;
    }

    return *this;
}



Messages::Messages(std::string message, int x, int y, int w, int h, App* app, Messages::color newColor)
{
/*
 * Messages::Messages open a font for font pointer and make a surface and texture for renderering to the screen
 * 
 * pre and postconditions:
 *
 * w and h are not less than or equal to zero
 * app is not NULL
*/
    if(app == NULL || app == nullptr)
    {
        throw std::invalid_argument("app can not be NULL");
    }
    else if(w <= 0 || h <= 0)
    {
        throw std::invalid_argument("h and w can not be less than or equal to zero");
    }

    std::string errorMessage = "";
    bool openFontFailed = false;
    bool surfaceMessageFailed = false;
    bool textureMessageFailed = false;

    try
    {
        //this opens a font style and sets a size
        font = TTF_OpenFont("images/GoogleSans-Bold-v3.003.ttf", 24);

        if(font == NULL)
        {
            std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
	    errorMessage = errorMessage+"TTF_OpenFont failed";
            openFontFailed = true;
        }
    
        SDL_Color messageColor;

        colorToSDLColor(messageColor, newColor);
        
        // You have to create the surface first
        surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), messageColor);

        if(surfaceMessage == NULL)
        {
            std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
            errorMessage = errorMessage+"TTF_RenderText_Solid failed";
            surfaceMessageFailed = true;
        }
    
        // now you can convert it into a texture
        Message = SDL_CreateTextureFromSurface(app->renderer, surfaceMessage);

        if(Message == NULL)
        {
            std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            errorMessage = errorMessage+"SDL_CreateTextureFromSurface failed";
            textureMessageFailed = true;
        }

        currentMessage = std::move(message);
        currentColor = newColor;
        appPointer = app;      
        Message_rect.x = x;  //controls the rect's x coordinate
        Message_rect.y = y; // controls the rect's y coordinte
        Message_rect.w = w; // controls the width of the rect
        Message_rect.h = h; // controls the height of the rect

        if(errorMessage != "")
        {
            throw std::runtime_error(errorMessage);
        }
    }
    catch(...)
    {
        if(!surfaceMessageFailed)
        {
            SDL_FreeSurface(surfaceMessage);
            surfaceMessage = nullptr;
        }

        if(!textureMessageFailed)
        {
            SDL_DestroyTexture(Message);
            Message = nullptr;
        }
        
        if(!openFontFailed) 
        {
            TTF_CloseFont(font);
            font = nullptr;
        }

        throw;
    }
}

Messages::Messages(Messages&& moveFromMessage)
 : font{moveFromMessage.font}, surfaceMessage{moveFromMessage.surfaceMessage}, Message{moveFromMessage.Message}
{
    Message_rect.x = moveFromMessage.Message_rect.x;
    Message_rect.y = moveFromMessage.Message_rect.y;
    Message_rect.w = moveFromMessage.Message_rect.w;
    Message_rect.h = moveFromMessage.Message_rect.h;

    moveFromMessage.font = NULL;
    moveFromMessage.surfaceMessage = NULL;
    moveFromMessage.Message = NULL;
    
    moveFromMessage.Message_rect.x = 0;
    moveFromMessage.Message_rect.y = 0;
    moveFromMessage.Message_rect.w = 0;
    moveFromMessage.Message_rect.h = 0;
}

Messages& Messages::operator=(Messages&& moveFromMessage)
{
    if(this != &moveFromMessage)
    {
        TTF_CloseFont(font);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);

        font = moveFromMessage.font;
        surfaceMessage = moveFromMessage.surfaceMessage;
        Message = moveFromMessage.Message;

        Message_rect.x = moveFromMessage.Message_rect.x;
        Message_rect.y = moveFromMessage.Message_rect.y;
        Message_rect.w = moveFromMessage.Message_rect.w;
        Message_rect.h = moveFromMessage.Message_rect.h;

        moveFromMessage.font = NULL;
        moveFromMessage.surfaceMessage = NULL;
        moveFromMessage.Message = NULL;

        moveFromMessage.Message_rect.x = 0;
        moveFromMessage.Message_rect.y = 0;
        moveFromMessage.Message_rect.w = 0;
        moveFromMessage.Message_rect.h = 0;
    }

    return *this;
}

Messages::~Messages()
{
/*
 * Messages::~Messages free surface and texture 
 * 
 * pre and postconditions:
 * 
 * Postcondition surfaceMessage and Message should be NULL to avoid errors
 * Postcondition font should be closed and set to NULL
*/
    SDL_FreeSurface(surfaceMessage);
    surfaceMessage = nullptr;

    SDL_DestroyTexture(Message);
    Message = nullptr;

    TTF_CloseFont(font);
    font = nullptr;
}

Messages& Messages::newMessage(std::string message, int x, int y, int w, int h, Messages::color newColor)
{
/*
 * Messages::newMessage creates a new message from x y w h and message 
 * 
 * pre and postconditions:
 *
 * Precondition w and h can not be less than zero
 * Precondition set message to current message if it is invalidMessage
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
        throw std::invalid_argument("new message h and w can not be less than zero");
    }

    SDL_FreeSurface(surfaceMessage);

    SDL_Color messageColor;

    colorToSDLColor(messageColor, newColor);

    surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), messageColor);

    if(surfaceMessage == NULL || surfaceMessage == nullptr)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
	throw std::runtime_error("TTF_RenderText_Solid failed");
    }
    
    SDL_DestroyTexture(Message);

    Message = SDL_CreateTextureFromSurface(appPointer->renderer, surfaceMessage);

    if(Message == NULL || Message == nullptr)
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
    SDL_RenderCopy(appPointer->renderer, Message, NULL, &Message_rect);

    return *this;
}

Messages& Messages::colorToSDLColor(SDL_Color& messageColor, Messages::color newColor)
{
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

    return *this;
}

Messages& Messages::nextColor()
{
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
    nextColor();

    SDL_FreeSurface(surfaceMessage);

    SDL_Color messageColor;

    colorToSDLColor(messageColor, currentColor);

    surfaceMessage = TTF_RenderText_Solid(font, currentMessage.c_str(), messageColor);

    if(surfaceMessage == NULL || surfaceMessage == nullptr)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
        throw std::runtime_error("TTF_RenderText_Solid failed");
    }

    SDL_DestroyTexture(Message);

    Message = SDL_CreateTextureFromSurface(appPointer->renderer, surfaceMessage);

    if(Message == NULL || Message == nullptr)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    return *this;
}



audio::audio(std::string path)
 : currentMusic{Mix_LoadMUS(path.c_str())}
{
    if(currentMusic == NULL || currentMusic == nullptr)
    {
        std::cerr << "Mix_LoadMUS failed: " << Mix_GetError() << std::endl;
        throw std::runtime_error("Mix_LoadMUS failed");
    }
}

audio::audio(audio&& moveFromAudio)
 : currentMusic{moveFromAudio.currentMusic}
{
    moveFromAudio.currentMusic = NULL;
}

audio& audio::operator=(audio&& moveFromAudio)
{
    if(this != &moveFromAudio)
    {
        currentMusic = moveFromAudio.currentMusic;
        moveFromAudio.currentMusic = NULL;
    }

    return *this;
}

inline audio& audio::play(int loops)
{
    if(Mix_PlayMusic(currentMusic, loops) < 0)
    {
        std::cerr << "Mix_PlayMusic failed: " << Mix_GetError() << std::endl;
        throw std::runtime_error("Mix_PlayMusic failed");
    }
    
    return *this;
}
