//Header for player.cpp
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <future>

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
    thing(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string path, App* iappPointer);

    thing(const thing& copyFromThing) = delete;
    thing& operator=(const thing& copyFromThing) = delete;

    thing(thing&& moveFromThing);
    thing& operator=(thing&& moveFromThing);

    virtual thing& logic();
    virtual int show();

    inline int getX() const;
    inline int getY() const;
    inline int getW() const;
    inline int getH() const;
    inline int getHealth() const;
    inline int getSpeed() const;

    inline thing& setX(int setX);
    inline thing& setY(int setY);
    inline thing& setW(int setW);
    inline thing& setH(int setH);
    inline thing& setSpeed(int setSpeed) noexcept;
    inline thing& setHealth(int setHealth);

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

    //Mabe make this static
    App* appPointer = NULL;
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

class user : public thing
{
    public:
    explicit user(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string path, App* iappPointer, int iback, int idirection);

    user(const user& copyFromUser) = delete;
    user& operator=(const user& copyFromUser) = delete;

    user(user&& moveFromUser) = delete;
    user& operator=(user&& moveFromUser) = delete;

    ~user();

    user& doKeyDown(SDL_KeyboardEvent *event, bool DownUp);
    user& doButtonDown(const SDL_Event& event, bool DownOrUp);

    user& doAxisMove(const SDL_Event& event);
    user& doBallMove(const SDL_Event& event);

    user& input();

    user& keyMenu(bool& start, SDL_KeyboardEvent *event);
    user& menuInput(bool& start);

    user& logic(thing& enemy, points& point);

    int show() override;
    void playerDeath();

    inline int getDirection() const;

    counter playerScore;

    private:
    enum class directions: unsigned char { up, down, left, right, none};

    //Change direction to directions ^
    char direction;
    char back;
    bool playerUp = false;
    bool playerDown = false;
    bool playerLeft = false;
    bool playerRight = false;
    bool playerFired = false;

    bool useController = false;
    SDL_Pointer<SDL_Joystick, SDL_JoystickClose> joystickOne;
    SDL_Pointer<SDL_GameController, SDL_GameControllerClose> gameController;
    directions joystickDirection;

    std::unique_ptr<healthDisplay> playerHealth;

    std::vector<bulletClass*> bullets;
};

class enemys : public thing
{
    public:
    explicit enemys(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string path, App* iappPointer);

    enemys(const enemys& copyFromEnemy) = delete;
    enemys& operator=(const enemys& copyFromEnemy) = delete;

    enemys(enemys&& moveFromEnemy) = delete;
    enemys& operator=(enemys&& moveFromEnemy) = delete;

    enemys& spawnEnemys();
    enemys& didEnemyKill(user& player);
    void makeEnd(int& levelOne);
    enemys& scaleDifficulty(const counter& playerScore);

    private:
    int minimum = 1;
    int maximum = 15;
    int enemySpawnTimer = 0;
};

class points : public thing
{
    public:
    explicit points(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string path, App* iappPointer);
    
    points(const points& copyFromPoint) = delete;
    points& operator=(const points& copyFromPoint) = delete;
    
    points(points&& moveFromPoint) = delete;
    points& operator=(points&& moveFromPoint) = delete;

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
    explicit bulletClass(int ix, int iy, int iw, int ih, int ihealth, int ispeed, std::string path, App* iappPointer);

    bulletClass(const bulletClass& copyFromBullet) = delete;   
    bulletClass& operator=(const bulletClass& copyFromBullet) = delete;

    bulletClass(bulletClass&& moveFromBullet) = delete;
    bulletClass& operator=(bulletClass&& moveFromBullet) = delete;

    bulletClass& logic(const user& player);
    inline bulletClass& didBulletHit(thing& enemy, counter& playerScore);
};

class healthDisplay
{
    public:     
    explicit healthDisplay(std::string full, std::string health, std::string critical, App& app);

    healthDisplay(const healthDisplay& copyFromHealthDisplay) = delete;
    healthDisplay& operator=(const healthDisplay& copyFromHealthDisplay) = delete;

    healthDisplay(healthDisplay&& moveFromHealthDisplay) = delete;
    healthDisplay& operator=(healthDisplay&& moveFromHealthDisplay) = delete;

    void healthDisplayShow(const user& player, App& app);
    
    std::string currentHealth;
    std::unordered_map<std::string, Image> healthImages; 
};

inline bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
