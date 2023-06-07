//Header for player.cpp
#pragma once
#include <string>
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
    virtual ~thing(){ SDL_DestroyTexture(texture); }

    virtual void logic(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    virtual void newTexture(SDL_Texture* newTexture);
    virtual void newTexture(const char* newTexturePath);
    virtual int show();
    
    int x;
    int y;
    int w;
    int h;
    int health;
    int speed;
    SDL_Texture* texture;
    App* appPointer;
};

class user : public thing
{
    friend bulletClass;

    public:
    user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer, int iback, int idirection);
    user(user& userCopy) = delete;
    ~user(){ SDL_DestroyTexture(texture); }

    void doKeyDown(SDL_KeyboardEvent *event);
    void doKeyUp(SDL_KeyboardEvent *event);

    void input(thing& bullet, thing& bullet2);

    void keyMenu(bool& start, SDL_KeyboardEvent *event);
    void menuInput(bool& start);

    void logic(int SCREEN_WIDTH, int SCREEN_HEIGHT);

    void playerDeath(const int& SCREEN_WIDTH, const int& SCREEN_HEIGHT);

    protected:
    int direction;
    int playerUp = 0;
    int playerDown = 0;
    int playerLeft = 0;
    int playerRight = 0;
    int playerFired = 0;
    int back;
};

class enemys : public thing
{
    public:
    enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    ~enemys(){ SDL_DestroyTexture(texture); }

    void spawnEnemys(int& enemySpawnTimer, user& player);
    void didEnemyKill(user& player);
    void makeEnd(int& levelOne);
    void scaleDifficulty(int& counter);

    private:
    int minimum = 1;
    int maximum = 15;
    int smart;
};

class points : public thing
{
    public:
    points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    ~points(){ SDL_DestroyTexture(texture); }

    void initPoints(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    void didYouGetPoints(user& player, thing& bullet, int& counter);

    private:
    int randomNum;
};

class bulletClass : public thing
{
    public:
    bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    ~bulletClass(){ SDL_DestroyTexture(texture); }

    void logic(user& player, int SCREEN_WIDTH, int SCREEN_HEIGHT);
    void didBulletHit(thing& enemy, int& counter);
};

class healthDisplay
{
     public:
     healthDisplay(SDL_Texture* ifullHealth, SDL_Texture* ihalfHealth, SDL_Texture* icritical);
     ~healthDisplay(){ SDL_DestroyTexture(fullHealth); SDL_DestroyTexture(halfHealth); SDL_DestroyTexture(critical); }
     
     SDL_Texture* healthDisplayUpdate(user& player);
     
     SDL_Texture* fullHealth;
     SDL_Texture* halfHealth;
     SDL_Texture* critical;
};

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
