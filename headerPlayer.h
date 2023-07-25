//Header for player.cpp
#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <random>
#include <chrono>

#include <cassert>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "headerVisuals.h"

class thing;
class counter;
class user;
class enemys;
class points;
class bulletClass;
class healthDisplay;

class thing
{
    public:
    thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path);

    thing(const thing& copyFromThing) = delete;
    thing& operator=(const thing& copyFromThing) = delete;

    thing(thing&& moveFromThing);
    thing& operator=(thing&& moveFromThing);

    virtual ~thing() = default;

    virtual thing& logic();
    virtual bool show();

    inline int getX() const{ return x; }
    inline int getY() const{ return y; }
    inline int getW() const{ return w; } 
    inline int getH() const{ return h; }
    inline int getHealth() const{ return health; }
    inline int getSpeed() const{ return speed; }

    inline thing& setX(int setX);
    inline thing& setY(int setY);
    inline thing& setW(int setW);
    inline thing& setH(int setH);
    inline thing& setSpeed(int setSpeed) noexcept;
    inline thing& setHealth(int setHealth);
    static void setApp(App* app){ appPointer = app; }

    inline thing& removeFromScreen();
    inline thing& minusHealth(int subtractNum);

    protected:    
    int x;
    int y;
    int w;
    int h;
    int health;
    int speed;
    
    std::string currentImage = "default";
    std::unordered_map<std::string, Image> Images;
    
    static App* appPointer;
};

class counter
{
    public:
    counter();

    counter(const counter& copyFromCounter) = delete;
    counter& operator=(const counter& copyFromCounter) = delete;

    counter(counter&& moveFromCounter) = delete;
    counter& operator=(counter&& moveFromCounter) = delete;

    ~counter();

    inline unsigned long long count() const { return currentCount; }
    std::string stringCurrentCount();
    counter& operator++(int);
    
    private:
    void updateStringCount();

    unsigned long long currentCount = 0;
    unsigned long long oldCount = 0;

    std::string stringCount;
};

enum class directions: unsigned char {up, down, left, right, none};

class user : public thing
{
    public:
    explicit user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path, char iback);

    user(const user& copyFromUser) = delete;
    user& operator=(const user& copyFromUser) = delete;

    user(user&& moveFromUser) = delete;
    user& operator=(user&& moveFromUser) = delete;

    ~user();

    user& input();

    user& keyMenu(bool& start, const SDL_KeyboardEvent& event);
    user& menuInput(bool& start);

    user& logic(thing& enemy, points& point);

    bool show() override;

    directions getDirection() const{ return direction; };

    counter playerScore;

    private:
    void playerDeath();
    
    directions direction = directions::right;
    char back;

    bool animationCanCancel = true;

    bool playerUp = false;
    bool playerDown = false;
    bool playerLeft = false;
    bool playerRight = false;
    bool playerFired = false;
    
    user& doKeyDown(const SDL_KeyboardEvent& event, bool DownUp);

    bool useController = false;
    directions joystickDirection = directions::none;
    SDL_Pointer<SDL_Joystick, SDL_JoystickClose> joystickOne;
    SDL_Pointer<SDL_GameController, SDL_GameControllerClose> gameController;
    
    user& doButtonDown(const SDL_Event& event, bool DownOrUp);
    user& doAxisMove(const SDL_Event& event);
    user& doBallMove(const SDL_Event& event);
    user& doJoyHatMove(const SDL_Event& event);
    user& addControllerSupport();
    user& removeControllerSupport();

    bool useTouchScreen = false;
    SDL_TouchID touchDeviceID;

    user& doFingerDown(const SDL_Event& event, bool upOrDown);
    user& doFingerMove(const SDL_Event& event);
    user& doMultiGesture(const SDL_Event& event);

    std::unique_ptr<healthDisplay> playerHealth;

    std::vector<bulletClass*> bullets;
};

class enemys : public thing
{
    public:
    explicit enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path);

    enemys(const enemys& copyFromEnemy) = delete;
    enemys& operator=(const enemys& copyFromEnemy) = delete;

    enemys(enemys&& moveFromEnemy) = delete;
    enemys& operator=(enemys&& moveFromEnemy) = delete;
    
    ~enemys() = default;

    enemys& spawnEnemys();
    enemys& didEnemyKill(user& player);
    void makeEnd(const user& player);
    enemys& scaleDifficulty(const counter& playerScore);

    private:
    int minimum = 1;
    int maximum = 15;
    int enemySpawnTimer = 0;
    int level = 0;
};

class points : public thing
{
    public:
    explicit points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path);
    
    points(const points& copyFromPoint) = delete;
    points& operator=(const points& copyFromPoint) = delete;
    
    points(points&& moveFromPoint) = delete;
    points& operator=(points&& moveFromPoint) = delete;
    
    ~points() = default;

    points& initPoints();
    points& didYouGetPoints(user& player, thing& bullet, counter& playerScore);

    private:
    int randomNum;
    bool isHealth = false;
};

class bulletClass : public thing
{
    friend user;

    public:
    explicit bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string_view path);

    bulletClass(const bulletClass& copyFromBullet) = delete;   
    bulletClass& operator=(const bulletClass& copyFromBullet) = delete;

    bulletClass(bulletClass&& moveFromBullet) = delete;
    bulletClass& operator=(bulletClass&& moveFromBullet) = delete;

    ~bulletClass() = default;

    bulletClass& logic(const user& player);
    inline bulletClass& didBulletHit(thing& enemy, counter& playerScore);
};

class healthDisplay
{
    public:     
    explicit healthDisplay(std::string_view full, std::string_view health, std::string_view critical, App& app);

    healthDisplay(const healthDisplay& copyFromHealthDisplay) = delete;
    healthDisplay& operator=(const healthDisplay& copyFromHealthDisplay) = delete;

    healthDisplay(healthDisplay&& moveFromHealthDisplay) = delete;
    healthDisplay& operator=(healthDisplay&& moveFromHealthDisplay) = delete;

    ~healthDisplay() = default;

    void healthDisplayShow(const user& player, App& app);
    
    std::string currentHealth;
    std::unordered_map<std::string, Image> healthImages; 
};

inline bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
