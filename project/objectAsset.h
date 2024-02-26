
//objectAsset.h
#ifndef ASSETOBJECT_H
#define ASSETOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "rendererObject.h"

using namespace std;
using namespace glm;

class AssetObject {
public:
    vec2 Position, Size;
    vec3 Color;
    bool IsSolid, Destroyed;
    Texture2D Sprite;

    AssetObject(vec2 pos, vec2 size, Texture2D sprite, vec3 color = vec3(1.0f))
        : 
        Position(pos), 
        Size(size), 
        Sprite(sprite), 
        Color(color), 
        IsSolid(false), 
        Destroyed(false) {}
};

#endif