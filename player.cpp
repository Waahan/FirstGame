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
/*
* thing::thing copy constructor make a copy of thing as best as I can 
*
* Postcondition warn about copying thing
*/
    std::cerr << "It is better to move a thing then copy";
}

thing& thing::operator=(const thing& copyFromThing)
{
/*
* thing::operator= copy copy thing 
*
* Postcondition warn about copying thing
*/
    x = copyFromThing.x;
    y = copyFromThing.y;
    w = copyFromThing.w;
    h = copyFromThing.h;
    health = copyFromThing.health;
    speed = copyFromThing.speed;
    texture = copyFromThing.appPointer->loadImages("images/defaultThing.png");
    appPointer = copyFromThing.appPointer;

    std::cerr << "It is best to move thing not copy it";

    return *this;
}

thing::thing(thing&& moveFromThing)
 : x{moveFromThing.x}, y{moveFromThing.y}, w{moveFromThing.w}, h{moveFromThing.h}, health{moveFromThing.health}, speed(moveFromThing.speed), texture{moveFromThing.texture}, appPointer{moveFromThing.appPointer}
{
/*
* thing::thing move constructor move thing
*
* Postcondition thing&& can not destroy texture pointer when deconstructed
*/
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
/*
* thing::operator= move same thing as thing move constructor
*/
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

thing& thing::logic()
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

    return *this;
}

thing& thing::newTexture(SDL_Texture* newTexture)
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

    return *this;
}

thing& thing::newTexture(const char* newTexturePath)
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

    return *this;
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

inline thing& thing::setX(int setX)
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

    return *this;
}

inline thing& thing::setY(int setY)
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

    return *this;
}

inline thing& thing::setW(int setW)
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

    return *this;
}

inline thing& thing::setH(int setH)
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

    return *this;
}

inline thing& thing::setSpeed(int setSpeed)
{
/*
* thing::setSpeed set speed equal to setSpeed
*/
    speed = setSpeed;

    return *this;
}

inline thing& thing::setHealth(int setHealth)
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

    return *this;
}

inline thing& thing::removeFromScreen()
{
/*
* thing::removeFromScreen get thing off screeen
*/
    x = appPointer->SCREEN_WIDTH * 2;
    y = appPointer->SCREEN_HEIGHT * 2;

    return *this;
}

inline thing& thing::minusHealth(int subtractNum)
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

    return *this;
}



counter::counter()
{
/*
* counter::counter just update string count
*/
    updateStringCount();
}   

counter::~counter(){}

std::string counter::stringCurrentCount()
{
/*
* counter::stringCurrentCount return current count as a string update if needed 
*/
    if(oldCount != currentCount)
    {
        updateStringCount();
    }

    return stringCount;
}

counter& counter::operator++(int)
{
/*
* counter::operator++ increase the currentCount by one
*/
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

        std::cout << "NumJoysticks: " << SDL_NumJoysticks() << std::endl;
        if(SDL_NumJoysticks() > 0 && SDL_IsGameController(0))
        {
            useController = true;

            // 0 is left 1 is right 
            gameController = SDL_GameControllerOpen(0);

            joystickOne = SDL_GameControllerGetJoystick(gameController);
            joystickTwo = SDL_GameControllerGetJoystick(gameController);

            if(joystickOne == NULL || joystickTwo == NULL)
            {
                std::cerr << "SDL_GameControllerGetJoystick failed: " << SDL_GetError() << std::endl;
            }
            
            if(gameController == NULL)
            {
                std::cerr << "SDL_GameControllerOpen failed: " << SDL_GetError() << std::endl;
            }
    
            std::cout << "Found game controller: " << SDL_GameControllerName(gameController) << std::endl;
            std::cout << "joystick axes num: " << SDL_JoystickNumAxes(joystickOne) << std::endl;
            std::cout << "joystick num buttons: " << SDL_JoystickNumButtons(joystickOne) << std::endl;
            std::cout << "joystick num balls: " << SDL_JoystickNumBalls(joystickOne) << std::endl;
            std::cout << "joystick num hats: " << SDL_JoystickNumHats(joystickOne) << std::endl;

            SDL_JoystickEventState(SDL_ENABLE);
        }
    }
    catch(...)
    {
        SDL_DestroyTexture(itexture);
        throw;
    }
}

user::user(const user& copyFromUser)
: thing(copyFromUser), direction{copyFromUser.direction}, back{copyFromUser.back}
/*
* user::user copy constructor uses the default thing copy constructor and then just add the extra varibles 
*/
{}

user::user(user&& moveFromUser)
 : thing(moveFromUser), direction{moveFromUser.direction}, back{moveFromUser.back}
/*
* Same as the copy constructor description but for move
*/
{
    moveFromUser.direction = 0;
    moveFromUser.back = 0;
}

user::~user()
{
/*
* user::~user delete texture healthDisplayCurrent playerHealth a bullets 
*/
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(healthDisplayCurrent);

    delete playerHealth;

    for(auto& currentBullet : bullets)
    {
        delete currentBullet;
        currentBullet = nullptr;
        bullets.erase(std::remove(bullets.begin(), bullets.end(), currentBullet), bullets.end());
    }

    if(useController)
    {
        SDL_JoystickClose(joystickOne);
        joystickOne = NULL;
        
        SDL_JoystickClose(joystickTwo);
        joystickTwo = NULL;

        SDL_GameControllerClose(gameController);        
        gameController = NULL;
    }
}

//Scan codes at https://wiki.libsdl.org/SDL2/SDL_Scancode
user& user::doKeyDown(SDL_KeyboardEvent *event, bool DownUp)
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

    return *this;
}

user& user::input()
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

            case SDL_JOYAXISMOTION:
                
                playerUp = false;
                playerDown = false;
                playerLeft = false;
                playerRight = false;

                if(event.jaxis.value < -8000 && event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
                {
                    //left motion
                    playerLeft = true;
                    joystickDirection = directions::left;
                }
                else if(event.jaxis.value < -100 && event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX && joystickDirection == directions::left)
                {
                    playerLeft = true;
                }
                else if(event.jaxis.value > 8000 && event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
                {
                    //right motion
                    playerRight = true;
                    joystickDirection = directions::right;
                }
                else if(event.jaxis.value > 100 && event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX && joystickDirection == directions::right)
                {
                    playerRight = true;
                }
                else if(event.jaxis.value < -8000 && event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
                {
                    //up motion
                    playerUp = true;
                    joystickDirection = directions::up;
                }
                else if(event.jaxis.value < -100 && event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY && joystickDirection == directions::up)
                {
                    playerUp = true;
                }
                else if(event.jaxis.value > 8000 && event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
                {
                    //down motion
                    playerDown = true;
                    joystickDirection = directions::up;
                }
                else if(event.jaxis.value > 100 && event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY && joystickDirection == directions::down)
                { 
                   playerDown = true;
                }
                else
                {
                    joystickDirection = directions::none;
                }

                if(event.jaxis.value < -8000 && event.jaxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
                    playerFired = true;
                else
                    playerFired = false;

                if(event.jaxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
                    std::cout << "Trigger left. value: " << event.jaxis.value << std::endl;

                if(event.jaxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
                    std::cout << "Trigger right. value: " << event.jaxis.value << std::endl;
                
                if(event.jaxis.axis == SDL_CONTROLLER_AXIS_INVALID)
                    std::cout << "axis invalid. value: " << event.jaxis.value << std::endl;
                
                if(event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
                    std::cout << "axis leftx. value: " << event.jaxis.value << std::endl;
                
                if(event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
                    std::cout << "axis lefty. value: " << event.jaxis.value << std::endl;
                
                if(event.jaxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
                    std::cout << "axis rightx. value: " << event.jaxis.value << std::endl;
                
                if(event.jaxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
                    std::cout << "axis righty. value: " << event.jaxis.value << std::endl;
                
                if(event.jaxis.axis == SDL_CONTROLLER_AXIS_MAX)
                    std::cout << "Axis max. value: " << event.jaxis.value << std::endl;

                break;

            case SDL_JOYBALLMOTION:
                playerUp = false;
                playerDown = false;
                playerLeft = false;
                playerRight = false;

                if(event.jhat.value == SDL_HAT_UP)
                {
                    playerUp = true;
                }
                
                if(event.jhat.value == SDL_HAT_DOWN)
                {
                    playerDown = true;
                }
    
                if(event.jhat.value == SDL_HAT_LEFT)
                {
                    playerLeft = true;
                }

                if(event.jhat.value == SDL_HAT_RIGHT)
                {
                    playerRight = true;
                }

                break;
            case SDL_CONTROLLERBUTTONDOWN:
                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
                {
                    playerFired = true;
                }
                
                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
                    std::cout << "LEFTSHOULDER" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_PADDLE1)
                    std::cout << "PADDLE1" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_PADDLE2)
                    std::cout << "PADDLE2" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_PADDLE3)
                    std::cout << "PADDLE3" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_PADDLE4)
                    std::cout << "PADDLE4" << std::endl;
                
                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                    std::cout << "BUTTONA" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
                    std::cout << "BUTTONB" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
                    std::cout << "BUTTONX" << std::endl;
                
                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
                    std::cout << "BUTTONY" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
                    std::cout << "BUTTON BACK" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
                    std::cout << "BUTTON GUIDE" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
                    std::cout << "BUTTON START" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_MISC1)
                    std::cout << "BUTTON MISC1" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_INVALID)
                    std::cout << "button invalid" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
                    std::cout << "button left stick" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
                    std::cout << "button right stick" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                    std::cout << "button dpad up" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                    std::cout << "button dpad down" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                    std::cout << "button dpad left" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                    std::cout << "button dpad right" << std::endl;
                
                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_TOUCHPAD)
                    std::cout << "button touchpad" << std::endl;

                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_MAX)
                    std::cout << "button max" << std::endl;

                break;

            case SDL_CONTROLLERBUTTONUP:
                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
                {
                    playerFired = false;
                }
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                break;
            
            case SDL_TEXTEDITING:
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
    
    return *this;
}

user& user::keyMenu(bool& start, SDL_KeyboardEvent *event)
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

    return *this;
}

user& user::menuInput(bool& start)
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

    return *this;
}

user& user::logic(thing& enemy, points& point)
{
/*
* user::logic push user when outside of SCREEN_WIDTH and SCREEN_HEIGHT and delete bullets when outside of the window
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

    return *this;
}

int user::show()
{
/*
* user::show show user, healthDiplay and bullets or call playerDeath 
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

enemys& enemys::spawnEnemys()
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

    return *this;
}
 
enemys& enemys::didEnemyKill(user& player)
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

    return *this;
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

enemys& enemys::scaleDifficulty(const counter& playerScore)
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

    return *this;
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

points& points::initPoints()
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

    return *this;
}

points& points::didYouGetPoints(user& player, thing& bullet, counter& playerScore)
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
    
    return *this;
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

bulletClass& bulletClass::logic(const user& player)
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

    return *this;
}

bulletClass& bulletClass::didBulletHit(thing& enemy, counter& playerScore)
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

    return *this;
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
