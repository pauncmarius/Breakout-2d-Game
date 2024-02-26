// objectCrtFrame.h
#ifndef objectCrtFrame_H
#define objectCrtFrame_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "rendererObject.h"
#include <stdlib.h> // Pentru rand()

using namespace std;
using namespace glm;

class CrtFrame{
public:
    vec2 Position, Size;
    float Alpha;
    Texture2D Sprite;

    float crtAlpha;
    float alphaMin;
    float alphaMax;
    float alphaChangeTimer;
    float alphaChangeInterval;

    CrtFrame();
    CrtFrame(vec2 pos, vec2 size, Texture2D sprite,
        float minAlpha = 0.4f, float maxAlpha = 0.6f, float changeInterval = 0.1f);

    void Update(float dt);
    void Draw(ObjectRenderer& renderer);

private:
    void UpdateAlphaValue(float dt);
};

#endif
