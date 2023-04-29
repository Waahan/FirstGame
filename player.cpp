#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <fstream>
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
points::points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture) : thing(ix, iy, iw, ih, ihealth, ispeed, itexture){}

void points::initPoints(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    if (health > 0)
    {
	x -= speed;

        if (x > SCREEN_WIDTH)
        {
            health = 0;
        }
        else if (x < 0)
        {
            health = 0;
        }
        else if (y > SCREEN_HEIGHT)
        {
            health = 0;
        }
        else if (y < 0)
        {
            health = 0;
        }
    }
    else
    {
	x = SCREEN_WIDTH;
	y = rand() % SCREEN_HEIGHT;

	int randomNum = 1 + (rand() % 9);
	health = randomNum;
	speed = randomNum;
    }
}

//For keyboard event detection. More scan codes at https://wiki.libsdl.org/SDL2/SDL_Scancode
void user::doKeyDown(SDL_KeyboardEvent *event)
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

void user::doKeyUp(SDL_KeyboardEvent *event)
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


void user::input(thing& bullet, thing& bullet2, App app)
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
		doKeyDown(&event.key);
		break;

	    case SDL_KEYUP:
		doKeyUp(&event.key);
		break;

	    default:
		break;
	}
    }
    if (playerUp)
    {
            y -= speed;
            direction = 1;
	    texture = app.loadImages("images/PlayerUp.png");
    }
    if (playerDown)
    {
            y += speed;
            direction = 2;
	    texture = app.loadImages("images/PlayerDown.png");
    }
    if (playerLeft)
    {
            x -= speed;
            direction = 3;
	    texture = app.loadImages("images/PlayerLeft.png");
    }
    if (playerRight)
    {
            x += speed;
            direction = 4;
	    texture = app.loadImages("images/PlayerRight.png");
    }
    if (playerFired && bullet.health == 0)
    {
            bullet.x = x;
            bullet.y = y;
            bullet.health = 1;
            bullet.speed = speed/2;
	    texture = app.loadImages("images/Player.png");
    }
    else if (playerFired && bullet2.health == 0 && bullet.health == 1)
    {
            bullet2.x = x;
            bullet2.y = y;
            bullet2.health = 1;
            bullet2.speed = speed/2;
	    texture = app.loadImages("images/Player.png");
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

void user::logic(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    // This keeps the player on the screen, remember [x, y] where y is upside down
    if (x < 0)
    {
        x += back;
    }
    else if (x > SCREEN_WIDTH-100)
    {
        x -= back;
    }
    if (y < 0)
    {
        y += back;
    }
    else if (y > SCREEN_HEIGHT-100)
    {
        y -= back;
    }
}

void bulletClass::logic(user player, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    if(player.direction == 1)
    {
        y -= speed;
    }
    else if(player.direction == 2)
    {
        y += speed;
    }
    else if(player.direction == 3)
    {
        x -= speed;
    }
    else if(player.direction == 4)
    {
        x += speed;
    }

    if (x > SCREEN_WIDTH)
    {
        health = 0;
    }
    else if (x < 0)
    {
        health = 0;
    }
    else if (y > SCREEN_HEIGHT)
    {
        health = 0;
    }
    else if (y < 0)
    {
        health = 0;
    }
}

void thing::logic(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    if (x > SCREEN_WIDTH)
    {
        health = 0;
    }
    else if (x < 0)
    {
        health = 0;
    }
    else if (y > SCREEN_HEIGHT)
    {
        health = 0;
    }
    else if (y < 0)
    {
        health = 0;
    }
}

void enemys::spawnEnemys(int& enemySpawnTimer)
{
    if(enemySpawnTimer <= 0 && health == 0)
    {
       cout << "Spawning enemy\n";

       x = 1280;
       y = rand() % 620;

       speed = 1 + (rand() % 15);

       health = 1;

       x -= rand() % 10;

       enemySpawnTimer = rand() % 100;
    }
    else
    {
        x -= speed;
        enemySpawnTimer--;
    }
}

//Takes two objects dimetions
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
        return (max(x1, x2) < min(x1 + w1, x2 + w2)) && (max(y1, y2) < min(y1 + h1, y2 + h2));
}

void bulletClass::didBulletHit(thing& enemy, int& counter)
{
    if(collision(x, y, w, h, enemy.x, enemy.y, enemy.w, enemy.h))
    {
        enemy.health = 0;
        health -= 1;
	counter++;
	cout << "Count: " << counter << "\n";
    }
}

void enemys::didEnemyKill(user& player)
{
    if(collision(player.x, player.y, player.w, player.h, x, y, w, h))
    {
        health = 0;
        player.health -= 1;
        cout << "Health is:" << player.health << "\n";
    }
}

void points::didYouGetPoints(user& player, thing& bullet, int& counter)
{
    if(collision(player.x, player.y, player.w, player.h, x, y, w, h))
    {
	if(health > player.health)
	{
	    player.health += health;
	}
	else
	{
	    counter++;
	}

        health = 0;
	counter++;
    }
    else if(collision(x, y, w, h, bullet.x, bullet.y, bullet.w, bullet.h))
    {
	bullet.health += health;
        health = 0;
	counter++;
    }
}

void enemys::makeEnd(int& levelOne, App app)
{
    texture = app.loadImages("images/secretEnd.gif");
    speed = 1;
    levelOne = 1;

    app.imagePos(texture, x, y, w, h);

    app.showVisuals();

    SDL_Delay(60000);
}

healthDisplay::healthDisplay(SDL_Texture* ifullHealth, SDL_Texture* ihalfHealth, SDL_Texture* icritical)
{
    fullHealth = ifullHealth;
    halfHealth = ihalfHealth;
    critical = icritical;
}

SDL_Texture* healthDisplay::healthDisplayUpdate(user& player)
{
    switch(player.health)
    {
        case 10:
	    return fullHealth;
	case 5:
	    return halfHealth;
	case 1:
	    return critical;
	default:
	    if(player.health > 5)
	    {
	        return fullHealth;
	    }
	    else if(player.health > 1)
	    {
	        return halfHealth;
	    }
    }
}
