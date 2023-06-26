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
#include "headerPlayer.h"

int main(int argc, char* args[])
{
    int levelOne = 0;
    int startTimer = 0;
    bool start = false;

    enum class color: unsigned char { red, orange, yellow, green, blue, indigo, violet };
    color myColor;

    App app(1280, 720);

    user player(100, 100, 90, 90, 3, 10, app.loadImages("images/Player.png"), &app, 30, 1);

    enemys enemy(2000, 2000, 90, 90, 0, 0, app.loadImages("images/enemy.png"), &app);

    points point(2000, 2000, 40, 40, 0, 0, app.loadImages("images/points.png"), &app);

    thing background(0, 0, app.SCREEN_WIDTH, app.SCREEN_HEIGHT, 10, 0, app.loadImages("images/Background.png"), &app);

    Messages Score("Score", 0, 0, 100, 100, app, Score.Green);
    Messages Title("Sus invaders", 0, 0, 500, 500, app, Score.White);
    Messages Start("Enter to start", app.SCREEN_WIDTH/2, app.SCREEN_HEIGHT/2, 500, 100, app, Score.White);
    Messages Controls("W:up A:left S:down D:right SPACE:fire", 100, 500, 1000, 100, app, Score.Red);

    while(!start)
    {
        app.makeVisuals();

	Start.drawMessage(app);
	Controls.drawMessage(app);
	Title.drawMessage(app);
	
	if(startTimer > 100)
	{
	    startTimer = 0;

	    switch(myColor)
	    {
		case color::red:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, app, Title.Red);
		    myColor = color::orange;
		    break;
		case color::orange:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, app, Title.Orange);
		    myColor = color::yellow;
		    break;
		case color::yellow:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, app, Title.Yellow);
		    myColor = color::green;
		    break;
		case color::green:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, app, Title.Green);
		    myColor = color::blue;
		    break;
		case color::blue:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, app, Title.Blue);
		    myColor = color::indigo;
		    break;
		case color::indigo:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, app, Title.Indigo);
		    myColor = color::violet;
		    break;
		case color::violet:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, app, Title.Violet);
		    myColor = color::red;
                    break;
                default:
                    myColor = color::red;
	    }
            
	}

	startTimer++;

	player.menuInput(start);

	app.showVisuals();
    }

    while (true)
    {
        app.makeVisuals();

	background.show();

	Score.newMessage(player.playerScore.stringCurrentCount().c_str(), 0, 0, 100, 100, app, Score.Green);

	player.input();

	enemy.logic();
	player.logic(enemy, point);

	point.initPoints();

        enemy.spawnEnemys();

	enemy.scaleDifficulty(player.playerScore);
	Score.drawMessage(app);

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
