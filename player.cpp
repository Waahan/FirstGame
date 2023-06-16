#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <future>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "headerPlayer.h"
#include "headerVisuals.h"

thing::thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer)
{
/*
* thing::thing create a thing with the x y w h texture and app pointer 
* 
* pre and postconditions:
* 
* Precondition iappPointer and itexture can not be NULL
* Precondition iw and ih can not be less or equal to zero 
* Precondition ihealth can not be less than zero
*
* Postcondition constructs a valid thing
* Postcondition itexture is destroyed even if exceptions are thrown 
*/    
    try
    {
        if(iappPointer == NULL || iappPointer == nullptr)
        {
            throw std::invalid_argument("iappPointer can not be NULL");
        }
        else if(itexture == NULL || itexture == nullptr)
        {
            throw std::invalid_argument("itexture can not be NULL");
        }
        else if(iw <= 0 || ih <= 0)
        {
            throw std::invalid_argument("thing w or height can not be less than or equal to zero");
        }
        else if(ihealth < 0)
        {
            throw std::invalid_argument("health can not be less than zero");
        }

        x = ix;
        y = iy;
        w = iw;
        h = ih;
        health = ihealth;
        speed = ispeed;
        texture = itexture;
        appPointer = iappPointer;
    }
    catch(...)
    {
        SDL_DestroyTexture(itexture);
        throw;
    }
}

void thing::logic()
{
/*
* thing::logic sets health equal to zero if thing is outside SCREEN_WIDTH and SCREEN_HEIGHT
*/
    if (x > appPointer->appSCREEN_WIDTH)
    {
        health = 0;
    }
    else if (x < 0)
    {
        health = 0;
    }
    else if (y > appPointer->appSCREEN_HEIGHT)
    {
        health = 0;
    }
    else if (y < 0)
    {
        health = 0;
    }
}

void thing::newTexture(SDL_Texture* newTexture)
{
/*
* thing::newTexture set newTexture to texture 
*
* pre and postconditions:
* 
* Precondition newTexture is not NULL
*/
    if(newTexture == nullptr || newTexture == NULL)
    {
	std::cerr << "newTexture can not be nullptr or 0 or NULL" << std::endl;
	return;
    }

    SDL_DestroyTexture(texture);
    texture = newTexture;
}

void thing::newTexture(const char* newTexturePath)
{
/*
* thing::newTexture load newTexture from the file path 
*
* pre and postconditions
*
* Precondition newTexturePath is not NULL 
*/
    if(newTexturePath == NULL || newTexturePath == nullptr)
    {
        throw std::invalid_argument("newTexturePath can not be NULL");
    }

    SDL_DestroyTexture(texture);
    texture = appPointer->loadImages(newTexturePath);
}

int thing::show()
{
/*
* thing::show use imagePos to display thing on screen
*/
    if(health > 0)
    {
        appPointer->imagePos(texture, x, y, w, h);
        return 1;
    }
    else
    {
        return 0;
    }
}



user::user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer, int iback, int idirection) 
: thing(ix, iy, iw, ih, ihealth, ispeed, itexture, iappPointer)
{
/*
* user::user construct a valid user 
*
* pre and postconditions:
*
* Precondition iw and ih can not be less than or equal to zero handled by thing 
* Precondition ihealth can not be less than zero handled by thing
* Precondition itexture can not be NULL handled by thing
* Precondition iappPointer can not be NULL handled by thing
* Precondition idirection can not be anything other than 1 2 3 or 4 
*/
    try
    {
        if(idirection != 1 && idirection != 2 && idirection != 3 && idirection != 4)
        {
            throw std::invalid_argument("idirection can not be anything other than 1 2 3 or 4");
        }

        back = iback;
        direction = idirection;

        playerHealth = new healthDisplay;
        playerHealth->fullHealth = appPointer->loadImages("images/Health1.jpg");
        playerHealth->halfHealth = appPointer->loadImages("images/Health2.jpg");
        playerHealth->critical = appPointer->loadImages("images/Health3.jpg");
    }
    catch(...)
    {
        SDL_DestroyTexture(itexture);
        throw;
    }
}

user::~user()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(healthDisplayCurrent);

    delete playerHealth;

    for(auto& currentBullet : bullets)
    {
        delete currentBullet;
        currentBullet = nullptr;
        bullets.erase(std::remove(bullets.begin(), bullets.end(), currentBullet), bullets.end());
    }
}

//Scan codes at https://wiki.libsdl.org/SDL2/SDL_Scancode
void user::doKeyDown(SDL_KeyboardEvent *event, bool DownUp)
{
/*
* user::doKeyDown handle event keys by setting playerUp playerDown playerLeft playerRight and playerFired to true or false
* 
* pre and postconditions:
*
* Precondition event can not be NULL
*/
    if(event == NULL || event == nullptr)
    {
        throw std::invalid_argument("Event can not be NULL");
    }

    //Ignores keyboard repeat events
    //if(event->repeat == 0)
    //{
        if(event->keysym.scancode == SDL_SCANCODE_UP || event->keysym.scancode == SDL_SCANCODE_W)
	{
	    playerUp = DownUp;
	}

	if(event->keysym.scancode == SDL_SCANCODE_DOWN || event->keysym.scancode == SDL_SCANCODE_S)
	{
	    playerDown = DownUp;
	}

	if(event->keysym.scancode == SDL_SCANCODE_LEFT || event->keysym.scancode == SDL_SCANCODE_A)
	{
	    playerLeft = DownUp;
	}

	if(event->keysym.scancode == SDL_SCANCODE_RIGHT || event->keysym.scancode == SDL_SCANCODE_D)
	{
	    playerRight = DownUp;
	}

	if(event->keysym.scancode == SDL_SCANCODE_SPACE)
        {
	    playerFired = DownUp;
	}
    //}
}

void user::input(thing& bullet, thing& bullet2)
{
/*
* user::input do actions based on event.type and doKeyDown
*/
    SDL_Event event;
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
	    case SDL_QUIT:
                exit(0);
		break;

	    case SDL_KEYDOWN:
		doKeyDown(&event.key, true);
		break;

	    case SDL_KEYUP:
		doKeyDown(&event.key, false);
		break;

	    default:
		break;
	}
    }
    
    if (playerUp)
    {
            y -= speed;
            direction = 1;
	    newTexture("images/PlayerUp.png");
    }
    if (playerDown)
    {
            y += speed;
            direction = 2;
	    newTexture("images/PlayerDown.png");
    }
    if (playerLeft)
    {
            x -= speed;
            direction = 3;
	    newTexture("images/PlayerLeft.png");
    }
    if (playerRight)
    {
            x += speed;
            direction = 4;
	    newTexture("images/PlayerRight.png");
    }
    if (playerFired)
    {
            for(auto& currentBullet : bullets)
            {
                currentBullet->speed += 2;
            }
            
            bulletClass* newBulletClass = new bulletClass; 

            newBulletClass->appPointer = appPointer;
            newBulletClass->texture = appPointer->loadImages("images/bullet.png");
            newBulletClass->x = x;
            newBulletClass->y = y;
            newBulletClass->w = 22;
            newBulletClass->h = 22;
            newBulletClass->speed = 2;

            bullets.push_back(newBulletClass);
	    
            newTexture("images/Player.png");
    }
}

void user::keyMenu(bool& start, SDL_KeyboardEvent *event)
{
/*
* user::keyMenu handle key board events on the start menu 
*
* pre and postconditions:
*
* Precondition event is not NULL
*/
    if(event == NULL || event == nullptr)
    {
        throw std::invalid_argument("event can not be NULL");
    }    

    //Ignores keyboard repeat events
    if(event->repeat == 0)
    {
        if(event->keysym.scancode == SDL_SCANCODE_RETURN)
        {
            start = true;
        }
    }
}

void user::menuInput(bool& start)
{
/*
* user::menuInput Handle menu input 
*/
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                keyMenu(start, &event.key);
                break;

            default:
                break;
        }
    }
}

void user::logic(thing& enemy, points& point, int& counter)
{
/*
* user::logic push user when outside of SCREEN_WIDTH and SCREEN_HEIGHT 
*/
    // Remember [x, y] where y is upside down
    if (x < 0)
    {
        x += back;
    }
    else if (x > appPointer->appSCREEN_WIDTH-100)
    {
        x -= back;
    }
    else if (y < 0)
    {
        y += back;
    }
    else if (y > appPointer->appSCREEN_HEIGHT-100)
    {
        y -= back;
    }

    for(auto& currentBullet : bullets)
    {
        currentBullet->logic(*this);
        currentBullet->didBulletHit(enemy, counter);
        point.didYouGetPoints(*this, *currentBullet, counter);

        if(currentBullet->health <= 0)
        {
            delete currentBullet;
            currentBullet = nullptr;
        }
    }

    for(auto& currentBullet : bullets)
    {
        if(currentBullet == nullptr)
        {
            bullets.erase(std::remove(bullets.begin(), bullets.end(), currentBullet), bullets.end());
        }
    }
}

int user::show()
{
/*
* user::show show user or call playerDeath 
*/
    if(health > 0)
    {
        appPointer->imagePos(texture, x, y, w, h);
        
        healthDisplayCurrent = playerHealth->healthDisplayUpdate(*this);
        appPointer->imagePos(healthDisplayCurrent, 100, 0, 10, 10);
    
        for(auto& currentBullet : bullets)
        {
            currentBullet->show();
        }

        return 1;
    }
    else
    {
        playerDeath();
        return 0;
    }
    
    return 0;
}

void user::playerDeath()
{
/*
* user::playerDeath make death screen and exit game if health is less than or equal to zero  
*/
    if(health <= 0)
    {
        thing deathImage(0, 0, appPointer->appSCREEN_WIDTH, appPointer->appSCREEN_HEIGHT, 10, 0, appPointer->loadImages("images/Death.jpg"), appPointer);

        deathImage.show();

        appPointer->showVisuals();

        SDL_Delay(3000);

        exit(0);
    }
}



enemys::enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer)
 : thing(ix, iy, iw, ih, ihealth, ispeed, itexture, iappPointer)
{
/*
* enemys::enemys construct enemys with thing constructor 
* Pre and postconditions are taken care of by thing constructor 
*/
}

void enemys::spawnEnemys(int& enemySpawnTimer)
{
/*
* enemys::spawnEnemys if the spawn timer is up then spawn a enemy at SCREEN_WIDTH at random SCREEN_HEIGHT y value with a random speed 
*/
    if(enemySpawnTimer <= 0 && health <= 0)
    {
       x = appPointer->appSCREEN_WIDTH;
       y = rand() % appPointer->appSCREEN_HEIGHT;

       speed = minimum + (rand() % maximum);

       health = 1;

       smart = rand() % 2;

       x -= rand() % 10;

       enemySpawnTimer = rand() % 100;
    }
    else
    {
        x -= speed;

	enemySpawnTimer--;
    }
}

void enemys::didEnemyKill(user& player)
{
/*
* enemys::didEnemyKill take one health from player and make them use the sad texture if health is not zero and then put at a invalid x value 
*/
    if(collision(player.x, player.y, player.w, player.h, x, y, w, h) && health > 0)
    {
        health = 0;
        x = 1000;
        y = 1000;
        player.health -= 3;
        player.newTexture("images/PlayerSad.png");
    }
}

void enemys::makeEnd(int& levelOne)
{
/*
* enemys::makeEnd create the secret end with the enemy 
*/
    newTexture("images/secretEnd.gif");
    speed = 1;
    levelOne = 1;

    appPointer->imagePos(texture, 100, 100, w, h);

    appPointer->showVisuals();

    SDL_Delay(60000);
}

void enemys::scaleDifficulty(int counter)
{
/*
* enemys::scaleDifficulty Increase the speed range of the enemy based on the players score
*/
    if(counter > 200)
    {
        maximum = 20;
	minimum = 5;
    }
    else if(counter > 400)
    {
        maximum = 30;
	minimum = 10;
    }
}



points::points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer)
 : thing(ix, iy, iw, ih, ihealth, ispeed, itexture, iappPointer)
{
/*
* points::points construct a points with the thing constructor
* pre and postconditions are handled by thing constructor 
*/
}

void points::initPoints()
{
/*
* points::initPoints spawn point at a random point if it has no health and keep in the bounds of SCREEN_WIDTH and SCREEN_HEIGHT
*/
    if (health > 0)
    {
        x -= speed;

        if (x > appPointer->appSCREEN_WIDTH)
        {
            health = 0;
        }
        else if (x < 0)
        {
            health = 0;
        }
        else if (y > appPointer->appSCREEN_HEIGHT)
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
        x = appPointer->appSCREEN_WIDTH;
        y = rand() % appPointer->appSCREEN_HEIGHT;

        randomNum = 1 + (rand() % 9);
        health = randomNum;
        speed = randomNum;

        isHealth = false;
        newTexture("images/points.png");
    }
}

void points::didYouGetPoints(user& player, thing& bullet, int& counter)
{
/*
* points::didYouGetPoints detect collision of bullet and player and handle them 
*/
    auto playerPointCollision = std::async(std::launch::async, collision, player.x, player.y, player.w, player.h, x, y, w, h);
    auto bulletPointCollision = std::async(std::launch::async, collision, bullet.x, bullet.y, bullet.w, bullet.h, x, y, w, h);

    if(playerPointCollision.get() && health != 0)
    {
        if(health > player.health)
        {
	    int randomNum = rand() % 2;

	    if(randomNum && isHealth)
	    {
                player.health += 4;
	    }

            player.newTexture("images/PlayerHappy.png");
        }
        else
        {
            counter++;
            player.newTexture("images/PlayerHappy.png");
        }

        health = 0;
        counter++;
    }
    else if(bulletPointCollision.get() && bullet.health != 0 && health != 0 && !isHealth)
    {
        bullet.health += 1;
        newTexture("images/health.png");
        isHealth = true;
        counter++;
    }
}



bulletClass::bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer) 
: thing(ix, iy, iw, ih, ihealth, ispeed, itexture, iappPointer)
{
/*
* bulletClass::bulletClass construct a valid bulletClass
* thing constructor handles pre and postconditions
*/
};

void bulletClass::logic(const user& player)
{
/*
* bulletClass::logic move bulletClass based on player direction and set health to zero if off screen 
*/
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

    if (x > appPointer->appSCREEN_WIDTH)
    {
        health = 0;
    }
    else if (x < 0)
    {
        health = 0;
    }
    else if (y > appPointer->appSCREEN_HEIGHT)
    {
        health = 0;
    }
    else if (y < 0)
    {
        health = 0;
    }
}

void bulletClass::didBulletHit(thing& enemy, int& counter)
{
/*
* bulletClass::didBulletHit check if bullet colides with enemy and set enemy health to zero if true then update counter 
*/
    if(collision(x, y, w, h, enemy.x, enemy.y, enemy.w, enemy.h) && health != 0 && enemy.health != 0)
    {
        enemy.health = 0;
        enemy.x = 1000;
        enemy.y = 1000;
        health -= 1;
        counter++;
    }
}



healthDisplay::healthDisplay(SDL_Texture* ifullHealth, SDL_Texture* ihalfHealth, SDL_Texture* icritical)
{
/*
* healthDisplay::healthDisplay construct a health display with 3 textures  
*
* pre and postconditions:
*
* Precondition pointers are not NULL
*/
    try
    {
        if(ifullHealth == NULL || ifullHealth == nullptr)
        {
            throw std::invalid_argument("ifullHealth can not be NULL");
        }
        else if(ihalfHealth == NULL || ihalfHealth == nullptr)
        {
            throw std::invalid_argument("ihalfHealth can not be NULL");
        }
        else if(icritical == NULL || icritical == nullptr)
        {
            throw std::invalid_argument("icritical can not be NULL");
        }

        fullHealth = ifullHealth;
        halfHealth = ihalfHealth;
        critical = icritical;
    }
    catch(...)
    {
        SDL_DestroyTexture(ifullHealth);
        SDL_DestroyTexture(ihalfHealth);
        SDL_DestroyTexture(icritical);
    }
}

SDL_Texture* healthDisplay::healthDisplayUpdate(const user& player)
{
/*
* healthDisplay::healthDisplayUpdate update healthDisplay based on player health
*/
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
    return fullHealth;
}



//Takes two objects dimetions
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
/*
* collision use the two objects x y w h to detect collision 
*/
    return ( std::max(x1, x2) < std::min(x1 + w1, x2 + w2) ) && ( std::max(y1, y2) < std::min(y1 + h1, y2 + h2) );
}
