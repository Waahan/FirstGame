#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <algorithm>
#include <future>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "headerVisuals.h"
#include "headerPlayer.h"

int main()
{
    user player(100, 100, 90, 90, 3, 10, "images/Player.png", 30);

    enemys enemy(2000, 2000, 90, 90, 0, 0, "images/enemy.png");

    points point(2000, 2000, 40, 40, 0, 0, "images/points.png");

    thing background(0, 0, App::get().SCREEN_WIDTH, App::get().SCREEN_HEIGHT, 10, 0, "images/Background.png");

    Messages Score("Score", 0, 0, 100, 100, colour::green);

    {
        Messages Title("Sus invaders", 0, 0, 500, 500, colour::red);
        Messages Start("Enter to start", App::get().SCREEN_WIDTH/2, App::get().SCREEN_HEIGHT/2, 500, 100, colour::white);
        Messages Controls("W:up A:left S:down D:right SPACE:fire", 100, 500, 1000, 100, colour::red);

        int startTimer = 0;
        bool start = false;

        while(!start)
        {
            App::get().makeVisuals();

	    Start.show();
            Controls.show();
	    Title.show();
	
	    if(startTimer > 100)
	    {
                startTimer = 0;

                Title.nextInRainbow();
	    }

	    startTimer++;

	    player.menuInput(start);

	    App::get().showVisuals();
        }
    }

    while (true)
    {
        App::get().makeVisuals();

	background.show();

	Score.setMessage(player.playerScore.stringCurrentCount());

	player.input();

	enemy.logic();
	player.logic(enemy, point);

	point.initPoints();

        enemy.spawnEnemys();

	enemy.scaleDifficulty(player.playerScore);
	
        Score.show();

	enemy.didEnemyKill(player);
	
	enemy.makeEnd(player);

	enemy.show();

        point.show();
	
	player.show();

	App::get().showVisuals();

        //Delay is in milliseconds so its .10 of a second 
	SDL_Delay(10);
    }

    return 0;
}
