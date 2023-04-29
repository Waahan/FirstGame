//Header for player.cpp
#pragma once
#include "headerVisuals.h"
#include <fstream>
#include <string>

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
    thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture);
    virtual void logic(int SCREEN_WIDTH, int SCREEN_HEIGHT);
};

class bulletClass;

class user : public thing
{
    friend bulletClass;

    public:
    int back;
    int direction;

    user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, int iback, int idirection);

    void doKeyDown(SDL_KeyboardEvent *event);
    void doKeyUp(SDL_KeyboardEvent *event);

    void input(thing& bullet, thing& bullet2, App app);

    virtual void logic(int SCREEN_WIDTH, int SCREEN_HEIGHT);

    protected:
    int playerUp = 0;
    int playerDown = 0;
    int playerLeft = 0;
    int playerRight = 0;
    int playerFired = 0;
};

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

class enemys : public thing
{
    public:
    enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture) : thing(ix, iy, iw, ih, ihealth, ispeed, itexture){}

    void spawnEnemys(int& enemySpawnTimer);
    void didEnemyKill(user& player);
    void makeEnd(int& levelOne, App app);
};

class points : public thing
{
    public:
    points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture);

    void initPoints(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    void didYouGetPoints(user& player, thing& bullet, int& counter);
};

class bulletClass : public thing
{
    public:
    bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture) : thing(ix, iy, iw, ih, ihealth, ispeed, itexture){};

    void logic(user player, int SCREEN_WIDTH, int SCREEN_HEIGHT);
    void didBulletHit(thing& enemy, int& counter);
};

class healthDisplay
{
     public:
     healthDisplay(SDL_Texture* ifullHealth, SDL_Texture* ihalfHealth, SDL_Texture* icritical);
     SDL_Texture* healthDisplayUpdate(user& player);
     
     SDL_Texture* fullHealth;
     SDL_Texture* halfHealth;
     SDL_Texture* critical;
};
