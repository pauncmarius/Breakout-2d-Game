// objectBall.cpp
#include "objectBall.h"
#include "collisionManager.h"

Ball::Ball() : Position(0.0f), Size(1.0f), Speed(0.0f), Radius(1.0f), Stopped(true), AnimationSpeed(0.0f), CurrentFrame(0.0f) {
}

Ball::Ball(vec2 pos, float radius, vec2 speed, vector<Texture2D> spriteFrames, float animationSpeed)
    : Position(pos), Radius(radius), Speed(speed), AnimationFrames(spriteFrames), AnimationSpeed(animationSpeed), CurrentFrame(0.0f) {
    Size = vec2(radius * 2, radius * 2);
    Stopped = true;
}

vec2 Ball::Move(float dt, unsigned int window_width, unsigned int window_height) {
    if (!this->Stopped) {
        // Update the ball's position based on its speed and the elapsed time (dt)
        this->Position += this->Speed * dt;

        // Handle collisions using CollisionManager and adjust position and speed accordingly
        bitset<16> collisionResult = CollisionManager::CheckBallWindowCollision(*this, window_width, window_height);

        if (collisionResult[Left] || collisionResult[Right]) {
            Speed.x = -Speed.x;
            // Adjust position to prevent sticking to the wall
            if (collisionResult[Left]) Position.x = 0.0f;
            if (collisionResult[Right]) Position.x = window_width - Size.x;
        }

        // Reverse speed on collision with the top or bottom walls
        if (collisionResult[Top]) {
            Speed.y = -Speed.y;
            // Adjust position to prevent sticking to the wall
            if (collisionResult[Top]) Position.y = 0.0f;
        }

        return this->Position;
    }
}


void Ball::Reset(vec2 position, vec2 speed) {
    Position = position;
    Speed = speed;
    Stopped = true;
}

void Ball::UpdateAnimation(float dt) {
    if (!AnimationFrames.empty()) {
        CurrentFrame += AnimationSpeed * dt;
        if (CurrentFrame >= AnimationFrames.size()) CurrentFrame = 0.0f;
    }
}

void Ball::DrawAnimation(ObjectRenderer& renderer) {
    if (!AnimationFrames.empty()) {
        Texture2D frame = AnimationFrames[static_cast<unsigned int>(CurrentFrame)];
        renderer.DrawObject(frame, Position, Size, vec3(1.0f));
    }
}
