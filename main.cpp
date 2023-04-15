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

//For the player movement
int playerUp = 0;
int playerDown = 0;
int playerLeft = 0;
int playerRight = 0;
int playerFired = 0;

int main(int argc, char* args[])
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    int enemySpawnTimer = 0;

    App app(SCREEN_WIDTH, SCREEN_HEIGHT);

    user player(100, 100, 90, 90, 10, 10, app.loadImages("images/Player.png"), 100, 1);
    thing bullet(1000, 1000, 22, 22, 0, 1, app.loadImages("images/bullet.png"));
    thing bullet2(1000, 1000, 22, 22, 0, 1, app.loadImages("images/bullet.png"));
    thing enemy(2000, 2000, 90, 90, 0, 0, app.loadImages("images/enemy.png"));

    while (1)
    {
        app.makeVisuals();

	int escape = noEscape(player, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (escape == 0)
	{
	    input(player, bullet, bullet2, playerUp, playerDown, playerLeft, playerRight, playerFired);

	    bulletLogic(bullet, player, SCREEN_WIDTH, SCREEN_HEIGHT);
	    bulletLogic(bullet2, player, SCREEN_WIDTH, SCREEN_HEIGHT);

	    if (bullet.health > 0)
            {
	        app.imagePos(bullet.texture, bullet.x, bullet.y, bullet.w, bullet.h);
	    }
	    if (bullet2.health > 0)
	    {
	        app.imagePos(bullet2.texture, bullet2.x, bullet2.y, bullet2.w, bullet2.h);
	    }
	}
	else
	{
	    noEscapeExec(player, escape);
	}

        enemys(enemy, enemySpawnTimer, app);

        didBulletHit(bullet, enemy);
        didBulletHit(bullet2, enemy);

	thingLogic(enemy, SCREEN_WIDTH, SCREEN_HEIGHT);

	didEnemyKill(player, enemy);

	if(enemy.health > 0)
	{
	    app.imagePos(enemy.texture, enemy.x, enemy.y, enemy.w, enemy.h);
	}
	
	if(player.health > 0)
	{
	    app.imagePos(player.texture, player.x, player.y, player.w, player.h);
	}
	else
	{
	    exit(0);
	    cout << "You Died";
	}

	app.showVisuals();

        //Delay is in milliseconds so its .10 of a second 
	SDL_Delay(10);
    }
    return 0;
}
