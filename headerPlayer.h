//Header for player.cpp
#pragma once
#ifndef headerPlayer_H
#define headerPlayer_H
#include "headerVisuals.h"

void doKeyDown(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired);
void doKeyUp(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired);

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
};

class user : public thing
{
    public:
    int back;
    int direction;

    user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, int iback, int idirection);

    void doKeyDown(SDL_KeyboardEvent *event);
    void doKeyUp(SDL_KeyboardEvent *event);

    void input(thing& bullet, thing& bullet2, App app);
    protected:
    int playerUp = 0;
    int playerDown = 0;
    int playerLeft = 0;
    int playerRight = 0;
    int playerFired = 0;
};

class points : public thing
{
    public:
    points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture);
    void initPoints(int SCREEN_WIDTH, int SCREEN_HEIGHT);
};

void input(user& player, thing& bullet, thing& bullet2, App app, int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired);

int noEscape(user player, int SCREEN_WIDTH, int SCREEN_HEIGHT);
void noEscapeExec(user& player, int escape);

void bulletLogic(thing& bullet, user player, int SCREEN_WIDTH, int SCREEN_HEIGHT);
void thingLogic(thing& bullet, int SCREEN_WIDTH, int SCREEN_HEIGHT);

void enemys(thing& enemys, int& enemySpawnTimer, App app);
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void didBulletHit(thing& bullet, thing& enemy, long long int& counter);
void didEnemyKill(user& player, thing& enemy);
void didYouGetPoints(user& player, thing& bullet, points& point, long long int& counter);

#endif
