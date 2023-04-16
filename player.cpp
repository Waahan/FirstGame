#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "headerPlayer.h"
#include "headerVisuals.h"
using namespace std;

thing::thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture)
{
    x = ix;
    y = iy;
    w = iw;
    h = ih;
    health = ihealth;
    speed = ispeed;
    texture = itexture;
}

user::user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, int iback, int idirection) : thing(ix, iy, iw, ih, ihealth, ispeed, itexture)
{
	back = iback;
	direction = idirection;
}

//For keyboard event detection. More scan codes at https://wiki.libsdl.org/SDL2/SDL_Scancode
void doKeyDown(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired)
{
    //Ignores keyboard repeat events
    if(event->repeat == 0)
    {
	//For using the arrow keys
	
	//"Up" (the Up arrow key (navigation keypad))
        if(event->keysym.scancode == SDL_SCANCODE_UP)
	{
	    playerUp = 1;
	}
	//"Down" (the Down arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_DOWN)
	{
	    playerDown = 1;
	}
	//"Left" (the Left arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_LEFT)
	{
	    playerLeft = 1;
	}
	//"Right" (the Right arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_RIGHT)
	{
	    playerRight = 1;
	}

	//For using WASD
	
	//"Up" (the Up arrow key (navigation keypad))
        if(event->keysym.scancode == SDL_SCANCODE_W)
	{
	    playerUp = 1;
	}
	//"Down" (the Down arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_S)
	{
	    playerDown = 1;
	}
	//"Left" (the Left arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_A)
	{
	    playerLeft = 1;
	}
	//"Right" (the Right arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_D)
	{
	    playerRight = 1;
	}

	//For player firing
	if(event->keysym.scancode == SDL_SCANCODE_SPACE)
        {
	    playerFired = 1;
	}

    }
}

void doKeyUp(SDL_KeyboardEvent *event, int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired)
{
    //Ignores keyboard repeat events
    if(event->repeat == 0)
    {
	//For using the arrow keys
	
	//"Up" (the Up arrow key (navigation keypad))
        if(event->keysym.scancode == SDL_SCANCODE_UP)
	{
	    playerUp = 0;
	}
	//"Down" (the Down arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_DOWN)
	{
	    playerDown = 0;
	}
	//"Left" (the Left arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_LEFT)
	{
	    playerLeft = 0;
	}
	//"Right" (the Right arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_RIGHT)
	{
	    playerRight = 0;
	}

	//For using WASD
	
	//"Up" (the Up arrow key (navigation keypad))
        if(event->keysym.scancode == SDL_SCANCODE_W)
	{
	    playerUp = 0;
	}
	//"Down" (the Down arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_S)
	{
	    playerDown = 0;
	}
	//"Left" (the Left arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_A)
	{
	    playerLeft = 0;
	}
	//"Right" (the Right arrow key (navigation keypad))
	if(event->keysym.scancode == SDL_SCANCODE_D)
	{
	    playerRight = 0;
	}

	//For firing
	if(event->keysym.scancode == SDL_SCANCODE_SPACE)
	{
	     playerFired = 0;
	}

    }
}


void input(user& player, thing& bullet, thing& bullet2, App app, int& playerUp, int& playerDown, int& playerLeft, int& playerRight, int& playerFired)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
	    case SDL_QUIT:
                exit(0);
		break;

	    case SDL_KEYDOWN:
		doKeyDown(&event.key, playerUp, playerDown, playerLeft, playerRight, playerFired);
		break;

	    case SDL_KEYUP:
		doKeyUp(&event.key, playerUp, playerDown, playerLeft, playerRight, playerFired);
		break;

	    default:
		break;
	}
    }
    if (playerUp)
    {
            player.y -= player.speed;
            player.direction = 1;
	    player.texture = app.loadImages("images/PlayerUp.png");
    }
    if (playerDown)
    {
            player.y += player.speed;
            player.direction = 2;
	    player.texture = app.loadImages("images/PlayerDown.png");
    }
    if (playerLeft)
    {
            player.x -= player.speed;
            player.direction = 3;
	    player.texture = app.loadImages("images/PlayerLeft.png");
    }
    if (playerRight)
    {
            player.x += player.speed;
            player.direction = 4;
	    player.texture = app.loadImages("images/PlayerRight.png");
    }
    if (playerFired && bullet.health == 0)
    {
            bullet.x = player.x;
            bullet.y = player.y;
            bullet.health = 1;
            bullet.speed = player.speed/2;
	    player.texture = app.loadImages("images/Player.png");
    }
    else if (playerFired && bullet2.health == 0 && bullet.health == 1)
    {
            bullet2.x = player.x;
            bullet2.y = player.y;
            bullet2.health = 1;
            bullet2.speed = player.speed/2;
	    player.texture = app.loadImages("images/Player.png");
    }
    else if(playerFired && bullet2.health > 0 && bullet.health > 0)
    {
            if(bullet.speed < 65535)
            {
                bullet.speed += bullet.speed;
            }
            else
            {
                bullet.speed = 20;
            }
            if(bullet2.speed < 65535)
            {
                bullet2.speed += bullet2.speed;
            }
            else
            {
                bullet2.speed = 20;
            }
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

void enemys(thing& enemy, int& enemySpawnTimer, App app)
{
    if(enemySpawnTimer <= 0 && enemy.health == 0)
    {
       cout << "Spawning enemy\n";

       enemy.x = 1280;
       enemy.y = rand() % 620;

       enemy.speed = 1 + (rand() % 15);

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
        app.imagePos(enemy.texture, enemy.x, enemy.y, enemy.w, enemy.h);

        enemySpawnTimer--;
    }
}



//Takes two objects dimetions
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
        return (max(x1, x2) < min(x1 + w1, x2 + w2)) && (max(y1, y2) < min(y1 + h1, y2 + h2));
}

void didBulletHit(thing& bullet, thing& enemy, long long int& counter)
{
    if(collision(bullet.x, bullet.y, bullet.w, bullet.h, enemy.x, enemy.y, enemy.w, enemy.h))
    {
        enemy.health = 0;
        bullet.health -= 1;
	counter++;
	cout << "Count: " << counter << "\n";
    }
}

void didEnemyKill(user& player, thing& enemy)
{
    if(collision(player.x, player.y, player.w, player.h, enemy.x, enemy.y, enemy.w, enemy.h))
    {
        enemy.health = 0;
        player.health -= 1;
        cout << "Health is:" << player.health << "\n";
    }
}

