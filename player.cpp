#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "player.h"
using namespace std;

//For keyboard event detection. More scan codes at https://wiki.libsdl.org/SDL2/SDL_Scancode
void doKeyDown(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight)
{
    //Ignores keyboard repeat events
    if(event->repeat == 0)
    {
	// This keeps the player on the screen, remember [x, y] where y is upside down
    	if (playerLeft < 0)
	{
            playerRight = 1;
	    cout << "out of bounds";
	}
	else if (playerRight > 1184)
	{
            playerLeft = 1;
	    cout << "out of bounds";
	}
    	if (playerUp < 0)
	{
            playerDown = 1;
	    cout << "out of bounds";
	}
	else if (playerDown > 628)
	{
             playerUp = 1;
	     cout << "out of bounds";
	}
	//For using the arrow keys
	
	//"Up" (the Up arrow key (navigation keypad))
        if(event->keysym.scancode == SDL_SCANCODE_UP)
	{
	    playerUp = 1;
	}
	//"Down" (the Down arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_DOWN)
	{
	    playerDown = 1;
	}
	//"Left" (the Left arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_LEFT)
	{
	    playerLeft = 1;
	}
	//"Right" (the Right arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_RIGHT)
	{
	    playerRight = 1;
	}

	//For using WASD
	
	//"Up" (the Up arrow key (navigation keypad))
        if(event->keysym.scancode == SDL_SCANCODE_W)
	{
	    playerUp = 1;
	}
	//"Down" (the Down arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_S)
	{
	    playerDown = 1;
	}
	//"Left" (the Left arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_A)
	{
	    playerLeft = 1;
	}
	//"Right" (the Right arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_D)
	{
	    playerRight = 1;
	}

    }
}

void doKeyUp(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight)
{
    //Ignores keyboard repeat events
    if(event->repeat == 0)
    {
	//For using the arrow keys
	
	//"Up" (the Up arrow key (navigation keypad))
        if(event->keysym.scancode == SDL_SCANCODE_UP)
	{
	    playerUp = 0;
	}
	//"Down" (the Down arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_DOWN)
	{
	    playerDown = 0;
	}
	//"Left" (the Left arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_LEFT)
	{
	    playerLeft = 0;
	}
	//"Right" (the Right arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_RIGHT)
	{
	    playerRight = 0;
	}

	//For using WASD
	
	//"Up" (the Up arrow key (navigation keypad))
        if(event->keysym.scancode == SDL_SCANCODE_W)
	{
	    playerUp = 0;
	}
	//"Down" (the Down arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_S)
	{
	    playerDown = 0;
	}
	//"Left" (the Left arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_A)
	{
	    playerLeft = 0;
	}
	//"Right" (the Right arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_D)
	{
	    playerRight = 0;
	}

    }
}


void input(int& playerUp, int& playerDown, int& playerLeft, int& playerRight)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
	    case SDL_QUIT:
                exit(0);
		break;

	    case SDL_KEYDOWN:
		doKeyDown(&event.key, playerUp, playerDown, playerLeft, playerRight);
		break;

	    case SDL_KEYUP:
		doKeyUp(&event.key, playerUp, playerDown, playerLeft, playerRight);
		break;

	    default:
		break;
	}
    }
}
