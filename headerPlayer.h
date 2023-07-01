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
class counter;
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

    thing(const thing& copyFromThing);
    thing& operator=(const thing& copyFromThing);

    thing(thing&& moveFromThing);
    thing& operator=(thing&& moveFromThing);

    virtual ~thing(){ SDL_DestroyTexture(texture); }

    virtual thing& logic();
    virtual thing& newTexture(SDL_Texture* newTexture);
    virtual thing& newTexture(const char* newTexturePath);
    virtual int show();

    inline int getX() const;
    inline int getY() const;
    inline int getW() const;
    inline int getH() const;
    inline int getHealth() const;
    inline int getSpeed() const;
    inline SDL_Texture* getTexture() const;

    inline thing& setX(int setX);
    inline thing& setY(int setY);
    inline thing& setW(int setW);
    inline thing& setH(int setH);
    inline thing& setSpeed(int setSpeed);
    inline thing& setHealth(int setHealth);

    inline thing& removeFromScreen();
    inline thing& minusHealth(int subtractNum);

    protected:    
    int x;
    int y;
    int w;
    int h;
    int health;
    int speed;
    SDL_Texture* texture = NULL;
    App* appPointer = NULL;
};

class counter
{
    public:
    counter();

    inline unsigned long long count() const { return currentCount; }
    std::string stringCurrentCount();
    counter& operator++(int);
    
    protected:
    void updateStringCount(){ stringCount = std::to_string(currentCount); oldCount = currentCount; }

    unsigned long long currentCount = 0;

    unsigned long long oldCount = 0;
    std::string stringCount;
};

class user : public thing
{
    public:
    explicit user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer, int iback, int idirection);
    user(const user& copyFromUser);
    user(user&& moveFromUser);
    ~user();

    user& doKeyDown(SDL_KeyboardEvent *event, bool DownUp);

    user& input();

    user& keyMenu(bool& start, SDL_KeyboardEvent *event);
    user& menuInput(bool& start);

    user& logic(thing& enemy, points& point);

    int show() override;
    void playerDeath();

    inline int getDirection() const;

    counter playerScore;

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
    explicit enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    enemys(const enemys& copyFromEnemy);
    enemys(enemys&& moveFromEnemy);
    ~enemys(){ SDL_DestroyTexture(texture); }

    enemys& spawnEnemys();
    enemys& didEnemyKill(user& player);
    void makeEnd(int& levelOne);
    enemys& scaleDifficulty(const counter& playerScore);

    private:
    int minimum = 1;
    int maximum = 15;
    int enemySpawnTimer = 0;
};

class points : public thing
{
    public:
    explicit points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    points(const points& copyFromPoints);
    points(points&& moveFromPoint);
    ~points(){ SDL_DestroyTexture(texture); }

    points& initPoints();
    points& didYouGetPoints(user& player, thing& bullet, counter& playerScore);

    private:
    int randomNum;
    bool isHealth = false;
};

class bulletClass : public thing
{
    friend user;

    public:
    explicit bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer);
    bulletClass() : thing() {};
    bulletClass(const bulletClass& copyFromBullet);
    bulletClass(bulletClass&& moveFromBullet);
    ~bulletClass(){ SDL_DestroyTexture(texture); }

    bulletClass& logic(const user& player);
    bulletClass& didBulletHit(thing& enemy, counter& playerScore);
};

class healthDisplay
{
    public:     
    explicit healthDisplay(SDL_Texture* ifullHealth = nullptr, SDL_Texture* ihalfHealth = nullptr, SDL_Texture* icritical = nullptr);
    healthDisplay(const healthDisplay& copyFromHealthDisplay) = delete;
    healthDisplay(healthDisplay&& moveFromHealthDisplay);
    ~healthDisplay(){ SDL_DestroyTexture(fullHealth); SDL_DestroyTexture(halfHealth); SDL_DestroyTexture(critical); }
     
    SDL_Texture* healthDisplayUpdate(const user& player);
     
    SDL_Texture* fullHealth;
    SDL_Texture* halfHealth;
    SDL_Texture* critical;
};

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
