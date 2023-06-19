//Header for player.cpp
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <future>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "headerVisuals.h"

class thing;
class user;
class enemys;
class points;
class bulletClass;
class healthDisplay;

class thing
{
    public:
    thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    thing(){};
    thing(thing& copyThing) = delete;
    virtual ~thing(){ SDL_DestroyTexture(texture); }

    virtual void logic();
    virtual void newTexture(SDL_Texture* newTexture);
    virtual void newTexture(const char* newTexturePath);
    virtual int show();
    
    int x;
    int y;
    int w;
    int h;
    int health;
    int speed;
    SDL_Texture* texture = NULL;
    App* appPointer = NULL;
};

class user : public thing
{
    friend bulletClass;

    public:
    user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer, int iback, int idirection);
    user(user& userCopy) = delete;
    ~user();

    void doKeyDown(SDL_KeyboardEvent *event, bool DownUp);

    void input();

    void keyMenu(bool& start, SDL_KeyboardEvent *event);
    void menuInput(bool& start);

    void logic(thing& enemy, points& point, int& counter);

    int show();
    void playerDeath();

    protected:
    char direction;
    bool playerUp = 0;
    bool playerDown = 0;
    bool playerLeft = 0;
    bool playerRight = 0;
    bool playerFired = 0;
    char back;
    
    SDL_Texture* healthDisplayCurrent;
    healthDisplay* playerHealth;

    std::vector<bulletClass*> bullets;
};

class enemys : public thing
{
    public:
    enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    enemys(enemys& copyEnemy) = delete;
    ~enemys(){ SDL_DestroyTexture(texture); }

    void spawnEnemys(int& enemySpawnTimer);
    void didEnemyKill(user& player);
    void makeEnd(int& levelOne);
    void scaleDifficulty(int counter);

    private:
    int minimum = 1;
    int maximum = 15;
    int smart;
};

class points : public thing
{
    public:
    points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    points(points& copyPoints) = delete;
    ~points(){ SDL_DestroyTexture(texture); }

    void initPoints();
    void didYouGetPoints(user& player, thing& bullet, int& counter);

    private:
    int randomNum;
    bool isHealth = false;
};

class bulletClass : public thing
{
    friend user;

    public:
    bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    bulletClass() : thing() {};
    bulletClass(bulletClass& copyBullet) = delete;
    ~bulletClass(){ SDL_DestroyTexture(texture); }

    void logic(const user& player);
    void didBulletHit(thing& enemy, int& counter);
};

class healthDisplay
{
    public:     
    healthDisplay(SDL_Texture* ifullHealth, SDL_Texture* ihalfHealth, SDL_Texture* icritical);
    healthDisplay() : fullHealth(nullptr), halfHealth(nullptr), critical(nullptr){}
    healthDisplay(healthDisplay& copyHealthDisplay) = delete;
    ~healthDisplay(){ SDL_DestroyTexture(fullHealth); SDL_DestroyTexture(halfHealth); SDL_DestroyTexture(critical); }
     
    SDL_Texture* healthDisplayUpdate(const user& player);
     
    SDL_Texture* fullHealth;
    SDL_Texture* halfHealth;
    SDL_Texture* critical;
};

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
