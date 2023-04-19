#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
    long long int counter = 0;

    App app(SCREEN_WIDTH, SCREEN_HEIGHT);

    user player(100, 100, 90, 90, 10, 10, app.loadImages("images/Player.png"), 100, 1);
    thing bullet(1000, 1000, 22, 22, 0, 1, app.loadImages("images/bullet.png"));
    thing bullet2(1000, 1000, 22, 22, 0, 1, app.loadImages("images/bullet.png"));
    thing enemy(2000, 2000, 90, 90, 0, 0, app.loadImages("images/enemy.png"));
    points point(2000, 2000, 40, 40, 0, 0, app.loadImages("images/points.png"));
    thing background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 10, 0, app.loadImages("images/Background.png"));

    while (1)
    {
        app.makeVisuals();

	app.imagePos(background.texture, background.x, background.y, background.w, background.h);

	noEscape(player, SCREEN_WIDTH, SCREEN_HEIGHT);

	player.input(bullet, bullet2, app);

        bulletLogic(player, bullet, SCREEN_WIDTH, SCREEN_HEIGHT);
	bulletLogic(player, bullet2, SCREEN_WIDTH, SCREEN_HEIGHT);

	point.initPoints(SCREEN_WIDTH, SCREEN_HEIGHT);

        enemys(enemy, enemySpawnTimer, app);

        didBulletHit(bullet, enemy, counter);
        didBulletHit(bullet2, enemy, counter);
	didYouGetPoints(player, bullet, point, counter);

	if(counter > 100 && levelOne == 0)
	{
	    enemy.texture = app.loadImages("images/secretEnd.gif");
	    enemy.speed = 1;
	    levelOne = 1;

	    if(enemy.x > SCREEN_WIDTH/2)
	    {
	        enemy.speed = 0;
	    }

	    app.imagePos(enemy.texture, enemy.x, enemy.y, enemy.w, enemy.h);

	    app.showVisuals();

	    SDL_Delay(60000);
	}
	else if(levelOne == 1)
	{
	    enemy.texture = app.loadImages("images/enemy.png");
	    levelOne += 1;
	}

	enemy.logic(SCREEN_WIDTH, SCREEN_HEIGHT);

	didEnemyKill(player, enemy);

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
	}

	app.showVisuals();

        //Delay is in milliseconds so its .10 of a second 
	SDL_Delay(10);
    }
    return 0;
}
