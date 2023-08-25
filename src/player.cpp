#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <random>
#include <chrono>
#include <execution>

#include <cassert>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "headerPlayer.h"
#include "headerVisuals.h"

thing::thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path)
 : x{ix}, y{iy}, w{iw}, h{ih}, health{ihealth}, speed{ispeed}
{
/*
* thing::thing create a thing with the x y w h texture and app pointer 
* 
* pre and postconditions:
* 
* Precondition iw and ih can not be less or equal to zero 
* Precondition ihealth can not be less than zero
* 
* Postcondition creates a default Image from path
*/    
    assert(!(iw <= 0 || ih <= 0) && "Invalid thing w or h");
        
    assert(!(ihealth < 0) && "Invalid health must be greater than zero");
    
    Images["default"] = Image(path, 0, 0, iw, ih);
}

thing& thing::logic()
{
/*
* thing::logic sets health equal to zero if thing is outside SCREEN_WIDTH and SCREEN_HEIGHT
*
* Postcondition thing has no health if outside of screen
*/
    if (x > App::get().SCREEN_WIDTH || x < 0 || y > App::get().SCREEN_HEIGHT || y < 0)
        health = 0;

    return *this;
}

bool thing::show()
{
/*
* thing::show use imagePos to display thing on screen
*
* Precondition only renders if thing has health
* 
* Postcondition increments currentImage so it can go to the next frame
* Postcondition sets currentImage to default if animation done 
*/
    if(health > 0)
    {
        App::get().imagePos(Images[currentImage], x, y, w, h);
        Images[currentImage]++;
    }

    return health > 0;
}

inline thing& thing::setX(int setX)
{
/*
* thing::setX set x equal to setX
* 
* Precondition setX is in the window 
*/
    assert(!(setX > App::get().SCREEN_WIDTH || setX < 0) && "Invalid thing x value");

    x = setX;

    return *this;
}

inline thing& thing::setY(int setY)
{
/*
* thing::setY set y equal to y
*
* Precondition setY is in the window 
*/
    assert(!(setY > App::get().SCREEN_HEIGHT || setY < 0) && "Invalid thing y value");

    y = setY;

    return *this;
}

inline thing& thing::setSpeed(int setSpeed) noexcept
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
* Precondition setHealth can not be less than zero
*/
    if(setHealth < 0)
        std::cerr << "health can not be less than zero" << std::endl;

    health = setHealth;

    return *this;
}

inline thing& thing::removeFromScreen()
{
/*
* thing::removeFromScreen get thing off screeen
*
* Postcondition y and x are not on screen
*/
    x = App::get().SCREEN_WIDTH * 2;
    y = App::get().SCREEN_HEIGHT * 2;

    return *this;
}

inline thing& thing::minusHealth(int subtractNum)
{
/*
* thing::minusHealth subtract subtractNum from health
*
* Pre and postconditions:
*
* Precondition health is not negative after subtraction
*/
    if(health - subtractNum < 0)
        std::cerr << "health can not be less than zero" << std::endl;

    health -= subtractNum;

    return *this;
}



counter::counter()
{
/*
* counter::counter update string count
*/
    updateStringCount();
}   

std::string counter::stringCurrentCount()
{
/*
* counter::stringCurrentCount return current count as a string update if needed 
*
* Postcondition does not update if not needed
* Postcondition returns a string equal to the current count
*/
    if(oldCount != currentCount)
        updateStringCount();

    return stringCount;
}

counter& counter::operator++(int)
{
/*
* counter::operator++ increment currentCount
*/
    currentCount++;

    return *this;
}

void counter::updateStringCount()
{
/*
* count::updateStringCount set string to string version of currentCount and update oldCount
*/
    stringCount = std::to_string(currentCount);
    
    oldCount = currentCount;
}



user::user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path, char iback) 
 : thing(ix, iy, iw, ih, ihealth, ispeed, path), back{iback}, joystickOne{}, gameController{}
{
/*
* user::user constructs a valid user 
*
* Postcondition user has a health display 
* Postcondition if there are controllers/joysticks user opens them 
*/
    playerHealth.reset(new healthDisplay{"images/Health1.png", "images/Health2.png", "images/Health3.png"});

    addControllerSupport();    
 
    if(SDL_GetNumTouchDevices() > 0)
    {
        useTouchScreen = true;
        
        touchDeviceID = SDL_GetTouchDevice(0);

        if(touchDeviceID == 0)
        {
            std::cerr << "SDL_GetTouchDevice failed: " << SDL_GetError() << std::endl;
        }

        std::cout << "Found touch device!" << std::endl;
        std::cout << "Touch devices: " << SDL_GetNumTouchDevices() << std::endl;
        std::cout << "Fingers down: " << SDL_GetNumTouchFingers(touchDeviceID) << std::endl; 
    }
}

//Scan codes at https://wiki.libsdl.org/SDL2/SDL_Scancode
void user::doKeyDown(const SDL_KeyboardEvent& event, bool DownUp)
{
/*
* user::doKeyDown handle keyboard events
*/
    switch(event.keysym.scancode)
    {
        case SDL_SCANCODE_UP:
            [[fallthrough]];
        case SDL_SCANCODE_W:
            playerUp = DownUp;
            break;
        
        case SDL_SCANCODE_DOWN:
            [[fallthrough]];
        case SDL_SCANCODE_S:
            playerDown = DownUp;
            break;
    
        case SDL_SCANCODE_LEFT:
            [[fallthrough]];
        case SDL_SCANCODE_A:
            playerLeft = DownUp;
            break;

        case SDL_SCANCODE_RIGHT:
            [[fallthrough]];
        case SDL_SCANCODE_D:
            playerRight = DownUp;
            break;
        
        case SDL_SCANCODE_SPACE:
            playerFired = DownUp;
            break;

        default:
            break;
    }
}

void user::doButtonDown(const SDL_Event& event, bool upOrDown)
{
/*
* user::doButtonDown handles controller button events 
*/
    //Buttons at https://wiki.libsdl.org/SDL2/SDL_GameControllerButton
    switch(event.cbutton.button)
    {
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            std::cout << "RIGHTSHOULDER" << std::endl;
            playerFired = upOrDown;
            break;

        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            std::cout << "LEFTSHOULDER" << std::endl;
            break;
    }
}

void user::doAxisMove(const SDL_Event& event)
{
/*
* user::doAxisMove handle axis moves by scaling speed based on event.jaxis.value
*
* Postcondition scale a future varible for speed based a event.jaxis.value 
*/
    //https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinput.html
    //More axis at https://wiki.libsdl.org/SDL2/SDL_GameControllerAxis
    //The directions "down" and "right" have positive values here.
    
    //-32768 to 32767
    short value = event.jaxis.value;

    char axis = event.jaxis.axis; 
    //SDL_JoystickID which = event.jaxis.which;
    
    const int ignoreZone = 8000;
    const int softIgnoreZone = 0;

    playerUp = false;
    playerDown = false;
    playerLeft = false;
    playerRight = false;
    playerFired = false;

    switch(axis)
    {
        case SDL_CONTROLLER_AXIS_LEFTX:
            if(value > ignoreZone || (value > softIgnoreZone && joystickDirection == directions::right))
            {
                playerRight = true;
                joystickDirection = directions::right;
            }
            else if(value < -ignoreZone || (value < -softIgnoreZone && joystickDirection == directions::left))
            {
                playerLeft = true;
                joystickDirection = directions::left;
            }
            break;
        
        case SDL_CONTROLLER_AXIS_RIGHTX:
            if(value > ignoreZone || (value > softIgnoreZone && joystickDirection == directions::right))
            {
                playerRight = true;
                joystickDirection = directions::right;
            }
            else if(value < -ignoreZone || (value < -softIgnoreZone && joystickDirection == directions::left))
            {
                playerLeft = true;
                joystickDirection = directions::left;
            }
            break;

        case SDL_CONTROLLER_AXIS_LEFTY:
            if(value > ignoreZone || (value > softIgnoreZone && joystickDirection == directions::up))
            {
                playerDown = true;
                joystickDirection = directions::up;
            }
            else if(value < -ignoreZone || (value < -softIgnoreZone && joystickDirection == directions::down))
            {
                playerUp = true;
                joystickDirection = directions::down;
            }
            break;

        case SDL_CONTROLLER_AXIS_RIGHTY:
            if(value > ignoreZone || (value > softIgnoreZone && joystickDirection == directions::up))
            {
                playerDown = true;
                joystickDirection = directions::up;
            }
            else if(value < -ignoreZone || (value < -softIgnoreZone && joystickDirection == directions::down))
            {
                playerUp = true;
                joystickDirection = directions::down;
            }
            break;
        
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
            playerFired = true;
            break;
        
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
            playerFired = true;            
            break;

        default:
            break;
    }
}

void user::doJoyHatMove(const SDL_Event& event)
{
/*
 * user::doJoyHatMove handle joyhat events 
*/
    playerUp = false;
    playerDown = false;
    playerLeft = false;
    playerRight = false;

    //https://wiki.libsdl.org/SDL2/SDL_JoyHatEvent
    switch(event.jhat.value)
    {
        case SDL_HAT_UP:
            playerUp = true;
            break;
        
        case SDL_HAT_DOWN:
            playerDown = true;
            break;

        case SDL_HAT_LEFT:
            playerLeft = true;
            break;
            
        case SDL_HAT_RIGHT:
            playerRight = true;
            break;
    }
}

void user::addControllerSupport()
{
/*
 * user::addControllerSupport opens controller and joystick and enables joystick events 
*/
    SDL_JoystickEventState(SDL_ENABLE);

    if(SDL_NumJoysticks() > 0 && SDL_IsGameController(0))
    {
        useController = true;

        gameController.reset(SDL_GameControllerOpen(0));

        joystickOne.reset(SDL_GameControllerGetJoystick(gameController.get()));

        if(!joystickOne)
            std::cerr << "SDL_GameControllerGetJoystick failed: " << SDL_GetError() << std::endl;

        if(!gameController)
            std::cerr << "SDL_GameControllerOpen failed: " << SDL_GetError() << std::endl;

        std::cout << "Found game controller: " << SDL_GameControllerName(gameController.get()) << std::endl;
        std::cout << "NumJoysticks: " << SDL_NumJoysticks() << std::endl;
        std::cout << "joystick axes num: " << SDL_JoystickNumAxes(joystickOne.get()) << std::endl;
        std::cout << "joystick num buttons: " << SDL_JoystickNumButtons(joystickOne.get()) << std::endl;
        std::cout << "joystick num balls: " << SDL_JoystickNumBalls(joystickOne.get()) << std::endl;
        std::cout << "joystick num hats: " << SDL_JoystickNumHats(joystickOne.get()) << std::endl;
    }
}

void user::removeControllerSupport()
{
/*
 * user::removeControllerSupport
*/
    useController = false;
}

void user::doFingerDown(const SDL_Event& event, bool upOrDown)
{
/*
* user::doFingerDown change varibles based on touch event varibles
*/
    //To get the touch coordinates in screen coordinates simply multiply the touch coordinates by the screen resolution. 
    //From 0 to 1
    float touchX = event.tfinger.x;
    float touchY = event.tfinger.y;
    float pressure = event.tfinger.pressure;
    
    std::cout << "finger down: " << "x: " << touchX << "y: " << touchY << "pressure: " << pressure << "up or down: " << upOrDown << std::endl;
}

void user::doFingerMove(const SDL_Event& event)
{
/*
* user::doFingerMove change varibles based on finger move
*/
    //To get the touch coordinates in screen coordinates simply multiply the touch coordinates by the screen resolution. 

    //From 0 to 1
    float touchX = event.tfinger.x;
    float touchY = event.tfinger.y;
    float pressure = event.tfinger.pressure;
    
    //From -1 to 1
    float xChange = event.tfinger.dx;
    float yChange = event.tfinger.dy;

    std::cout << "Finger moved: " << "x: " << touchX << "y: " << touchY << "pressure: " << pressure << std::endl;
    std::cout << "Change in x and y: " << "x: " << xChange << "y: " << yChange << std::endl;
}

void user::doMultiGesture(const SDL_Event& event)
{
/*
 * user::doMultiGesture handle pinch/rotate/swipe gestures
*/
    float gestureX = event.mgesture.x;
    float gestureY = event.mgesture.y;

    //the amount that the fingers rotated during this motion.
    float rotation = event.mgesture.dTheta;
    
    //the amount that the fingers pinched during this motion.
    float pinch = event.mgesture.dDist;

    short fingersTotal = event.mgesture.numFingers;

    std::cout << "Multi gesture: " << "x: " << gestureX << "y: " << gestureY << std::endl;
    std::cout << "Multi gesture: " << "rotation: " << rotation << "pinch: " << pinch << std::endl;
    std::cout << "Multi gesture fingers: " << fingersTotal << std::endl;
}

void user::updateForInput()
{
/*
 * user::updateForInput update the player based on input
*/
    if (playerUp)
    {
        y -= speed;

        direction = directions::up;
    }

    if (playerDown)
    {
        y += speed;

        direction = directions::down;
    }

    if (playerLeft)
    {
        x -= speed;

        direction = directions::left;
    }

    if (playerRight)
    {
        x += speed;

        direction = directions::right;
    }

    if (playerFired)
    {
        std::for_each(std::execution::par, bullets.begin(), bullets.end(), [](auto& currentBullet){ currentBullet->speed += 2; });

        bullets.push_back(std::make_unique<bulletClass>(x, y, 22, 22, 1, 2, "images/bullet.png"));
    }
}

user& user::input()
{
/*
* user::input handle event.type's by calling other functions then update based on the varibles 
*/
    //event types at https://wiki.libsdl.org/SDL2/SDL_EventType
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
	    case SDL_QUIT:
                exit(0);
	        break;

	    case SDL_KEYDOWN:
	        doKeyDown(event.key, true);
	        break;

	    case SDL_KEYUP:
    	        doKeyDown(event.key, false);
		break;

            case SDL_JOYAXISMOTION:
                doAxisMove(event);
                break;

            case SDL_JOYHATMOTION:
                doJoyHatMove(event);
                break;

            case SDL_CONTROLLERBUTTONDOWN:
                doButtonDown(event, true);
                break;

            case SDL_CONTROLLERBUTTONUP:
                doButtonDown(event, false);
                break;
            
            case SDL_JOYDEVICEADDED:
                addControllerSupport();
                break;

            case SDL_JOYDEVICEREMOVED:
                removeControllerSupport();
                break;

            case SDL_FINGERDOWN:
                doFingerDown(event, true);
                break;

            case SDL_FINGERMOTION:
                doFingerMove(event);
                break;

            case SDL_FINGERUP:
                doFingerDown(event, false);
                break;
            
            case SDL_MULTIGESTURE:
                doMultiGesture(event);
                break;            

	    default:
		break;
	    }
    }
   
    updateForInput();
 
    return *this;
}

user& user::keyMenu(bool& start, const SDL_KeyboardEvent& event)
{
/*
* user::keyMenu handle keyboard events on the start menu 
*/
    switch(event.keysym.scancode)
    {
        case SDL_SCANCODE_RETURN:
            start = true;
            break;

        default:
            break;
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
                keyMenu(start, event.key);
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
*
* Postcondition player's x and y are inside the screen
* Postcondition all bullets are updated
*/
    // Remember [x, y] where y is upside down
    if (x < 0)
    {
        x += back;
    }
    else if (x > App::get().SCREEN_WIDTH-w)
    {
        x -= back;
    }
    else if (y < 0)
    {
        y += back;
    }
    else if (y > App::get().SCREEN_HEIGHT-h)
    {
        y -= back;
    }  

    std::for_each(std::execution::par, bullets.begin(), bullets.end(), [this](auto& currentBullet){ currentBullet->logic(*this);}); 

    for(auto& currentBullet : bullets)
    {
        currentBullet->didBulletHit(enemy, playerScore);
        point.didYouGetPoints(*this, *currentBullet, playerScore);

        if(currentBullet->health <= 0)
            currentBullet.reset();
    }
    
    //Erasing in previous for range loop did not work
    std::erase_if(bullets, [](const auto& bullet){ return !bullet; });

    return *this;
}

bool user::show()
{
/*
* user::show show user, healthDiplay and bullets. Call playerDeath if health is less than zero
*/
    if(health > 0)
    {
        App::get().imagePos(Images[currentImage], x, y, w, h);
        
        playerHealth->healthDisplayShow(*this);
    
        for(const auto& currentBullet : bullets)
            currentBullet->show();
    }
    else
        playerDeath();
    
    return true;
}

void user::playerDeath()
{
/*
* user::playerDeath make a death screen and exit  
*/
    thing deathImage(0, 0, App::get().SCREEN_WIDTH, App::get().SCREEN_HEIGHT, 10, 0, "images/Death.png");

    deathImage.show();

    App::get().showVisuals();

    SDL_Delay(3000);

    exit(0);
}



enemys::enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path)
 : thing(ix, iy, iw, ih, ihealth, ispeed, path)
{
/*
* enemys::enemys construct enemys with thing constructor 
*/
}

enemys& enemys::spawnEnemys()
{
/*
* enemys::spawnEnemys if the spawn timer is up then spawn a enemy at SCREEN_WIDTH at random SCREEN_HEIGHT y value with a random speed
*
* Postcondition enemy is fairly random  
*/
    if(enemySpawnTimer <= 0 && health <= 0)
    {
        x = App::get().SCREEN_WIDTH;
        y = randomGen::get()(0, App::get().SCREEN_HEIGHT-w);

        speed = randomGen::get()(minimum, maximum);

        health = randomGen::get()(minimum, maximum);

        enemySpawnTimer = randomGen::get()(0, 100);
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
*
* Precondition make sure health is greater than zero 
*
* Postcondition set health to zero and remove from screen to prevent player taking extra damage
* Postcondition remove some health from player
*/
    if(collision(player, *this) && health > 0)
    {
        health = 0;
        player.minusHealth(1);
    }

    return *this;
}

void enemys::makeEnd(const user& player)
{
/*
* enemys::makeEnd create the end of the game with secret image 
*
* Precondition present renderer before pausing game
*
* Postcondition enemy is on screen when stoping game
* Postcondition set levelOne the one 
*/
    if(level == 0 && player.playerScore.count() > 400)
    {
        //Use secret image here 
        level = 1;
        speed = 1;

        App::get().imagePos(Images[currentImage], 100, 100, w, h);

        App::get().showVisuals();

        SDL_Delay(3000);
    }
}

enemys& enemys::scaleDifficulty(const counter& playerScore)
{
/*
* enemys::scaleDifficulty increase enemy diffculty as playerScore get higher
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



points::points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path)
 : thing(ix, iy, iw, ih, ihealth, ispeed, path)
{
/*
* points::points construct a points with the thing constructor
*/
}

points& points::initPoints()
{
/*
* points::initPoints randomly spawn a point somewhere on screen 
*
* Postcondition point position is fairly random
*/
    if (health > 0)
    {
        x -= speed;
        
        thing::logic();
    }
    else
    {
        x = App::get().SCREEN_WIDTH;
        y = randomGen::get()(0, App::get().SCREEN_HEIGHT-w);

        int randomNum = randomGen::get()(1, 9);
        health = randomGen::get()(randomNum, 10);
        speed = randomGen::get()(randomNum, 10);
    }

    return *this;
}

points& points::didYouGetPoints(user& player, thing& bullet, counter& playerScore)
{
/*
* points::didYouGetPoints detect collisions with point and update other objects accordingly 
*
*/
    bool playerPointCollision = collision(player, *this);
    bool bulletPointCollision = collision(bullet, *this);

    if(health > 0 && playerPointCollision)
    {
        health = 0;
        playerScore++;
    }
    else if(bulletPointCollision && bullet.getHealth() > 0 && health > 0)
    {
        bullet.minusHealth(-1);
        health = 0;
        playerScore++;
    }
    
    return *this;
}



bulletClass::bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path) 
: thing(ix, iy, iw, ih, ihealth, ispeed, path)
{
/*
* bulletClass::bulletClass construct a valid bulletClass
*/
}

bulletClass& bulletClass::logic(const user& player)
{
/*
* bulletClass::logic move bulletClass based on player direction and use thing::logic 
*/
    switch(player.getDirection())
    {
        case directions::up:
            y -= speed;
            break;

        case directions::down:
            y += speed;
            break;

        case directions::left:
            x -= speed;
            break;
        
        case directions::right:
            x += speed;
            break;

        default:
            break;
    }
    
    thing::logic();

    return *this;
}

inline bulletClass& bulletClass::didBulletHit(thing& enemy, counter& playerScore)
{
/*
* bulletClass::didBulletHit check bullet collision with enemy and update accordingly
*
* Precondition enemy and bullet health are greater than zero
*/
    if(collision(enemy, *this) && health > 0 && enemy.getHealth() > 0)
    {
        enemy.minusHealth(1);
        enemy.addToX( randomGen::get()(-1, int(enemy.getSpeed()/2) ) );
        
        health -= 1;
        
        if(!enemy.getHealth())
            playerScore++;
    }

    return *this;
}



healthDisplay::healthDisplay(const std::string& full, const std::string& half, const std::string& critical)
{
/*
* healthDisplay::healthDisplay construct healthDisplay  
*/
    healthImages["full"] = Image(full, 0, 0, 100, 100);
    healthImages["half"] = Image(half, 0, 0, 100, 100);
    healthImages["critical"] = Image(critical, 0, 0, 100, 100);
}

void healthDisplay::healthDisplayShow(const user& player)
{
/*
* healthDisplay::healthDisplayUpdate update healthDisplay based on player health
*/
    switch(player.getHealth())
    {
        case 3:
            currentHealth = "full";
            break;
        case 2:
	    currentHealth = "half";
            break;
	case 1:
            currentHealth = "critical";
            break;
    }
    
    App::get().imagePos(healthImages[currentHealth], 100, 0);
}



inline bool collision(const thing& first, const thing& second)
{
/*
 * collision use the thing api to get collision using collision
*/
    return collision(first.getX(), first.getY(), first.getW(), first.getH(), second.getX(), second.getY(), second.getW(), second.getH());
}

//Takes two objects dimetions
inline bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
/*
* collision check for any kind of collision for squares/rectangles
*/
    /*
     _____
    |_____|

    */
    // the | and | of the rectangle
    int leftSideOne = x1;
    int rightSideOne = x1 + w1;

    // the ___ and ___ of the rectangle
    int topSideOne = y1;
    int bottomSideOne = y1 + h1;
        
    int leftSideTwo = x2;
    int rightSideTwo = x2 + w2;
        
    int topSideTwo = y2;
    int bottomSideTwo = y2 + h2;
             //left less, right move                            up less, down more   
             //                 left2                   right2                       _________top2
             //      left1       ___________________________                         |________|________top1  
             //         ________|_____________<right1      |                         |        |        |
             //         |       |            |             |                         |________|bottom2 |
             //         |       |            |             |                         |                 |
             //         |_______|____________|             |                         |_________________|bottom1 
             //                 |__________________________|
             //                                                                      
    return ( (leftSideOne < rightSideTwo) && (rightSideOne > leftSideTwo) ) && ( (topSideOne < bottomSideTwo) && (bottomSideOne > topSideTwo) );
}

inline bool circleCollision(int center1X, int center1Y, double radius1, int center2X, int center2Y, double radius2)
{
/*
 * circleCollision Check if the distance between the two centers is less than the sum of the radiuses
*/
    //Get the minimum distance be touching
    double radiusSum = radius1+radius2;

    //Calculate the line between the circles
    double distanceBetween = std::sqrt( std::pow( (center2X - center1X), 2) + std::pow( (center2Y - center1Y), 2) );

    return (distanceBetween <= radiusSum);
}

inline bool rectangleCircleCollision(int centerX, int centerY, double radius, int rectX, int rectY, int width, int height)
{
/*
 * rectangleCircleCollision calclate collision with center coodinates and width, height and radius 
*/
    int distanceX = std::abs(centerX - rectX);
    int distanceY = std::abs(centerY - rectY);

    int halfWidth = width/2;
    int halfHeight = height/2;

    //Calculate if they are to far away from each other
    if ( ( distanceX > (halfWidth + radius) ) || ( distanceY > (halfHeight + radius) ) ) 
    { 
        return false; 
    }
    //Handle cases where it is close and easy to detect collision
    else if ( (distanceX <= halfWidth) || (distanceY <= halfHeight) ) 
    { 
        return true; 
    }

    //Handles courner collision 

    // finds the distance from the center of the circle and the corner
    int cornerDistance = (std::pow( (distanceX - halfWidth), 2) + std::pow( (distanceY - halfHeight), 2));

    //Check if the distances is no more than the radius
    return ( cornerDistance <= std::pow(radius, 2) );
}
