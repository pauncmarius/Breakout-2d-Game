//objectPowerUp.h
// *info* -> objectPowerUp.cpp

#ifndef objectPowerUp_H
#define objectPowerUp_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "rendererObject.h"
#include "texture.h"
#include "game.h"
#include <algorithm>
#include "objectAsset.h"
#include "textureManager.h"

using namespace glm;
using namespace std;

class Game;

class PowerUp{
public:
    string Type;
    vec2 Position, Speed, Size;
    Texture2D Sprite;
    bool Destroyed, Activated;
    float Duration;
    vec3 Color;

    PowerUp(string type, vec3 color, float duration, vec2 position, Texture2D texture);

    void Activate(Game& game);
    static void SpawnPowerUps(AssetObject& block, vector<PowerUp>& powerUps);
    static bool IsOtherPowerUpActive(const vector<PowerUp>& powerUps, const string& type);
    void Update(float dt, Game& game);
    void Draw(ObjectRenderer& renderer);
};
#endif