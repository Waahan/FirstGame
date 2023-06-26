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
 : x{ix}, y{iy}, w{iw}, h{ih}, health{ihealth}, speed{ispeed}, texture{itexture}, appPointer{iappPointer}
{
/*
* thing::thing create a thing with the x y w h texture and app pointer 
* 
* pre and postconditions:
* 
* Precondition iappPointer and itexture can not be NULL
* Precondition iw and ih can not be less or equal to zero 
* Precondition ihealth can not be less than zero
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
    }
    catch(...)
    {
        SDL_DestroyTexture(itexture);
        throw;
    }
}

thing::thing(const thing& copyFromThing)
: x{copyFromThing.x}, y{copyFromThing.y}, w{copyFromThing.w}, h{copyFromThing.h}, health{copyFromThing.health}, speed{copyFromThing.speed}, texture{copyFromThing.appPointer->loadImages("images/defaultThing.png")}, appPointer{copyFromThing.appPointer}
{
    std::cerr << "It is better to move a thing then copy";
}

thing& thing::operator=(const thing& copyFromThing)
{
    x = copyFromThing.x;
    y = copyFromThing.y;
    w = copyFromThing.w;
    h = copyFromThing.h;
    health = copyFromThing.health;
    speed = copyFromThing.speed;
    texture = copyFromThing.appPointer->loadImages("images/defaultThing.png");
    appPointer = copyFromThing.appPointer;

    return *this;
}

thing::thing(thing&& moveFromThing)
 : x{moveFromThing.x}, y{moveFromThing.y}, w{moveFromThing.w}, h{moveFromThing.h}, health{moveFromThing.health}, speed(moveFromThing.speed), texture{moveFromThing.texture}, appPointer{moveFromThing.appPointer}
{
    moveFromThing.x = 0;
    moveFromThing.y = 0;
    moveFromThing.w = 0;
    moveFromThing.h = 0;
    moveFromThing.health = 0;
    moveFromThing.speed = 0;
    moveFromThing.texture = nullptr;
    moveFromThing.appPointer = nullptr;
}

thing& thing::operator=(thing&& moveFromThing)
{
    x = moveFromThing.x;
    y = moveFromThing.y;
    w = moveFromThing.w;
    h = moveFromThing.h;
    health = moveFromThing.health;
    speed = moveFromThing.speed;
    texture = moveFromThing.texture;
    appPointer = moveFromThing.appPointer;

    moveFromThing.x = 0;
    moveFromThing.y = 0;
    moveFromThing.w = 0;
    moveFromThing.h = 0;
    moveFromThing.health = 0;
    moveFromThing.speed = 0;
    moveFromThing.texture = nullptr;
    moveFromThing.appPointer = nullptr;

    return *this;
}

void thing::logic()
{
/*
* thing::logic sets health equal to zero if thing is outside SCREEN_WIDTH and SCREEN_HEIGHT
*/
    if (x > appPointer->SCREEN_WIDTH)
    {
        health = 0;
    }
    else if (x < 0)
    {
        health = 0;
    }
    else if (y > appPointer->SCREEN_HEIGHT)
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
        throw std::invalid_argument("newTexture can not be NULL");
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

/* 
    Get functions return protected value
*/
inline int thing::getX() const
{
    return x;
}

inline int thing::getY() const
{
    return y;
}

inline int thing::getW() const
{
    return w;
}

inline int thing::getH() const
{
    return h;
}

inline int thing::getHealth() const
{
    return health;
}

inline int thing::getSpeed() const
{
    return speed;
}

inline SDL_Texture* thing::getTexture() const
{
    return texture;
}

inline void thing::setX(int setX)
{
/*
* thing::setX set x equal to setX
* 
* Pre and postconditions:
*
* Precondition setX is in window x 
*/
    if(setX > appPointer->SCREEN_WIDTH || setX < 0)
    {
        throw std::out_of_range("x can not be less than screen_width or greater than zero");    
    }

    x = setX;
}

inline void thing::setY(int setY)
{
/*
* thing::setY set y equal to y
*
* Pre and postconditions:
*
* Precondition setY is in window y
*/
    if(setY > appPointer->SCREEN_HEIGHT || setY < 0)
    {
        throw std::out_of_range("y can not be less than screen_height or greater than zero");
    }

    y = setY;
}

inline void thing::setW(int setW)
{
/*
* thing::setW set w equal to setW
*
* Pre and postconditions:
*
* Precondition setW is not less than or equal to zero
*/
    if(setW <= 0)
    {
        throw std::invalid_argument("w can not be less than or equal to zero");
    }

    w = setW;
}

inline void thing::setH(int setH)
{
/*
* thing::setH set h equal to setH
*
* Pre and postconditions:
*
* Precondition setH is not less than or equal to zero
*/
    if(setH <= 0)
    {
        throw std::invalid_argument("h can not be less than or equal to zero");
    }

    h = setH;
}

inline void thing::setSpeed(int setSpeed)
{
/*
* thing::setSpeed set speed equal to setSpeed
*/
    speed = setSpeed;
}

inline void thing::setHealth(int setHealth)
{
/*
* thing::setHealth set health equal to setHealth
*
* Pre and postconditions:
*
* Precondition setHealth can not be less than zero
*/
    if(setHealth < 0)
    {
        throw std::invalid_argument("health can not be less than zero");
    }

    health = setHealth;
}

inline void thing::removeFromScreen()
{
    x = appPointer->SCREEN_WIDTH * 2;
    y = appPointer->SCREEN_HEIGHT * 2;
}

inline void thing::minusHealth(int subtractNum)
{
/*
* thing::minusHealth subtract subtractNum from health
*
* Pre and postconditions:
*
* Precondition health can not be negative
*/
    int checkNum = health - subtractNum;
    
    if(checkNum < 0)
    {
        throw std::invalid_argument("health can not be less than zero");
    }

    health -= subtractNum;
}



counter::counter()
{
    updateStringCount();
}   

std::string counter::stringCurrentCount()
{
    if(oldCount != currentCount)
    {
        updateStringCount();
    }

    return stringCount;
}

counter& counter::operator++(int)
{
    currentCount++;

    return *this;
}



user::user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer, int iback, int idirection) 
: thing(ix, iy, iw, ih, ihealth, ispeed, itexture, iappPointer)
{
/*
* user::user construct a valid user 
*
* pre and postconditions:
*
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

        playerHealth = new healthDisplay{appPointer->loadImages("images/Health1.jpg"), appPointer->loadImages("images/Health2.jpg"), appPointer->loadImages("images/Health3.jpg") };
    }
    catch(...)
    {
        SDL_DestroyTexture(itexture);
        throw;
    }
}

user::user(const user& copyFromUser)
: thing(copyFromUser), direction{copyFromUser.direction}, back{copyFromUser.back}
{}

user::user(user&& moveFromUser)
 : thing(moveFromUser), direction{moveFromUser.direction}, back{moveFromUser.back}
{
    moveFromUser.direction = 0;
    moveFromUser.back = 0;
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

void user::input()
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
            
            bulletClass* newBulletClass = new bulletClass{x, y, 22, 22, 1, 2, appPointer->loadImages("images/bullet.png"), appPointer}; 

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

void user::logic(thing& enemy, points& point)
{
/*
* user::logic push user when outside of SCREEN_WIDTH and SCREEN_HEIGHT 
*/
    // Remember [x, y] where y is upside down
    if (x < 0)
    {
        x += back;
    }
    else if (x > appPointer->SCREEN_WIDTH-100)
    {
        x -= back;
    }
    else if (y < 0)
    {
        y += back;
    }
    else if (y > appPointer->SCREEN_HEIGHT-100)
    {
        y -= back;
    }    

    for(auto& currentBullet : bullets)
    {
        currentBullet->logic(*this);
        currentBullet->didBulletHit(enemy, playerScore);
        point.didYouGetPoints(*this, *currentBullet, playerScore);

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
        appPointer->imagePos(texture, x, y);
        
        healthDisplayCurrent = playerHealth->healthDisplayUpdate(*this);
        appPointer->imagePos(healthDisplayCurrent, 100, 0);
    
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
        thing deathImage(0, 0, appPointer->SCREEN_WIDTH, appPointer->SCREEN_HEIGHT, 10, 0, appPointer->loadImages("images/Death.jpg"), appPointer);

        deathImage.show();

        appPointer->showVisuals();

        SDL_Delay(3000);

        exit(0);
    }
}

inline int user::getDirection() const 
{
    return direction;
}



enemys::enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, SDL_Texture* itexture, App* iappPointer)
 : thing(ix, iy, iw, ih, ihealth, ispeed, itexture, iappPointer)
{
/*
* enemys::enemys construct enemys with thing constructor 
* Pre and postconditions are taken care of by thing constructor 
*/
}

enemys::enemys(const enemys& copyFromEnemy)
 : thing(copyFromEnemy)
{}

enemys::enemys(enemys&& moveFromEnemy)
 : thing(moveFromEnemy)
{}

void enemys::spawnEnemys()
{
/*
* enemys::spawnEnemys if the spawn timer is up then spawn a enemy at SCREEN_WIDTH at random SCREEN_HEIGHT y value with a random speed 
*/
    if(enemySpawnTimer <= 0 && health <= 0)
    {
       x = appPointer->SCREEN_WIDTH;
       y = rand() % appPointer->SCREEN_HEIGHT;

       speed = minimum + (rand() % maximum);

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

void enemys::didEnemyKill(user& player)
{
/*
* enemys::didEnemyKill take one health from player and make them use the sad texture if health is not zero and then put at a invalid x value 
*/
    if(collision(player.getX(), player.getY(), player.getW(), player.getH(), x, y, w, h) && health > 0)
    {
        health = 0;
        removeFromScreen();
        player.minusHealth(1);
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

void enemys::scaleDifficulty(const counter& playerScore)
{
/*
* enemys::scaleDifficulty Increase the speed range of the enemy based on the players score
*/
    if(playerScore.count() > 200)
    {
        maximum = 20;
	minimum = 5;
    }
    else if(playerScore.count() > 400)
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

points::points(const points& copyFromPoint)
 : thing(copyFromPoint) 
{}

points::points(points&& moveFromPoint)
 : thing(moveFromPoint)
{}

void points::initPoints()
{
/*
* points::initPoints spawn point at a random point if it has no health and keep in the bounds of SCREEN_WIDTH and SCREEN_HEIGHT
*/
    if (health > 0)
    {
        x -= speed;

        if (x > appPointer->SCREEN_WIDTH)
        {
            health = 0;
        }
        else if (x < 0)
        {
            health = 0;
        }
        else if (y > appPointer->SCREEN_HEIGHT)
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
        x = appPointer->SCREEN_WIDTH;
        y = rand() % appPointer->SCREEN_HEIGHT;

        randomNum = 1 + (rand() % 9);
        health = randomNum;
        speed = randomNum;

        isHealth = false;
        newTexture("images/points.png");
    }
}

void points::didYouGetPoints(user& player, thing& bullet, counter& playerScore)
{
/*
* points::didYouGetPoints detect collision of bullet and player and handle them 
*/
    auto playerPointCollision = std::async(std::launch::async, collision, player.getX(), player.getY(), player.getW(), player.getH(), x, y, w, h);
    auto bulletPointCollision = std::async(std::launch::async, collision, bullet.getX(), bullet.getY(), bullet.getW(), bullet.getH(), x, y, w, h);

    if(playerPointCollision.get() && health != 0)
    {
        if(health > player.getHealth())
        {
	    int randomNum = rand() % 2;

	    if(randomNum && isHealth)
	    {
                player.minusHealth(-1);
	    }

            player.newTexture("images/PlayerHappy.png");
        }
        else
        {
            playerScore++;
            player.newTexture("images/PlayerHappy.png");
        }

        health = 0;
        playerScore++;
    }
    else if(bulletPointCollision.get() && bullet.getHealth() != 0 && health != 0 && !isHealth)
    {
        bullet.minusHealth(-1);
        newTexture("images/health.png");
        isHealth = true;
        playerScore++;
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

bulletClass::bulletClass(const bulletClass& copyFromBullet)
 : thing(copyFromBullet)
{}

bulletClass::bulletClass(bulletClass&& moveFromBullet)
 : thing(moveFromBullet)
{}

void bulletClass::logic(const user& player)
{
/*
* bulletClass::logic move bulletClass based on player direction and set health to zero if off screen 
*/
    if(player.getDirection() == 1)
    {
        y -= speed;
    }
    else if(player.getDirection() == 2)
    {
        y += speed;
    }
    else if(player.getDirection() == 3)
    {
        x -= speed;
    }
    else if(player.getDirection() == 4)
    {
        x += speed;
    }

    if (x > appPointer->SCREEN_WIDTH)
    {
        health = 0;
    }
    else if (x < 0)
    {
        health = 0;
    }
    else if (y > appPointer->SCREEN_HEIGHT)
    {
        health = 0;
    }
    else if (y < 0)
    {
        health = 0;
    }
}

void bulletClass::didBulletHit(thing& enemy, counter& playerScore)
{
/*
* bulletClass::didBulletHit check if bullet colides with enemy and set enemy health to zero if true then update counter 
*/
    if(collision(x, y, w, h, enemy.getX(), enemy.getY(), enemy.getW(), enemy.getH()) && health != 0 && enemy.getHealth() != 0)
    {
        enemy.setHealth(0);
        enemy.removeFromScreen();
        health -= 1;
        playerScore++;
    }
}



healthDisplay::healthDisplay(SDL_Texture* ifullHealth, SDL_Texture* ihalfHealth, SDL_Texture* icritical)
: fullHealth(ifullHealth), halfHealth(ihalfHealth), critical(icritical) 
{
/*
* healthDisplay::healthDisplay construct a health display with 3 textures  
*/
}

healthDisplay::healthDisplay(healthDisplay&& moveFromHealthDisplay)
 : fullHealth{moveFromHealthDisplay.fullHealth}, halfHealth{moveFromHealthDisplay.halfHealth}, critical{moveFromHealthDisplay.critical}
{
    moveFromHealthDisplay.fullHealth = nullptr;
    moveFromHealthDisplay.halfHealth = nullptr;
    moveFromHealthDisplay.critical = nullptr;
}

SDL_Texture* healthDisplay::healthDisplayUpdate(const user& player)
{
/*
* healthDisplay::healthDisplayUpdate update healthDisplay based on player health
*/
    switch(player.getHealth())
    {
        case 3:
	    return fullHealth;
	case 2:
	    return halfHealth;
	case 1:
	    return critical;
	default:
	    if(player.getHealth() > 2)
	    {
	        return fullHealth;
	    }
	    else if(player.getHealth() > 1)
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
