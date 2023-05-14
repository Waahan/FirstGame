#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <climits>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "headerVisuals.h"
#include "headerPlayer.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

int main(int argc, char* args[])
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    int enemySpawnTimer = 0;
    int levelOne = 0;
    int counter = 0;
    int oldCounter = 0;
    bool start = false;
    int color = 0;
    int startTimer = 0;

    App app(SCREEN_WIDTH, SCREEN_HEIGHT);

    string playerPath = "images/Player.png", bulletPath = "images/bullet.png", enemyPath = "images/enemy.png", pointPath = "images/points.png", backgroundPath = "images/Background.png", healthPath1 = "images/Health1.jpg", healthPath2 = "images/Health2.jpg", healthPath3 = "images/Health3.jpg";

    user player(100, 100, 90, 90, 10, 10, app.loadImages(playerPath.c_str()), 30, 1);
    bulletClass bullet(1000, 1000, 22, 22, 0, 1, app.loadImages(bulletPath.c_str()));
    bulletClass bullet2(1000, 1000, 22, 22, 0, 1, app.loadImages(bulletPath.c_str()));
    enemys enemy(2000, 2000, 90, 90, 0, 0, app.loadImages(enemyPath.c_str()));
    points point(2000, 2000, 40, 40, 0, 0, app.loadImages(pointPath.c_str()));
    thing background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 10, 0, app.loadImages(backgroundPath.c_str()));
    Messages Score("Score", 0, 0, 100, 100, app);
    Messages Title("Sus invaders", 0, 0, 500, 500, Score.Blue, app);
    Messages Start("Enter to start", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 500, 100, app);
    Messages Controls("W:up A:left S:down D:right SPACE:fire", 100, 500, 1000, 100, Score.Red, app);
    healthDisplay playerHealthDisplay(app.loadImages(healthPath1.c_str()), app.loadImages(healthPath2.c_str()), app.loadImages(healthPath3.c_str()));

    while(!start)
    {
        app.makeVisuals();

	Start.drawMessage(app);
	Controls.drawMessage(app);
	Title.drawMessage(app);
	
	if(startTimer > 100)
	{
	    startTimer = 0;

	    if(color == 0)
	    {
	        Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Red, app);
	    }
	    else if(color == 1)
	    {
	        Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Orange, app);
	    }
	    else if(color == 2)
	    {
	        Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Yellow, app);
	    }
	    else if(color == 3)
	    {
	        Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Green, app);
	    } 
	    else if(color == 4)
	    {
	        Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Blue, app);
	    }
	    else if(color == 5)
	    {
	        Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Indigo, app);
	    }
	    else if(color == 6)
	    {
	        Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Violet, app);
	    }
	    else if(color > 6)
	    {
	        color = 0;
	    }

	    if(color < 7)
	    {
	        color++;
	    }
	}

	startTimer++;

	player.menuInput(start);

	app.showVisuals();
    }

    while (1)
    {
        app.makeVisuals();

	app.imagePos(background.texture, background.x, background.y, background.w, background.h);

	if (oldCounter != counter)
	{
	    string counterString = "Score:"+to_string(counter);
	    Score.newMessage(counterString.c_str(), 0, 0, 100, 100, app);

	    oldCounter = counter;
	}
	else
	{
	    oldCounter = counter;
	}

	player.input(bullet, bullet2, app);

        bullet.logic(player, SCREEN_WIDTH, SCREEN_HEIGHT);
	bullet2.logic(player, SCREEN_WIDTH, SCREEN_HEIGHT);
	enemy.logic(SCREEN_WIDTH, SCREEN_HEIGHT);
	player.logic(SCREEN_WIDTH, SCREEN_HEIGHT);

	point.initPoints(SCREEN_WIDTH, SCREEN_HEIGHT);
        enemy.spawnEnemys(enemySpawnTimer, player);
	enemy.scaleDifficulty(counter);
	Score.drawMessage(app);

        bullet.didBulletHit(enemy, counter);
        bullet2.didBulletHit(enemy, counter);
	enemy.didEnemyKill(player, app);
	point.didYouGetPoints(player, bullet, counter, app);

	if(counter > 500 && levelOne == 0)
	{
	    enemy.makeEnd(levelOne, app);
	}
	else if(levelOne == 1)
	{ 
	    SDL_DestroyTexture(enemy.texture);
	    enemy.texture = app.loadImages("images/enemy.png");
	    levelOne += 1;
	}

	if(enemy.health > 0)
	{
	    app.imagePos(enemy.texture, enemy.x, enemy.y, enemy.w, enemy.h);
	}

        if (bullet.health > 0)
        {
            app.imagePos(bullet.texture, bullet.x, bullet.y, bullet.w, bullet.h);
        }

        if (bullet2.health > 0)
        {
            app.imagePos(bullet2.texture, bullet2.x, bullet2.y, bullet2.w, bullet2.h);
        }

        if (point.health > 0)
        {
            app.imagePos(point.texture, point.x, point.y, point.w, point.h);
        }
	
	if(player.health > 0)
	{
	    app.imagePos(player.texture, player.x, player.y, player.w, player.h);
	}
	else
	{
	    thing deathImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 10, 0, app.loadImages("images/Death.jpg"));

	    app.imagePos(deathImage.texture, deathImage.x, deathImage.y, deathImage.w, deathImage.h);

	    app.showVisuals();

	    SDL_Delay(6000);

	    exit(0);
	    return 0;
	}

	SDL_Texture* loadHealthDisplay = playerHealthDisplay.healthDisplayUpdate(player);
	app.imagePos(loadHealthDisplay, 100, 0, 10, 10);

	app.showVisuals();

        //Delay is in milliseconds so its .10 of a second 
	SDL_Delay(10);
    }

    return 0;
}
