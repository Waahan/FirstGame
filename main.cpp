#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <future>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "headerVisuals.h"
#include "headerPlayer.h"

int main(int argc, char* args[])
{
    int levelOne = 0;
    int startTimer = 0;
    bool start = false;

    App app(1280, 720);

    user player(100, 100, 90, 90, 3, 10, app.loadImages("images/Player.png"), &app, 30, 1);

    enemys enemy(2000, 2000, 90, 90, 0, 0, app.loadImages("images/enemy.png"), &app);

    points point(2000, 2000, 40, 40, 0, 0, app.loadImages("images/points.png"), &app);

    thing background(0, 0, app.SCREEN_WIDTH, app.SCREEN_HEIGHT, 10, 0, app.loadImages("images/Background.png"), &app);

    Messages Score("Score", 0, 0, 100, 100, &app, Messages::color::green);
    Messages Title("Sus invaders", 0, 0, 500, 500, &app);
    Messages Start("Enter to start", app.SCREEN_WIDTH/2, app.SCREEN_HEIGHT/2, 500, 100, &app);
    Messages Controls("W:up A:left S:down D:right SPACE:fire", 100, 500, 1000, 100, &app, Messages::color::red);

    while(!start)
    {
        app.makeVisuals();

	Start.drawMessage();
	Controls.drawMessage();
	Title.drawMessage();
	
	if(startTimer > 100)
	{
	    startTimer = 0;

            Title.rainbowColorSwitch();
	}

	startTimer++;

	player.menuInput(start);

	app.showVisuals();
    }

    while (true)
    {
        app.makeVisuals();

	background.show();

	Score.newMessage(player.playerScore.stringCurrentCount().c_str(), 2345, 2345, 0, 0);

	player.input();

	enemy.logic();
	player.logic(enemy, point);

	point.initPoints();

        enemy.spawnEnemys();

	enemy.scaleDifficulty(player.playerScore);
	Score.drawMessage();

	enemy.didEnemyKill(player);
	
        if(player.playerScore.count() > 400 && levelOne == 0)
	{
	    enemy.makeEnd(levelOne);
	}
	else if(levelOne == 1)
	{ 
	    enemy.newTexture("images/enemy.png");
	    levelOne += 1;
	}

	enemy.show();

        point.show();
	
	player.show();

	app.showVisuals();

        //Delay is in milliseconds so its .10 of a second 
	SDL_Delay(10);
    }

    return 0;
}
