#include <random>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "NPCs.h"
using namespace std;

void enemys(thing& enemy, int& enemySpawnTimer)
{
    enemy.texture = loadImages("images/enemy.png");
    if(enemySpawnTimer <= 0 && enemy.health == 0)
    {
	cout << "Spawning enemy\n";
        enemy.x = 1280;
	cout << "x:" << enemy.x;
	enemy.y = rand() % 620;
	cout << "y:" << enemy.y;
	enemy.speed = 10;
	cout << "\n";
	enemy.health = 1;

	enemy.x -= enemy.speed;

	enemySpawnTimer = rand() % 100;
    }
    else
    {
	enemy.x -= enemy.speed;
	cout << "x:" << enemy.x << "\n";
	imagePos(enemy.texture, enemy.x, enemy.y);
        enemySpawnTimer--;
    }
}
