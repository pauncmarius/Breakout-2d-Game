
//collisionManager.cpp
#include "collisionManager.h"
#include <cmath>

// Determine the direction of a vector
CollisionManager::Direction CollisionManager::getDirection(glm::vec2 target) {
    static const glm::vec2 UP_DIR(0.0f, 1.0f);
    static const glm::vec2 RIGHT_DIR(1.0f, 0.0f);
    static const glm::vec2 DOWN_DIR(0.0f, -1.0f);
    static const glm::vec2 LEFT_DIR(-1.0f, 0.0f);

    target = glm::normalize(target); // Normalize the target vector for angle comparison

    float angleUp = glm::degrees(glm::acos(glm::dot(UP_DIR, target)));
    float angleRight = glm::degrees(glm::acos(glm::dot(RIGHT_DIR, target)));
    float angleDown = glm::degrees(glm::acos(glm::dot(DOWN_DIR, target)));
    float angleLeft = glm::degrees(glm::acos(glm::dot(LEFT_DIR, target)));

    float minAngle = glm::min(angleUp, angleRight);
    minAngle = glm::min(minAngle, angleDown);
    minAngle = glm::min(minAngle, angleLeft);

    if (minAngle == angleUp) {
        return UP;
    }
    else if (minAngle == angleRight) {
        return RIGHT;
    }
    else if (minAngle == angleDown) {
        return DOWN;
    }
    else { // minAngle == angleLeft
        return LEFT;
    }
}

CollisionManager::Collision CollisionManager::CheckBallAABBCollision(Ball& ball, AssetObject& object) {
    // Finding the circle's center point
    glm::vec2 circleCenter = ball.Position + glm::vec2(ball.Radius, ball.Radius);
    // Calculating the rectangle's center and half-size
    glm::vec2 rectHalfSize = object.Size * 0.5f;
    glm::vec2 rectCenter = object.Position + rectHalfSize;
    // Determining the offset between the circle's center and the rectangle's center
    glm::vec2 centerOffset = circleCenter - rectCenter;
    // Clamping the offset within the rectangle's bounds
    glm::vec2 clampedOffset = glm::clamp(centerOffset, -rectHalfSize, rectHalfSize);
    // The nearest point on the rectangle to the circle's center
    glm::vec2 nearestPoint = rectCenter + clampedOffset;
    // Vector from the nearest point to the circle's center
    glm::vec2 impactVector = nearestPoint - circleCenter;
    // Checking for collision
    bool collision = glm::length(impactVector) < ball.Radius;
    // If collision occurs, use getDirection to determine the direction
    CollisionManager::Direction collisionDirection = CollisionManager::Direction::NONE;

    if (collision) {
        collisionDirection = CollisionManager::getDirection(impactVector);
    }

    return CollisionManager::Collision{ collision, collisionDirection, impactVector };
}

// Check if a power-up has collided with the player paddle
bool CollisionManager::CheckCollisionPowerUp(AssetObject& one, PowerUp& two) {
    // Get centers of each object
    glm::vec2 centerOne = one.Position + one.Size * 0.5f;
    glm::vec2 centerTwo = two.Position + two.Size * 0.5f;
    // Compute the difference vector between centers
    glm::vec2 difference = centerOne - centerTwo;
    // Calculate the extent (half the size) of each object along each axis
    glm::vec2 extentOne = one.Size * 0.5f;
    glm::vec2 extentTwo = two.Size * 0.5f;
    // Calculate overlap on each axis
    glm::vec2 overlap = glm::vec2(abs(difference.x), abs(difference.y)) - (extentOne + extentTwo);
    // Check for overlap on each axis
    return overlap.x < 0.0f && overlap.y < 0.0f;
}

bitset<16> CollisionManager::CheckBallWindowCollision(Ball& ball, unsigned int window_width, unsigned int window_height) {
    bitset<16> collisionResult;
    if (ball.Position.x <= 0.0f) collisionResult.set(Ball::Left);
    else if (ball.Position.x + ball.Size.x >= window_width) collisionResult.set(Ball::Right);
    else if (ball.Position.y <= 0.0f) collisionResult.set(Ball::Top);
    else if (ball.Position.y + ball.Size.y >= window_height) collisionResult.set(Ball::Bottom);
    else collisionResult.set(Ball::None);
    return collisionResult;
}

void CollisionManager::resolveDirectionCollision(Ball& ball, AssetObject& box, const CollisionManager::Collision& collision) {
    CollisionManager::Direction dir = collision.directionOfCollision;
    vec2 diff_vector = collision.pointOfImpact;

    if (dir == CollisionManager::LEFT || dir == CollisionManager::RIGHT) {
        ball.Speed.x = -ball.Speed.x; // Reverse horizontal speed
        float difference = ball.Radius - std::abs(diff_vector.x);
        ball.Position.x += (dir == CollisionManager::LEFT) ? difference : -difference;
    }
    else { // Vertical collision (UP or DOWN)
        ball.Speed.y = -ball.Speed.y; // Reverse vertical speed
        float difference = ball.Radius - std::abs(diff_vector.y);
        ball.Position.y += (dir == CollisionManager::UP) ? -difference : difference;
    }
}

void CollisionManager::adjustBallAfterPlayerCollision(Ball& ball, AssetObject& player) {
    float playerCenter = player.Position.x + player.Size.x / 2.0f;
    float hitPoint = (ball.Position.x + ball.Radius) - playerCenter;
    float impactFactor = hitPoint / (player.Size.x / 2.0f);
    vec2 previousSpeed = ball.Speed;
    ball.Speed.x = INITIAL_BALL_SPEED.x * impactFactor * 2.0f;
    ball.Speed.y = -std::abs(ball.Speed.y);
    ball.Speed = glm::normalize(ball.Speed) * glm::length(previousSpeed);
}