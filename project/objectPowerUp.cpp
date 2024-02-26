
//objectPowerUp.cpp
#include "objectPowerUp.h"

bool ShouldSpawn(unsigned int chance);

PowerUp::PowerUp(string type, vec3 color, float duration, vec2 position, Texture2D texture) :
    Position(position),
    Size(vec2(40.0f, 40.0f)),
    Sprite(texture),
    Color(color),
    Speed(vec2(0.0f, 150.0f)),
    Type(type),
    Duration(duration), 
    Activated(false),
    Destroyed(false){}


void PowerUp::Activate(Game& game) {
    if (Type == "pad-speed-increase" && game.playerSpeed < SCREEN_WIDTH / 1.5f) {
        game.playerSpeed += 50.0f;
    }
    else if (Type == "pad-size-increase") {
        game.Player->Size.x += 20;
    }
    else if (Type == "pad-size-decrease") {
        game.Player->Size.x -= 20;
    }
    else if (Type == "pad-speed-decrease") {
        game.playerSpeed -= 50.0f;
    }
    else if (Type == "confuse") {
        game.confuseTime = 5.0f;
        game.Effects->Confuse = true;
    }
}

void PowerUp::SpawnPowerUps(AssetObject& block, vector<PowerUp>& powerUps) {
    if (ShouldSpawn(50)) {
        string type;
        float duration = 0.0f;
        Texture2D texture;

        int powerUpType = rand() % 5;

        switch (powerUpType) {
        case 0:
            type = "pad-speed-increase";
            texture = TextureManager::GetTexture("powerup_speed_increase");
            break;
        case 1:
            type = "pad-size-increase";
            texture = TextureManager::GetTexture("powerup_size_increase");
            break;
        case 2:
            type = "pad-size-decrease";
            texture = TextureManager::GetTexture("powerup_size_decrease");
            break;
        case 3:
            type = "pad-speed-decrease";
            texture = TextureManager::GetTexture("powerup_speed_decrease");
            break;
        case 4:
            type = "confuse";
            texture = TextureManager::GetTexture("powerup_color_changing");
            duration = 1.0f;
            break;
        }
        powerUps.push_back(PowerUp(type, vec3(1.0f), duration, block.Position, texture));
    }
}

bool PowerUp::IsOtherPowerUpActive(const vector<PowerUp>& powerUps, const string& type) {
    return any_of(powerUps.begin(), powerUps.end(), [&](const PowerUp& powerUp) {
        return powerUp.Activated && powerUp.Type == type;
    });
}

void PowerUp::Update(float dt, Game& game) {
    if (!this->Destroyed) {
        this->Position += this->Speed * dt;
        if (this->Position.y >= SCREEN_HEIGHT) {
            this->Destroyed = true;
        }
    }

    if (this->Activated) {
        this->Duration -= dt;
        if (this->Duration <= 0.0f) {
            this->Activated = false;

            if (this->Type == "confuse" && !IsOtherPowerUpActive(game.PowerUps, "confuse")) {
                game.Effects->Confuse = false;
            }
        }
    }
}

bool ShouldSpawn(unsigned int chance) {
    unsigned int random = rand() % 100;
    return random < chance;
}


void PowerUp::Draw(ObjectRenderer& renderer) {
    renderer.DrawObject(this->Sprite, this->Position, this->Size);
};
