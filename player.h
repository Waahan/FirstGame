//Header for player.cpp
#pragma once
#ifndef player_H
#define player_H

void doKeyDown(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight);
void doKeyUp(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight);

void input(int& playerUp, int& playerDown, int& playerLeft, int& playerRight);

class user
{
    public:
	    int x;
	    int y;
	    int speed;
	    int back;
	    SDL_Texture* texture;
};


#endif
