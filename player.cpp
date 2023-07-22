#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <future>
#include <execution>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "headerPlayer.h"
#include "headerVisuals.h"

App* thing::appPointer;

thing::thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path)
 : x{ix}, y{iy}, w{iw}, h{ih}, health{ihealth}, speed{ispeed}
{
/*
* thing::thing create a thing with the x y w h texture and app pointer 
* 
* pre and postconditions:
* 
* Precondition iw and ih can not be less or equal to zero 
* Precondition ihealth can not be less than zero
* Precondition static appPointer is defined
* 
* Postcondition creates a default Image from path
*/    
    if(iw <= 0 || ih <= 0)
    {
        std::cerr << "thing w or height can not be less than or equal to zero" << std::endl;
    }
    else if(ihealth < 0)
    {
        std::cerr << "health can not be less than zero" << std::endl;
    }
    
    Images["default"] = Image(path, *appPointer, 0, 0, iw, ih);
}

thing::thing(thing&& moveFromThing)
 : x{moveFromThing.x}, y{moveFromThing.y}, w{moveFromThing.w}, h{moveFromThing.h}, 
 health{moveFromThing.health}, speed(moveFromThing.speed), currentImage{std::move(moveFromThing.currentImage)},
 Images{std::move(moveFromThing.Images)}
{
/*
* thing::thing move constructor moves thing
*
* Postcondition all values in thing are moved from thing to new thing
* Postcondition moveFromThing does not own anything that new thing does
* Postcondition moveFromThing is not a copy of new thing
*/
    moveFromThing.x = 0;
    moveFromThing.y = 0;
    moveFromThing.w = 0;
    moveFromThing.h = 0;
    moveFromThing.health = 0;
    moveFromThing.speed = 0;
}

thing& thing::operator=(thing&& moveFromThing)
{
/*
* thing::operator= move moveFrom thing to an already existing thing 
* 
* Precondition no self assigment 
* Precondition anything this owns is destroyed before move
*
* Postcondition all thing::thing(thing&&) postconditions must be met
*/
    if(this != &moveFromThing)
    {
        x = moveFromThing.x;
        y = moveFromThing.y;
        w = moveFromThing.w;
        h = moveFromThing.h;
        health = moveFromThing.health;
        speed = moveFromThing.speed;
        currentImage = std::move(moveFromThing.currentImage);
        Images = std::move(moveFromThing.Images);

        moveFromThing.x = 0;
        moveFromThing.y = 0;
        moveFromThing.w = 0;
        moveFromThing.h = 0;
        moveFromThing.health = 0;
        moveFromThing.speed = 0;
    }

    return *this;
}

thing& thing::logic()
{
/*
* thing::logic sets health equal to zero if thing is outside SCREEN_WIDTH and SCREEN_HEIGHT
*
* Postcondition thing has no health if outside of screen
*/
    if (x > appPointer->SCREEN_WIDTH || x < 0 || y > appPointer->SCREEN_HEIGHT || y < 0)
        health = 0;

    return *this;
}

int thing::show()
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
        appPointer->imagePos(Images[currentImage], x, y, w, h);
        Images[currentImage]++;

    if(currentImage != "default" && Images[currentImage].done())
        currentImage = "default";
    
    return health > 0;
}

inline thing& thing::setX(int setX)
{
/*
* thing::setX set x equal to setX
* 
* Precondition setX is in the window 
*/
    if(setX > appPointer->SCREEN_WIDTH || setX < 0)
        std::cerr << "x can not be less than screen_width or greater than zero" << std::endl;    

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
    if(setY > appPointer->SCREEN_HEIGHT || setY < 0)
        std::cerr << "y can not be less than screen_height or greater than zero" << std::endl;

    y = setY;

    return *this;
}

inline thing& thing::setW(int setW)
{
/*
* thing::setW set w equal to setW
*
* Precondition setW is not less than or equal to zero
*/
    if(setW <= 0)
        std::cerr << "w can not be less than or equal to zero" << std::endl;

    w = setW;

    return *this;
}

inline thing& thing::setH(int setH)
{
/*
* thing::setH set h equal to setH
*
* Precondition setH is not less than or equal to zero
*/
    if(setH <= 0)
        std::cerr << "h can not be less than or equal to zero" << std::endl;

    h = setH;

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

counter::~counter(){}

std::string counter::stringCurrentCount()
{
/*
* counter::stringCurrentCount return current count as a string update if needed 
*
* Postcondition does not update if not needed
* Postcondition returns a string equal to the current count
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



user::user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path, char iback) 
 : thing(ix, iy, iw, ih, ihealth, ispeed, path), back(iback), joystickOne(nullptr), gameController(nullptr)
{
/*
* user::user constructs a valid user 
*
* Postcondition user has a health display 
* Postcondition if there are controllers/joysticks user opens them 
*/
    playerHealth.reset(new healthDisplay{"images/Health1.jpg", "images/Health2.jpg", "images/Health3.jpg", *appPointer});

    addControllerSupport();    
 
    if(SDL_GetNumTouchDevices() > 0)
    {
        useTouchScreen = true;

        if(SDL_GetTouchDevice(0) == 0)
        {
            std::cerr << "SDL_GetTouchDevice failed: " << SDL_GetError() << std::endl;
        }

        touchDeviceID = SDL_GetTouchDevice(0);

        std::cout << "Found touch device" << std::endl;
        std::cout << "Touch devices: " << SDL_GetNumTouchDevices() << std::endl;
        std::cout << "Fingers down: " << SDL_GetNumTouchFingers(touchDeviceID) << std::endl; 
    }

    std::cout << "Tasks: " << std::thread::hardware_concurrency() << std::endl;
}

user::~user()
{
/*
* user::~user free users resources 
*
* Postcondition all bullets in bullets are deleted
* Postcondition bullets is empty
*/
    for(auto& currentBullet : bullets)
    {
        delete currentBullet;
    }

    bullets.clear();
}

//Scan codes at https://wiki.libsdl.org/SDL2/SDL_Scancode
user& user::doKeyDown(const SDL_KeyboardEvent& event, bool DownUp)
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
    }

    return *this;
}

user& user::doButtonDown(const SDL_Event& event, bool upOrDown)
{
/*
* user::doButtonDown handles controller button events 
*/
    //Buttons at https://wiki.libsdl.org/SDL2/SDL_GameControllerButton
    switch(event.cbutton.button)
    {
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            std::cout << "RIGHTSHOULDER" << std::endl;
            break;

        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            std::cout << "LEFTSHOULDER" << std::endl;
            break;
    }
    
    return *this;
}

user& user::doAxisMove(const SDL_Event& event)
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
    SDL_JoystickID which = event.jaxis.which;
    
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
            if(value > ignoreZone || value > softIgnoreZone && joystickDirection == directions::right)
            {
                playerRight = true;
               joystickDirection = directions::right;
            }
            else if(value < -ignoreZone || value < -softIgnoreZone && joystickDirection == directions::left)
            {
                playerLeft = true;
               joystickDirection = directions::left;
            }
            break;
        
        case SDL_CONTROLLER_AXIS_RIGHTX:
            if(value > ignoreZone || value > softIgnoreZone && joystickDirection == directions::right)
            {
                playerRight = true;
               joystickDirection = directions::right;
            }
            else if(value < -ignoreZone || value < -softIgnoreZone && joystickDirection == directions::left)
            {
                playerLeft = true;
               joystickDirection = directions::left;
            }
            break;

        case SDL_CONTROLLER_AXIS_LEFTY:
            if(value > ignoreZone || value > softIgnoreZone && joystickDirection == directions::up)
            {
                playerDown = true;
                joystickDirection = directions::up;
            }
            else if(value < -ignoreZone || value < -softIgnoreZone && joystickDirection == directions::down)
            {
                playerUp = true;
                joystickDirection = directions::down;
            }
            break;

        case SDL_CONTROLLER_AXIS_RIGHTY:
            if(value > ignoreZone || value > softIgnoreZone && joystickDirection == directions::up)
            {
                playerDown = true;
                joystickDirection = directions::up;
            }
            else if(value < -ignoreZone || value < -softIgnoreZone && joystickDirection == directions::down)
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
    
    return *this;
}

user& user::doBallMove(const SDL_Event& event)
{
/*
* user::doBallMove handle controller ball input
*/
    //event.jball
    //https://wiki.libsdl.org/SDL2/SDL_JoyBallEvent
    return *this;
}

user& user::doJoyHatMove(const SDL_Event& event)
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
    
    return *this;
}

user& user::addControllerSupport()
{
/*
 * user::addControllerSupport opens controller and joystick and enables joystick events 
*/
    if(SDL_NumJoysticks() > 0 && SDL_IsGameController(0))
    {
        useController = true;

        gameController.reset(SDL_GameControllerOpen(0));

        joystickOne.reset(SDL_GameControllerGetJoystick(gameController.get()));

        if(!joystickOne)
        {
            std::cerr << "SDL_GameControllerGetJoystick failed: " << SDL_GetError() << std::endl;
        }

        if(!gameController)
        {
            std::cerr << "SDL_GameControllerOpen failed: " << SDL_GetError() << std::endl;
        }

        std::cout << "Found game controller: " << SDL_GameControllerName(gameController.get()) << std::endl;
        std::cout << "NumJoysticks: " << SDL_NumJoysticks() << std::endl;
        std::cout << "joystick axes num: " << SDL_JoystickNumAxes(joystickOne.get()) << std::endl;
        std::cout << "joystick num buttons: " << SDL_JoystickNumButtons(joystickOne.get()) << std::endl;
        std::cout << "joystick num balls: " << SDL_JoystickNumBalls(joystickOne.get()) << std::endl;
        std::cout << "joystick num hats: " << SDL_JoystickNumHats(joystickOne.get()) << std::endl;

        SDL_JoystickEventState(SDL_ENABLE);
    }

    return *this;
}

user& user::removeControllerSupport()
{
/*
 * user::removeControllerSupport sets joystick events to ignore 
*/
    useController = false;

    SDL_JoystickEventState(SDL_IGNORE);
    
    return *this;
}

user& user::doFingerDown(const SDL_Event& event, bool upOrDown)
{
/*
* user::doFingerDown change varibles based on touch event varibles
*/
    //To get the touch coordinates in screen coordinates simply multiply the touch coordinates by the screen resolution. 

    SDL_TouchID currentTouchDeviceID = event.tfinger.touchId;
    SDL_FingerID currentFingerID = event.tfinger.fingerId;

    //From 0 to 1
    float touchX = event.tfinger.x;
    float touchY = event.tfinger.y;
    float pressure = event.tfinger.pressure;
    
    return *this;
}

user& user::doFingerMove(const SDL_Event& event)
{
/*
* user::doFingerMove change varibles based on finger move
*/
    //To get the touch coordinates in screen coordinates simply multiply the touch coordinates by the screen resolution. 

    SDL_TouchID currentTouchDeviceID = event.tfinger.touchId;
    SDL_FingerID currentFingerID = event.tfinger.fingerId;

    //From 0 to 1
    float touchX = event.tfinger.x;
    float touchY = event.tfinger.y;
    float pressure = event.tfinger.pressure;
    
    //From -1 to 1
    float xChange = event.tfinger.dx;
    float yChange = event.tfinger.dy;
    
    return *this;
}

user& user::doMultiGesture(const SDL_Event& event)
{
/*
 * user::doMultiGesture handle pinch/rotate/swipe gestures
*/
    SDL_TouchID currentTouchDeviceID = event.mgesture.touchId;
    
    float gestureX = event.mgesture.x;
    float gestureY = event.mgesture.y;

    //the amount that the fingers rotated during this motion.
    float rotationFingers = event.mgesture.dTheta;
    
    //the amount that the fingers pinched during this motion.
    float pinchFingers = event.mgesture.dDist;

    short fingersTotal = event.mgesture.numFingers;

    return *this;
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

            case SDL_JOYBALLMOTION:
                doBallMove(event);
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
        std::for_each(std::execution::par, bullets.begin(), bullets.end(), [](bulletClass* currentBullet){ currentBullet->speed += 2; });
            
        bulletClass* newBulletClass = new bulletClass{x, y, 22, 22, 1, 2, "images/bullet.png"}; 

        bullets.push_back(newBulletClass);
    }
    
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

    std::for_each(std::execution::par, bullets.begin(), bullets.end(), [this](bulletClass* currentBullet){ currentBullet->logic(*this);}); 

    for(auto& currentBullet : bullets)
    {
        currentBullet->didBulletHit(enemy, playerScore);
        point.didYouGetPoints(*this, *currentBullet, playerScore);

        if(currentBullet->health <= 0)
        {
            delete currentBullet;
            currentBullet = nullptr;
        }
    }
    
    //Erasing in previous for range loop did not work
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
* user::show show user, healthDiplay and bullets. Call playerDeath if health is less than zero
*/
    if(health > 0)
    {
        appPointer->imagePos(Images[currentImage], x, y, w, h);
        
        playerHealth->healthDisplayShow(*this, *appPointer);
    
        for(const auto& currentBullet : bullets)
        {
            currentBullet->show();
        }
    }
    else
    {
        playerDeath();
    }
    
    return true;
}

void user::playerDeath()
{
/*
* user::playerDeath make a death screen and exit  
*/
    thing deathImage(0, 0, appPointer->SCREEN_WIDTH, appPointer->SCREEN_HEIGHT, 10, 0, "images/Death.jpg");

    deathImage.show();

    appPointer->showVisuals();

    SDL_Delay(3000);

    exit(0);
}



enemys::enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path)
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
*
* Precondition make sure health is greater than zero 
*
* Postcondition set health to zero and remove from screen to prevent player taking extra damage
* Postcondition remove some health from player
*/
    if(collision(player.getX(), player.getY(), player.getW(), player.getH(), x, y, w, h) && health > 0)
    {
        health = 0;
        removeFromScreen();
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

        appPointer->imagePos(Images[currentImage], 100, 100, w, h);

        appPointer->showVisuals();

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



points::points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path)
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
        x = appPointer->SCREEN_WIDTH;
        y = rand() % appPointer->SCREEN_HEIGHT;

        randomNum = 1 + (rand() % 9);
        health = randomNum;
        speed = randomNum;

        isHealth = false;
    }

    return *this;
}

points& points::didYouGetPoints(user& player, thing& bullet, counter& playerScore)
{
/*
* points::didYouGetPoints detect collisions with point and update other objects accordingly 
*
*/
    bool playerPointCollision = collision( player.getX(), player.getY(), player.getW(), player.getH(), x, y, w, h);
    bool bulletPointCollision = collision( bullet.getX(), bullet.getY(), bullet.getW(), bullet.getH(), x, y, w, h);

    if(health != 0 && playerPointCollision)
    {
        if(health > player.getHealth() && isHealth)
        {
	        int randomNum = rand() % 2;

	        if(randomNum)
	        {
                    player.minusHealth(-1);
	        }
        }
        
        health = 0;
        playerScore++;
    }
    else if(bulletPointCollision && bullet.getHealth() > 0 && health > 0 && !isHealth)
    {
        bullet.minusHealth(-1);
        isHealth = true;
        playerScore++;
    }
    
    return *this;
}



bulletClass::bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path) 
: thing(ix, iy, iw, ih, ihealth, ispeed, path)
{
/*
* bulletClass::bulletClass construct a valid bulletClass
*/
};

bulletClass& bulletClass::logic(const user& player)
{
/*
* bulletClass::logic move bulletClass based on player direction and use thing::logic 
*/
    if(player.getDirection() == directions::up)
        y -= speed;
    else if(player.getDirection() == directions::down)
        y += speed;
    else if(player.getDirection() == directions::left)
        x -= speed;
    else if(player.getDirection() == directions::right)
        x += speed;

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
    if(collision(x, y, w, h, enemy.getX(), enemy.getY(), enemy.getW(), enemy.getH()) && health > 0 && enemy.getHealth() > 0)
    {
        enemy.setHealth(0);
        enemy.removeFromScreen();
        health -= 1;
        playerScore++;
    }

    return *this;
}



healthDisplay::healthDisplay(std::string_view full, std::string_view half, std::string_view critical, App& app)
{
/*
* healthDisplay::healthDisplay construct healthDisplay  
*/
    healthImages["full"] = Image(full, app, 0, 0, 100, 100);
    healthImages["half"] = Image(half, app, 0, 0, 100, 100);
    healthImages["critical"] = Image(critical, app, 0, 0, 100, 100);
}

void healthDisplay::healthDisplayShow(const user& player, App& app)
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
    
    app.imagePos(healthImages[currentHealth], 100, 0);
}



//Takes two objects dimetions
inline bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
/*
* chech for any kind of collision for squares/rectangles
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
