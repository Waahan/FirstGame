#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

//Screen size
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//needed for window
SDL_Window* Window = NULL;
SDL_Surface* ScreenSurface = NULL;
SDL_Surface* Image = NULL;

bool startWindow()
{
    bool loaded = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        loaded = false;
    }
    else
    {
	//Create a window
        Window = SDL_CreateWindow( "SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	if( Window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            loaded = false;
        }
        else
        {
	    //Load image
	    SDL_BlitSurface( Image, NULL, ScreenSurface, NULL );
            //Get window surface
            ScreenSurface = SDL_GetWindowSurface( Window );
        }
    }

    return loaded;
}

bool loadMedia()
{
    bool loaded = true;

    Image = SDL_LoadBMP( "images/background.bmp" );

    if( Image == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", " ", SDL_GetError() );
        loaded = false;
    }

    return loaded;
}

void shutdownProgram()
{
    //Deallocate surface
    SDL_FreeSurface( Image );
    Image = NULL;

    //Destroy window
    SDL_DestroyWindow( Window );
    Window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* args[])
{
    startWindow();
    loadMedia();
    
    sleep(60);

    shutdownProgram();

    return 0;
}
