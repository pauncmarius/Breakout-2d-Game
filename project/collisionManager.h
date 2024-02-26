#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "objectBall.h"
#include "objectAsset.h"
#include "objectPowerUp.h"
#include <tuple>
#include <glm/glm.hpp>

class PowerUp;

class CollisionManager {
public:
    enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

    struct Collision {
        bool hasCollided;
        Direction directionOfCollision;
        glm::vec2 pointOfImpact;
    };

    static Collision CheckBallAABBCollision(Ball& ball, AssetObject& object);
    static bool CheckCollisionPowerUp(AssetObject& one, PowerUp& two);
    static bitset<16> CheckBallWindowCollision(Ball& ball, unsigned int window_width, unsigned int window_height);

    static void resolveDirectionCollision(Ball& ball, AssetObject& box, const CollisionManager::Collision& collision);
    static void adjustBallAfterPlayerCollision(Ball& ball, AssetObject& player);



private:
    CollisionManager() {} // Private constructor to prevent instantiation

    static Direction getDirection(glm::vec2 target);
};

#endif // COLLISION_MANAGER_H
