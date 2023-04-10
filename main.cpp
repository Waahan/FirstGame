#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

//Screen size
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//needed for window
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//For the player movement
int playerUp = 0;
int playerDown = 0;
int playerLeft = 0;
int playerRight = 0;

int* pPlayerUp = &playerUp;
int* pPlayerDown = &playerDown;
int* pPlayerLeft = &playerLeft;
int* pPlayerRight = &playerRight;

//creates window
void initSDL()
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;
    
    //Check if it is working
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Could not start SDL:" << SDL_GetError();
    }

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    //Check if window was created
    if (!window)
    {
        cout << "Failed to open window: " << SDL_GetError();
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    //-1 so SDL use the first graphics acceleration device it finds
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    //Check if renderer is working
    if(!renderer)
    {
        cout << "Renderer failed: " << SDL_GetError();
    }

    //Allows window to load .png and .jpg images
    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if ((initted & flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
}

//For loading image to the window
SDL_Texture *loadImages(char* imageFile)
{
    //A variable to pass the image to SDL
    SDL_Texture *Image;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", imageFile);

    Image = IMG_LoadTexture(renderer, imageFile);

    return Image;
}

//sets image location 
void imagePos(SDL_Texture* image, int x, int y)
{
    SDL_Rect dest;

    //Where the image goes
    dest.x = x;
    dest.y = y;

    //Query the attributes of a texture
    //Takes image, Format(just set to NULL), Access(Also set to NULL), width and height
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image 
    SDL_RenderCopy(renderer, image, NULL, &dest);
}

void makeVisuals()
{
    //Color for background         0, 0, 0, 0, is for black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void showVisuals()
{
    SDL_RenderPresent(renderer);
}

int noEscape(user player)
{
    // This keeps the player on the screen, remember [x, y] where y is upside down
    if (player.x < 0)
    {
        cout << "out of bounds X+=\n";
	return 1;
    }
    else if (player.x > 1200)
    {
        cout << "out of bounds\n";
	return 2;
    }
    if (player.y < 0)
    {
        cout << "out of bounds Y-=\n";
	return 3;
    }
    else if (player.y > 640)
    {
        cout << "out of bounds Y+=\n";
	return 4;
    }
    else
    {
        return 0;
    }
}

int main(int argc, char* args[])
{
    initSDL();

    user player;
    player.x = 0;
    player.y = 0;
    player.speed = 4;
    player.back = 100;
    player.texture = loadImages("images/Player.png");

    while (1)
    {
        makeVisuals();

	int escape = noEscape(player);

	if (escape == 0)
	{
	    input(*pPlayerUp, *pPlayerDown, *pPlayerLeft, *pPlayerRight);
	    if (playerUp)
	    {
	        player.y -= player.speed;
	        cout << "Player Y: " << player.y << "\n";
	    }
	    if (playerDown)
	    {
	        player.y += player.speed;
	        cout << "Player Y: " << player.y << "\n";
	    }
	    if (playerLeft)
	    {
	        player.x -= player.speed;
	        cout << "Player X: " << player.x << "\n";
	    }
	    if (playerRight)
	    {			
	        player.x += player.speed;
	        cout << "Player X: " << player.x << "\n";
	    }
	}
	else if (escape == 1)
	{
	    player.x += player.back;
	}
	else if (escape == 2)
	{
            player.x -= player.back;
	}
	else if (escape == 3)
	{
            player.y += player.back;
	}
	else if (escape == 4)
	{
            player.y -= player.back;
	}

	imagePos(player.texture, player.x, player.y);
	showVisuals();

        //Delay is in milliseconds so its .10 of a second 
	SDL_Delay(10);
    }
    return 0;
}
