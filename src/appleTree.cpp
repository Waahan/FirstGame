#include <iostream>
#include <vector>
#include <memory>

#include <cassert>
#include <cmath>

#include "headerAppleTree.h"

namespace appleTree
{
    bool squareCollider::collision(const squareCollider& other) const 
    {
        return appleTree::collision(other.getX(), other.getY(), other.getWidth(), other.getHeight(), getX(), getY(), getWidth(), getHeight());
    }

    bool squareCollider::collision(const circleCollider& other) const
    {
        return appleTree::collision(other.getX(), other.getY(), other.getRadius(), getX(), getY(), getWidth(), getHeight());
    }

    bool circleCollider::collision(const squareCollider& other) const
    {
        return appleTree::collision(getX(), getY(), getRadius(), other.getX(), other.getY(), other.getWidth(), other.getHeight());
    }

    bool circleCollider::collision(const circleCollider& other) const
    {
        return appleTree::collision(getX(), getY(), getRadius(), other.getX(), other.getY(), other.getRadius());
    }

    apple::apple(vector2f pos, appleType setType, float setMass)
     : position{pos}, type{setType}, mass{setMass}
    {

    }

    apple::~apple()
    {
        if(appleTree)
            appleTree->removeApple(this);
    }
    
    tree::tree(const vector2f& setGravity)
     : gravity{setGravity}, apples{}
    {
    }

    tree& tree::addApple(apple* newApple)
    {
        assert(newApple && "newApple can not be nullptr");

        newApple->setTree(this);

        apples.push_back(newApple);

        return *this;
    }

    tree& tree::removeApple(apple* removeApple)
    {
        apples.erase(std::remove(apples.begin(), apples.end(), removeApple), apples.end());

        return *this;
    }

    void tree::simulate(float timeStep)
    {
    /*
        Simulate gravity using timeStep
    */
        simulateCollisions();

        if(timeStep == 0.0f)
        {
            timeStep = 0.1f;
        }

        for(auto& changeApple : apples)
        {
            if(changeApple->type != appleType::staticApple)
            {
                changeApple->addForce(gravity * changeApple->mass);

                changeApple->position += (changeApple->force / changeApple->mass) * timeStep;

                changeApple->force = vector2f{0.0, 0.0};
            }
        }
    }

    void tree::simulateCollisions()
    {
    /*
        find collisions between apples and add them to collisions    
    */
        std::vector<foundCollision> collisions;

        for(auto& firstApple : apples)
        {
            firstApple->updateCollider();

            for(auto& secondApple : apples)
            {
                secondApple->updateCollider();

                if(firstApple == secondApple)
                    break;

                if(!(firstApple->appleCollider) || !(secondApple->appleCollider))
                    break;

                if(firstApple->appleCollider->collision(*(secondApple->appleCollider)))
                    collisions.push_back(foundCollision{firstApple, secondApple});
            }
        }

        handleCollisions(collisions);
    }

    void tree::handleCollisions(const std::vector<foundCollision>& collisions)
    {
    /*
        Handle collisions found by simulateCollisions
    */
        for(auto& collisionFound : collisions)
        {
            vector2f firstAppleForce = collisionFound.firstApple->force; 

            collisionFound.secondApple->addForce(firstAppleForce);
            collisionFound.firstApple->addForce(-firstAppleForce);

            std::cout << "Collision " << std::endl;
        }
    }

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

    inline bool collision(int center1X, int center1Y, double radius1, int center2X, int center2Y, double radius2)
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


    inline bool collision(int centerX, int centerY, double radius, int rectX, int rectY, int width, int height)
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
}
