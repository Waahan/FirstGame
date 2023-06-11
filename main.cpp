#include <iostream>
#include <string>
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
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    int enemySpawnTimer = 0;
    int levelOne = 0;
    int counter = 0;
    int oldCounter = 0;
    bool start = false;

    enum class color: unsigned char { red = 0, orange = 1, yellow = 2, green = 3, blue = 4, indigo = 5, violet = 6 };
    color myColor;

    int startTimer = 0;

    App app(SCREEN_WIDTH, SCREEN_HEIGHT);

    user player(100, 100, 90, 90, 10, 10, app.loadImages("images/Player.png"), &app, 30, 1);

    bulletClass bullet(1000, 1000, 22, 22, 0, 1, app.loadImages("images/bullet.png"), &app);
    bulletClass bullet2(1000, 1000, 22, 22, 0, 1, app.loadImages("images/bullet.png"), &app);

    enemys enemy(2000, 2000, 90, 90, 0, 0, app.loadImages("images/enemy.png"), &app);

    points point(2000, 2000, 40, 40, 0, 0, app.loadImages("images/points.png"), &app);

    thing background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 10, 0, app.loadImages("images/Background.png"), &app);

    Messages Score("Score", 0, 0, 100, 100, app);
    Messages Title("Sus invaders", 0, 0, 500, 500, Score.Blue, app);
    Messages Start("Enter to start", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 500, 100, app);
    Messages Controls("W:up A:left S:down D:right SPACE:fire", 100, 500, 1000, 100, Score.Red, app);

    healthDisplay playerHealthDisplay(app.loadImages("images/Health1.jpg"), app.loadImages("images/Health2.jpg"), app.loadImages("images/Health3.jpg"));

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
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Red, app);
		    myColor = color::orange;
		    break;
		case color::orange:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Orange, app);
		    myColor = color::yellow;
		    break;
		case color::yellow:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Yellow, app);
		    myColor = color::green;
		    break;
		case color::green:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Green, app);
		    myColor = color::blue;
		    break;
		case color::blue:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Blue, app);
		    myColor = color::indigo;
		    break;
		case color::indigo:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Indigo, app);
		    myColor = color::violet;
		    break;
		case color::violet:
		    Title.newMessage("Sus invaders", 0, 0, 500, 500, Title.Violet, app);
		    myColor = color::red;
                    break;
	    }
            
	}

	startTimer++;

	player.menuInput(start);

	app.showVisuals();
    }

    while (true)
    {
        app.makeVisuals();

	app.imagePos(background.texture, background.x, background.y, background.w, background.h);

	if (oldCounter != counter)
	{
	    std::string counterString = "Score:"+std::__cxx11::to_string(counter);
	    Score.newMessage(counterString.c_str(), 0, 0, 100, 100, app);

	    oldCounter = counter;
	}
	else
	{
	    oldCounter = counter;
	}

	player.input(bullet, bullet2);

        bullet.logic(player, SCREEN_WIDTH, SCREEN_HEIGHT);
	bullet2.logic(player, SCREEN_WIDTH, SCREEN_HEIGHT);
	enemy.logic(SCREEN_WIDTH, SCREEN_HEIGHT);
	player.logic(SCREEN_WIDTH, SCREEN_HEIGHT);

	point.initPoints(SCREEN_WIDTH, SCREEN_HEIGHT);

        auto spawnEnemys = std::async(std::launch::async, [&enemy, &enemySpawnTimer, &player](){ enemy.spawnEnemys(enemySpawnTimer, player); } );

	enemy.scaleDifficulty(counter);
	Score.drawMessage(app);

        bullet.didBulletHit(enemy, counter);
        bullet2.didBulletHit(enemy, counter);
	enemy.didEnemyKill(player);
	point.didYouGetPoints(player, bullet, counter);

	if(counter > 400 && levelOne == 0)
	{
	    enemy.makeEnd(levelOne);
	}
	else if(levelOne == 1)
	{ 
	    enemy.newTexture("images/enemy.png");
	    levelOne += 1;
	}

	spawnEnemys.get();

	enemy.show();

        bullet.show();

        bullet2.show();

        point.show();
	
	if(!player.show())
	{
	    player.playerDeath(SCREEN_WIDTH, SCREEN_HEIGHT);
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
