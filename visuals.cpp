#include <iostream>
#include <string>
#include <algorithm>
#include <climits>
#include <future>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "headerVisuals.h"
#include "headerPlayer.h"

App::App(const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT)
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

    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Could not start SDL:" << SDL_GetError() << std::endl;
	throw std::runtime_error("SDL_Init failed");
    }

    if(TTF_Init() < 0)
    {
        std::cerr << "Could not start SDL ttf:" << SDL_GetError() << std::endl;
	throw std::runtime_error("TTF_Init failed");
    }

    window = SDL_CreateWindow("Amongus 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    //Check if window was created
    if (!window)
    {
        std::cerr << "Failed to open window: " << SDL_GetError() << std::endl;
	throw std::runtime_error("SDL_CreateWindow failed");
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    //-1 so SDL use the first graphics acceleration device it finds
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    //Check if renderer is working
    if(!renderer)
    {
        std::cerr << "Renderer failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("SDL_CreateRenderer failed");
    }

    //Allows window to load .png and .jpg images
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if ((initted & flags) != flags) 
    {
	std::cerr << "IMG_Init: Failed to init required jpg and png support!" << std::endl;
	std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
	throw std::runtime_error("IMG_Init failed");
    }
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

    IMG_Quit();
    SDL_Quit();
}

//For loading image to the window
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
        std::cerr << "IMG_LoadTexture failed: " << SDL_GetError() << std::endl; 
	throw std::runtime_error("IMG_LoadTexture failed");
    }

    return Image;
}

void App::imagePos(SDL_Texture* image, int x, int y, int w, int h)
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

    //Query the attributes of a texture
    //Takes image, Format(just set to NULL), Access(Also set to NULL), width and height
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image
    SDL_RenderCopy(renderer, image, NULL, &dest);
}

void App::imagePos(SDL_Texture* image, int x, int y)
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
}

void App::makeVisuals()
{
/*
 * App::makeVisuals draw a black background then clear the renderer
*/
    //Color for background         0, 0, 0, 0, is for black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void App::showVisuals()
{
/*
 * App::showVisuals present the renderer
*/
    SDL_RenderPresent(renderer);
}



Messages::Messages(const char* message, int x, int y, int w, int h, App& app)
{
/*
 * Messages::Messages open a font for font pointer and make a surface and texture for renderering to the screen
 * 
 * pre and postconditions:
 *
 * Precondition message is not NULL
 * Precondtition w and h are not less than or equal to 0
*/
    if(message == NULL || message == nullptr)
    {
        throw std::invalid_argument("message can not be NULL");
    }
    else if(w <= 0 || h <= 0)
    {
        throw std::invalid_argument("h and w can not be less than or equal to zero");
    }

    //this opens a font style and sets a size
    font = TTF_OpenFont("images/GoogleSans-Bold-v3.003.ttf", 24);

    if(font == NULL || font == nullptr)
    {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("TTF_OpenFont failed");
    }

    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    surfaceMessage = TTF_RenderText_Solid(font, message, White);

    if(surfaceMessage == NULL || surfaceMessage == nullptr)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("TTF_RenderText_Solid failed");
    }

    // now you can convert it into a texture
    Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);

    if(Message == NULL || Message == nullptr)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("SDL_CreateTextureFromSurface failed");
    }

    Message_rect.x = x;  //controls the rect's x coordinate
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = w; // controls the width of the rect
    Message_rect.h = h; // controls the height of the rect
}

Messages::Messages(const char* message, int x, int y, int w, int h, const SDL_Color& color, App& app)
{
/*
 * Message::Messages open a font for the font pointer and set the color, x y w h, and message
 * 
 * pre and post conditions:
 * 
 * Precondition message is not NULL
 * Precondition w and h are not less than or equal to zero 
*/
    if(message == NULL || message == nullptr)
    {
        throw std::invalid_argument("message can not be NULL");
    }
    else if(w <= 0 || h <= 0)
    {
        throw std::invalid_argument("w and h can not be less than or equal to zero");
    }

    //this opens a font style and sets a size
    font = TTF_OpenFont("images/GoogleSans-Bold-v3.003.ttf", 24);

    if(font == NULL || font == nullptr)
    {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("TTF_OpenFont failed");
    }

    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    surfaceMessage = TTF_RenderText_Solid(font, message, color);

    if(surfaceMessage == NULL || surfaceMessage == nullptr)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("TTF_RenderText_Solid failed");
    }

    // now you can convert it into a texture
    Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);

    if(Message == NULL)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("SDL_CreateTextureFromSurface failed");
    }

    Message_rect.x = x;  //controls the rect's x coordinate
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = w; // controls the width of the rect
    Message_rect.h = h; // controls the height of the rect
}

Messages::~Messages()
{
/*
 * Messages::~Messages free surface and texture 
 * 
 * pre and postconditions:
 * 
 * Postcondition surfaceMessage and Message should be NULL to avoid errors
*/
    SDL_FreeSurface(surfaceMessage);
    surfaceMessage = nullptr;

    SDL_DestroyTexture(Message);
    Message = nullptr;
}

void Messages::newMessage(const char* message, int x, int y, int w, int h, App& app)
{
/*
 * Messages::newMessage creates a new message from x y w h and message 
 * 
 * pre and postconditions:
 *
 * Precondition message can not be NULL
 * Precondition w and h must not be less than or equal to 0
*/
    if(message == NULL || nullptr)
    {
        throw std::invalid_argument("new message can not be NULL");
    }
    else if(w <= 0 || h <= 0)
    {
        throw std::invalid_argument("new message h and w can not be less than or equal to zero");
    }

    SDL_FreeSurface(surfaceMessage);

    surfaceMessage = TTF_RenderText_Solid(font, message, White);

    if(surfaceMessage == NULL || surfaceMessage == nullptr)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("TTF_RenderText_Solid failed");
    }

    SDL_DestroyTexture(Message);

    Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);

    if(Message == NULL || Message == nullptr)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;
}

void Messages::newMessage(const char* message, int x, int y, int w, int h, const SDL_Color& color, App& app)
{
/*
 * Messages::newMessage takes message x y w h and color and draws it to screen
 * 
 * pre and postconditions:
 *
 * Precondition message can not be NULL
 * Precondition w and h can not be less than or equal to zero
*/
    if(message == NULL || message == nullptr)
    {
        throw std::invalid_argument("message to be NULL");
    }
    else if(w <= 0 || h <= 0)
    {
        throw std::invalid_argument("w and h can not be less than or equal to zero");
    }

    SDL_FreeSurface(surfaceMessage);

    surfaceMessage = TTF_RenderText_Solid(font, message, color);

    if(surfaceMessage == NULL || surfaceMessage == nullptr)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("TTF_RenderText_Solid failed");
    }

    SDL_DestroyTexture(Message);

    Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);

    if(Message == NULL || Message == nullptr)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
	throw std::runtime_error("SDL_CreateTextureFromSurface failed");
    }

    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;
}

void Messages::drawMessage(App& app)
{
/*
 * Message::drawMessage draws message to screen
*/
    SDL_RenderCopy(app.renderer, Message, NULL, &Message_rect);
}

