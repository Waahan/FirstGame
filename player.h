//Header for player.cpp
#pragma once
#ifndef player_H
#define player_H

typedef struct playerDirections
{
    int playerUp;
    int playerDown;
    int playerLeft;
    int playerRight;
} playerD;

void doKeyDown(SDL_KeyboardEvent *event, playerD playerd);
void doKeyUp(SDL_KeyboardEvent *event, playerD playerd);

void input(playerD playerd);

class user
{
    public:
	    int x;
	    int y;
	    SDL_Texture* texture;
};


#endif
