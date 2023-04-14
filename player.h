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

	    thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture)
	    {
	        x = ix;
	        y = iy;
	        w = iw;
	        h = ih;
	        health = ihealth;
	        speed = ispeed;
	        texture = itexture;
	    }
};

class user : public thing
{
    public:
	    int back;
	    int direction;

    	    user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, int iback, int idirection) : thing(ix, iy, iw, ih, ihealth, ispeed, itexture)
	    {
		back = iback;
		direction = idirection;
	    }

};


#endif
