//Header for NPCs.cpp
#pragma once
#ifndef NPCs_H
#define NPCs_H
#include "player.h"


void enemys(thing& enemy, int& enemySpawnTimer);

//Things from main I need
void imagePos(SDL_Texture texture, int x, int y);
SDL_Texture *loadImages(char* imageFile);

#endif
