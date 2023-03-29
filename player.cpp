#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "player.h"
using namespace std;

void input()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
	    case SDL_QUIT:
                exit(0);
		break;

	    default:
		break;
	}
    }
}
