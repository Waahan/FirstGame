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
#include "headerVectorMath.h"
#include "headerAppleTree.h"

int main()
{
    {
        appleTree::tree world{vector2f{0.0, -9.8}};

        appleTree::apple floor{vector2f{0.0, -10}, appleTree::appleType::staticApple, 100.0f};

        appleTree::squareCollider floorCollider{100, 100};
        floor.setCollider(&floorCollider);
        world.addApple(&floor);

        appleTree::apple object{vector2f{0.0, 0.0}, appleTree::appleType::dynamicApple, 10.0f};

        appleTree::squareCollider objectCollider{10, 10};
        object.setCollider(&objectCollider);
        world.addApple(&object);

        for(int i = 0; i < 100; i++)
        {
            world.simulate(0.1f);

            std::cout << "Object pos: " << object.getPosition() << std::endl;
            std::cout << "Floor pos: " << floor.getPosition() << std::endl;
            std::cin.get();
        }
    }

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
