// objectBall.h
#ifndef OBJECTBALL_H
#define OBJECTBALL_H

#include <glm/glm.hpp>
#include "rendererObject.h"
#include "texture.h"
#include <vector>
#include <bitset>

using namespace glm;
using namespace std;

class Ball {
public:
    vec2 Position, Size, Speed;
    float Radius;
    bool Stopped;
    Texture2D Sprite;
    vector<Texture2D> AnimationFrames;
    float AnimationSpeed, CurrentFrame;

    enum CollisionType {
        None = 0,
        Left = 1 << 0,
        Right = 1 << 1,
        Top = 1 << 2,
        Bottom = 1 << 3
    };

    Ball();
    Ball(vec2 pos, float radius, vec2 speed, vector<Texture2D> spriteFrames, float animationSpeed);

    vec2 Move(float dt, unsigned int window_width, unsigned int window_height);
    void Reset(vec2 position, vec2 speed);
    void UpdateAnimation(float dt);
    void DrawAnimation(ObjectRenderer& renderer);
};

#endif
