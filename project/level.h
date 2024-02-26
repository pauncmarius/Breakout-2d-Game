//evelManager.h
// *info* -> evelManager.cpp

#ifndef LEVEL_H
#define LEVEL_H

#include "rendererObject.h"
#include "jsonParser.h" // Include the SimpleJSONParser header
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "objectAsset.h"

using namespace std;
using namespace glm;

class Level
{
public:
    vector<AssetObject> Blocks;

    Level() {}

    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    void Draw(ObjectRenderer& renderer);
    bool IsCompleted();

private:
    void init(const vector<vector<unsigned int>>& tileData, unsigned int levelWidth, unsigned int levelHeight);
    vec3 setBlockColor(unsigned int tileCode) const;
};

#endif
