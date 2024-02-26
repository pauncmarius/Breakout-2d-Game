
// Level.cpp
#include "level.h"
#include "textureManager.h"
#include <iostream>

void Level::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {
    this->Blocks.clear();

    SimplifiedLevelParser parser;
    
    vector<vector<unsigned int>> tileData;

    try {
        tileData = parser.parseLevelData(file);
    }
    catch (const exception& e) {
        cerr << "Failed to parse level data: " << e.what() << endl;
        return;
    }

    if (!tileData.empty()) {
        this->init(tileData, levelWidth, levelHeight);
    }
}


void Level::Draw(ObjectRenderer& renderer) {
    for (AssetObject& tile : this->Blocks) {
        if (!tile.Destroyed)
            renderer.DrawObject(tile.Sprite, tile.Position, tile.Size, tile.Color, 1.0f);
    }
}

bool Level::IsCompleted() {
    for (AssetObject& tile : this->Blocks) {
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    }
    return true;
}

void Level::init(const vector<vector<unsigned int>>& tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int rows = tileData.size()-1;
    unsigned int cols = (rows > 0) ? tileData[0].size() : 0;

    float unitWidth = levelWidth / static_cast<float>(cols);
    float unitHeight = levelHeight / static_cast<float>(rows);

    for (unsigned int y = 0; y < rows; ++y)
    {
        for (unsigned int x = 0; x < cols; ++x)
        {
            if (tileData[y][x] == 0) continue; // Skip empty tiles

            vec2 pos(unitWidth * x, unitHeight * y);
            vec2 size(unitWidth, unitHeight);
            vec3 color = this->setBlockColor(tileData[y][x]);

            AssetObject obj(pos, size, TextureManager::GetTexture("block"), color);
            obj.IsSolid = (tileData[y][x] == 1);

            this->Blocks.push_back(obj);
        }
    }
}

vec3 Level::setBlockColor(unsigned int tileCode) const
{
    switch (tileCode)
    {
    case 1: return vec3(0.3f); // Solid block color
    case 2: return vec3(0.2f, 0.6f, 1.0f);
    case 3: return vec3(0.0f, 0.7f, 0.0f);
    case 4: return vec3(0.8f, 0.8f, 0.4f);
    case 5: return vec3(1.0f, 0.5f, 0.0f);
    default: return vec3(1.0f); // Default color for non-solid blocks
    }
}
