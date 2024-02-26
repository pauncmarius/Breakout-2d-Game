
// objectCrtFrame.cpp
#include "objectCrtFrame.h"

CrtFrame::CrtFrame(vec2 pos, vec2 size, Texture2D sprite,
    float minAlpha, float maxAlpha, float changeInterval):
    Position(pos), Size(size), Sprite(sprite),
    alphaMin(minAlpha), alphaMax(maxAlpha),
    alphaChangeInterval(changeInterval), alphaChangeTimer(0.0f) {
    crtAlpha = minAlpha;
}

void CrtFrame::Update(float dt) {
    UpdateAlphaValue(dt);
}

void CrtFrame::Draw(ObjectRenderer& renderer) {
    if (this->Sprite.getTextureID()) {
        renderer.DrawObject(this->Sprite, this->Position, this->Size, vec3(1.0f), crtAlpha);
    }
}

void CrtFrame::UpdateAlphaValue(float dt) {
    alphaChangeTimer += dt;
    if (alphaChangeTimer >= alphaChangeInterval) {
        alphaChangeTimer = 0.0f;
        crtAlpha = alphaMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (alphaMax - alphaMin)));
    }
}
