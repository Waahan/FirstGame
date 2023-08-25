#pragma once

#include <vector>
#include <memory>

#include "headerVectorMath.h"

namespace appleTree
{
    class tree;

    class squareCollider;
    class circleCollider;

    class collider
    {
        public:
        collider() = default;

        collider(const collider& copyFrom) = delete;
        collider& operator=(const collider& copyFrom) = delete;

        collider(collider&& moveFrom) = delete;
        collider& operator=(collider&& moveFrom) = delete;

        ~collider() = default;

        virtual collider& setX(int setX) =0;
        virtual collider& setY(int setY) =0;

        virtual bool collision(const collider& other) const =0;
        virtual bool collision(const squareCollider& other) const =0;
        virtual bool collision(const circleCollider& other) const =0;
    };

    class squareCollider : public collider
    {
        public:
        squareCollider(int setWidth, int setHeight) : x{}, y{}, width{setWidth}, height{setHeight} {}

        squareCollider(const squareCollider& copyFrom) = default; 
        squareCollider& operator=(const squareCollider& copyFrom) = default; 

        squareCollider(squareCollider&& moveFrom) = default; 
        squareCollider& operator=(squareCollider&& moveFrom) = default; 

        ~squareCollider() = default;

        int getX() const { return x; }
        int getY() const { return y; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        
        squareCollider& setX(int setX) override { x = setX; return *this; }
        squareCollider& setY(int setY) override { y = setY; return *this; }

        bool collision(const collider& other) const override { return other.collision(*this); }
        bool collision(const squareCollider& other) const override;
        bool collision(const circleCollider& other) const override; 

        private:
        int x, y, width, height;
    };

    class circleCollider : public collider
    {
        public:
        circleCollider(double setRadius) : x{}, y{}, radius{setRadius} {}

        circleCollider(const circleCollider& copyFrom) = default;
        circleCollider& operator=(const circleCollider& copyFrom) = default;

        circleCollider(circleCollider&& moveFrom) = default;
        circleCollider& operator=(circleCollider&& moveFrom) = default;

        ~circleCollider() = default;
        
        int getX() const { return x; }
        int getY() const { return y; }
        double getRadius() const { return radius; }

        circleCollider& setX(int setX) override { x = setX; return *this; }
        circleCollider& setY(int setY) override { y = setY; return *this; }
        
        bool collision(const collider& other) const override { return other.collision(*this); }
        bool collision(const squareCollider& other) const override;
        bool collision(const circleCollider& other) const override;

        private:
        int x, y;
        double radius;
    };

    enum class appleType : unsigned char { dynamicApple = 0, staticApple };

    class apple
    {
        friend class tree;

        public:
        explicit apple(vector2f pos = vector2f{}, appleType setType = appleType::staticApple, float setMass = 1.0f);

        apple(const apple& copyFrom) = default;
        apple& operator=(const apple& copyFrom) = default;

        apple(apple&& moveFrom) = default;
        apple& operator=(apple&& moveFrom) = default;
        
        ~apple();

        vector2f getPosition() const { return position; }

        apple& setTree(tree* newTree) { appleTree = newTree; return *this; }
        apple& setCollider(collider* newCollider) { appleCollider = newCollider; return *this; }
        apple& addForce(vector2f add) { if(!(type == appleType::staticApple)) force += add; return *this; }
        void updateCollider() { if(appleCollider){ appleCollider->setX(position.getX()); appleCollider->setY(position.getY()); } }

        private:
        vector2f position;
        vector2f force{};

        appleType type;
        tree* appleTree = nullptr;
        collider* appleCollider = nullptr;
        float mass;
    };

    struct foundCollision
    {
        explicit foundCollision(apple* apple1, apple* apple2) : firstApple{apple1}, secondApple{apple2} {}

        foundCollision(const foundCollision& copyFrom) = delete;
        foundCollision& operator=(const foundCollision& copyFrom) = delete;

        foundCollision(foundCollision&& moveFrom) = default;
        foundCollision& operator=(foundCollision&& moveFrom) = default;

        ~foundCollision() = default;

        apple* firstApple;
        apple* secondApple;
    };

    class tree
    {
        public:
        explicit tree(const vector2f& setGravity);

        tree(const tree& copyFrom) = delete;
        tree& operator=(const tree& copyFrom) = delete;

        tree(tree&& moveFrom) = default;
        tree& operator=(tree&& moveFrom) = default;

        ~tree() = default;
        
        vector2f getGravity() const { return gravity; }
        void newGravity(const vector2f& newGravity) { gravity = newGravity; }

        tree& addApple(apple* newApple);
        tree& removeApple(apple* removeApple);

        void simulate(float timeStep);

        private:
        void simulateCollisions();
        void handleCollisions(const std::vector<foundCollision>& collisions);

        vector2f gravity;
        std::vector<apple*> apples;
    };

    inline bool collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

    inline bool collision(int center1X, int center1Y, double radius1, int center2X, int center2Y, double radius2);

    inline bool collision(int centerX, int centerY, double radius, int rectX, int rectY, int width, int height);
}
