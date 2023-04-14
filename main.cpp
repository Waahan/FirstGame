#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

//Screen size
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//needed for window
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//For the player movement
int playerUp = 0;
int playerDown = 0;
int playerLeft = 0;
int playerRight = 0;
int playerFired = 0;

int* pPlayerUp = &playerUp;
int* pPlayerDown = &playerDown;
int* pPlayerLeft = &playerLeft;
int* pPlayerRight = &playerRight;
int* pPlayerFired = &playerFired;

//For Enemy
int enemySpawnTimer = 0;

//creates window
void initSDL(SDL_Window* window, SDL_Renderer* renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;
    
    //Check if it is working
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Could not start SDL:" << SDL_GetError();
    }

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    //Check if window was created
    if (!window)
    {
        cout << "Failed to open window: " << SDL_GetError();
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    //-1 so SDL use the first graphics acceleration device it finds
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    //Check if renderer is working
    if(!renderer)
    {
        cout << "Renderer failed: " << SDL_GetError();
    }

    //Allows window to load .png and .jpg images
    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if ((initted & flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
}

//For loading image to the window
SDL_Texture *loadImages(char* imageFile, SDL_Renderer* renderer)
{
    //A variable to pass the image to SDL
    SDL_Texture *Image;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", imageFile);

    Image = IMG_LoadTexture(renderer, imageFile);

    return Image;
}

//sets image location 
void imagePos(SDL_Texture* image, int x, int y, int w, int h, SDL_Renderer* renderer)
{
    SDL_Rect dest;

    //Where the image goes
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    //Query the attributes of a texture
    //Takes image, Format(just set to NULL), Access(Also set to NULL), width and height
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);

    //Takes renderer, texture, NULL to copy whole image, &dest to know where to draw the image 
    SDL_RenderCopy(renderer, image, NULL, &dest);
}

void makeVisuals(SDL_Renderer* renderer)
{
    //Color for background         0, 0, 0, 0, is for black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void showVisuals(SDL_Renderer* renderer)
{
    SDL_RenderPresent(renderer);
}

void readInput(user& player, thing& bullet, thing& bullet2, int playerUp, int playerDown, int playerLeft, int playerRight, int playerFired)
{
    if (playerUp)
    {
	    player.y -= player.speed;
	    player.direction = 1;
    }
    if (playerDown)
    {
	    player.y += player.speed;
	    player.direction = 2;
    }
    if (playerLeft)
    {
	    player.x -= player.speed;
	    player.direction = 3;
    }
    if (playerRight)
    {			
	    player.x += player.speed;
	    player.direction = 4;
    }
    if (playerFired && bullet.health == 0)
    {
	    bullet.x = player.x;
	    bullet.y = player.y;
	    bullet.health = 1;
	    bullet.speed = 1;
    }
    else if (playerFired && bullet2.health == 0 && bullet.health == 1)
    {
	    bullet2.x = player.x;
	    bullet2.y = player.y;
	    bullet2.health = 1;
	    bullet2.speed += bullet2.speed;
    }
    else if(playerFired && bullet2.health > 0 && bullet.health > 0)
    {
	    bullet.speed += bullet.speed;
	    bullet2.speed += bullet2.speed;
    }
}

int noEscape(user player, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    // This keeps the player on the screen, remember [x, y] where y is upside down
    if (player.x < 0)
    {
	return 1;
    }
    else if (player.x > SCREEN_WIDTH-100)
    {
	return 2;
    }
    if (player.y < 0)
    {
	return 3;
    }
    else if (player.y > SCREEN_HEIGHT-100)
    {
	return 4;
    }
    else
    {
        return 0;
    }
}
void noEscapeExec(user& player, int escape)
{
    if (escape == 1)
    {
        player.x += player.back;
    }
    else if (escape == 2)
    {
        player.x -= player.back;
    }
    else if (escape == 3)
    {
        player.y += player.back;
    }
    else if (escape == 4)
    {
        player.y -= player.back;
    }
}

void bulletLogic(thing& bullet, user player, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    if(player.direction == 1)
    {
        bullet.y -= bullet.speed;
    }
    else if(player.direction == 2)
    {
        bullet.y += bullet.speed;
    }
    else if(player.direction == 3)
    {
        bullet.x -= bullet.speed;
    }
    else if(player.direction == 4)
    {
        bullet.x += bullet.speed;
    }
            
    if (bullet.x > SCREEN_WIDTH)
    {
        bullet.health = 0;
    }
    else if (bullet.x < 0)
    {
        bullet.health = 0;
    }
    else if (bullet.y > SCREEN_HEIGHT)
    {
        bullet.health = 0;
    }
    else if (bullet.y < 0)
    {
        bullet.health = 0;
    }
}
void thingLogic(thing& bullet, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    if (bullet.x > SCREEN_WIDTH)
    {
        bullet.health = 0;
	cout << "thing" << " is gone\n";
    }
    else if (bullet.x < 0)
    {
        bullet.health = 0;
	cout << "thing" << " is gone\n";
    }
    else if (bullet.y > SCREEN_HEIGHT)
    {
        bullet.health = 0;
	cout << "thing" << " is gone\n";
    }
    else if (bullet.y < 0)
    {
        bullet.health = 0;
	cout << "thing" << " is gone\n";
    }
}

void enemys(thing& enemy, int& enemySpawnTimer)
{
    if(enemySpawnTimer <= 0 && enemy.health == 0)
    {
	cout << "Spawning enemy\n";

        enemy.x = 1280;
	enemy.y = rand() % 620;

	enemy.speed = rand() % 15;

	if(enemy.speed == 0)
	{
	    enemy.speed += 1;
	}

	enemy.health = 1;

	enemy.x -= rand() % 10;

	enemySpawnTimer = rand() % 100;
    }
    else
    {
        enemy.x -= enemy.speed;
        imagePos(enemy.texture, enemy.x, enemy.y, enemy.w, enemy.h, renderer);

        enemySpawnTimer--;
    }
}

//Takes two objects dimetions
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return (max(x1, x2) < min(x1 + w1, x2 + w2)) && (max(y1, y2) < min(y1 + h1, y2 + h2));
}

void didBulletHit(thing& bullet, thing& enemy)
{
    if(collision(bullet.x, bullet.y, bullet.w, bullet.h, enemy.x, enemy.y, enemy.w, enemy.h))
    {
        enemy.health -= 1;
	bullet.health -= 1;
    }
}

void didEnemyKill(user& player, thing& enemy)
{
    if(collision(player.x, player.y, player.w, player.h, enemy.x, enemy.y, enemy.w, enemy.h))
    {
        enemy.health -= 1;
	player.health -= 1;
	cout << "Health is:" << player.health << "\n";
    }
}

int main(int argc, char* args[])
{
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    user player(100, 100, 90, 90, 10, 10, loadImages("images/Player.png", renderer), 100, 1);
    thing bullet(1000, 1000, 22, 22, 0, 1, loadImages("images/bullet.png", renderer));
    thing bullet2(1000, 1000, 22, 22, 0, 1, loadImages("images/bullet.png", renderer));
    thing enemy(2000, 2000, 90, 90, 0, 0, loadImages("images/enemy.png", renderer));

    while (1)
    {
        makeVisuals(renderer);

	int escape = noEscape(player, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (escape == 0)
	{
	    input(*pPlayerUp, *pPlayerDown, *pPlayerLeft, *pPlayerRight, *pPlayerFired);
	    readInput(player, bullet, bullet2, playerUp, playerDown, playerLeft, playerRight, playerFired);

	    bulletLogic(bullet, player, SCREEN_WIDTH, SCREEN_HEIGHT);
	    bulletLogic(bullet2, player, SCREEN_WIDTH, SCREEN_HEIGHT);

	    if (bullet.health > 0)
            {
	        imagePos(bullet.texture, bullet.x, bullet.y, bullet.w, bullet.h, renderer);
	    }
	    if (bullet2.health > 0)
	    {
	        imagePos(bullet2.texture, bullet2.x, bullet2.y, bullet2.w, bullet2.h, renderer);
	    }
	}
	else
	{
	    noEscapeExec(player, escape);
	}

        enemys(enemy, enemySpawnTimer);
	thingLogic(enemy, SCREEN_WIDTH, SCREEN_HEIGHT);

	didBulletHit(bullet, enemy);
	didBulletHit(bullet2, enemy);

	didEnemyKill(player, enemy);

	if(enemy.health > 0)
	{
	    imagePos(enemy.texture, enemy.x, enemy.y, enemy.w, enemy.h, renderer);
	}
	
	if(player.health > 0)
	{
	    imagePos(player.texture, player.x, player.y, player.w, player.h, renderer);
	}
	else
	{
	    exit(0);
	    cout << "You Died";
	}

	showVisuals(renderer);

        //Delay is in milliseconds so its .10 of a second 
	SDL_Delay(10);
    }
    return 0;
}
