//Header for player.cpp
#pragma once
#ifndef player_H
#define player_H

void doKeyDown(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired);
void doKeyUp(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired);

void input(int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired);

class thing
{
    public:
	    int x;
	    int y;
	    int w;
	    int h;
	    int health;
	    int speed;
	    SDL_Texture* texture;
};

class user : public thing
{
    public:
	    int back;
	    int direction;
};


#endif
