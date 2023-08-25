//Header for player.cpp
#pragma once

#include <iostream>
#include <string>
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
    thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path);

    thing(const thing& copyFromThing) = delete;
    thing& operator=(const thing& copyFromThing) = delete;

    thing(thing&& moveFromThing) = delete;
    thing& operator=(thing&& moveFromThing) = delete;

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
    inline thing& setSpeed(int setSpeed) noexcept;
    inline thing& setHealth(int setHealth);

    inline thing& removeFromScreen();
    inline thing& minusHealth(int subtractNum);
    inline thing& addToX(int addNum){ x += addNum; return *this; };

    protected:    
    int x;
    int y;
    int w;
    int h;
    int health;
    int speed;
    
    std::string currentImage = "default";
    std::unordered_map<std::string, Image> Images;
};

class counter
{
    public:
    counter();

    counter(const counter& copyFromCounter) = delete;
    counter& operator=(const counter& copyFromCounter) = delete;

    counter(counter&& moveFromCounter) = default;
    counter& operator=(counter&& moveFromCounter) = default;

    ~counter() = default;

    inline unsigned long long count() const { return currentCount; }
    std::string stringCurrentCount();
    counter& operator++(int);
    
    private:
    void updateStringCount();

    unsigned long long currentCount = 0;
    unsigned long long oldCount = 0;

    std::string stringCount;
};

enum class directions : unsigned char {up, down, left, right, none};

class user : public thing
{
    public:
    explicit user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path, char iback);

    user(const user& copyFromUser) = delete;
    user& operator=(const user& copyFromUser) = delete;

    user(user&& moveFromUser) = delete;
    user& operator=(user&& moveFromUser) = delete;

    ~user() = default;

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

    void updateForInput();
    
    void doKeyDown(const SDL_KeyboardEvent& event, bool DownUp);

    bool useController = false;
    directions joystickDirection = directions::none;
    SDL_Pointer<SDL_Joystick, SDL_JoystickClose> joystickOne;
    SDL_Pointer<SDL_GameController, SDL_GameControllerClose> gameController;
    
    void doButtonDown(const SDL_Event& event, bool DownOrUp);
    void doAxisMove(const SDL_Event& event);
    void doJoyHatMove(const SDL_Event& event);
    void addControllerSupport();
    void removeControllerSupport();

    bool useTouchScreen = false;
    SDL_TouchID touchDeviceID;

    void doFingerDown(const SDL_Event& event, bool upOrDown);
    void doFingerMove(const SDL_Event& event);
    void doMultiGesture(const SDL_Event& event);

    std::unique_ptr<healthDisplay> playerHealth;

    std::vector<std::unique_ptr<bulletClass>> bullets;
};

class enemys : public thing
{
    public:
    explicit enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path);

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
    explicit points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path);
    
    points(const points& copyFromPoint) = delete;
    points& operator=(const points& copyFromPoint) = delete;
    
    points(points&& moveFromPoint) = delete;
    points& operator=(points&& moveFromPoint) = delete;
    
    ~points() = default;

    points& initPoints();
    points& didYouGetPoints(user& player, thing& bullet, counter& playerScore);
};

class bulletClass : public thing
{
    friend user;

    public:
    explicit bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, const std::string& path);

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
    explicit healthDisplay(const std::string& full, const std::string& health, const std::string& critical);

    healthDisplay(const healthDisplay& copyFromHealthDisplay) = delete;
    healthDisplay& operator=(const healthDisplay& copyFromHealthDisplay) = delete;

    healthDisplay(healthDisplay&& moveFromHealthDisplay) = delete;
    healthDisplay& operator=(healthDisplay&& moveFromHealthDisplay) = delete;

    ~healthDisplay() = default;

    void healthDisplayShow(const user& player);
    
    std::string currentHealth;
    std::unordered_map<std::string, Image> healthImages; 
};

inline bool collision(const thing& first, const thing& second);

inline bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

inline bool circleCollision(int center1X, int center1Y, double radius1, int center2X, int center2Y, double radius2);

inline bool rectangeCircleCollision(int centerX, int centerY, double radius, int rectX, int rectY, int width, int height);
