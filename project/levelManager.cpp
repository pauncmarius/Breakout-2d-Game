
//levelManager.cpp
#include "levelManager.h"

LevelManager::LevelManager() : currentLevelIndex(0) {};

void LevelManager::LoadLevel(const char* levelFile, unsigned int width, unsigned int height){
    this->Height = height;
    this->Width = width;
    Level level;
    level.Load(levelFile, width, height);
    levels.push_back(level);
};
void LevelManager::ResetCurrentLevel() {
    if (currentLevelIndex < levels.size()) {
        if (currentLevelIndex == 0)
            this->levels[0].Load("assets/levels/level1.json", this->Width, this->Height);
        if (currentLevelIndex == 1)
            this->levels[1].Load("assets/levels/level2.json", this->Width, this->Height);
        if (currentLevelIndex == 1)
            this->levels[2].Load("assets/levels/level3.json", this->Width, this->Height);
    }
};
bool LevelManager::IsCurrentLevelCompleted() {
    return currentLevelIndex < levels.size() && levels[currentLevelIndex].IsCompleted();
};
void LevelManager::DrawCurrentLevel(ObjectRenderer& renderer) {
    if (currentLevelIndex < levels.size()) {
        levels[currentLevelIndex].Draw(renderer);
    }
};

unsigned int LevelManager::GetCurrentLevel() {
    return currentLevelIndex;
};

size_t LevelManager::GetLevelCount() const {
    return levels.size();
};

void LevelManager::GoToNextLevel() {
    if (currentLevelIndex < levels.size() - 1) {
        currentLevelIndex++;
    }
};

Level& LevelManager::GetCurrentLevelCons() {
    return levels[currentLevelIndex];
};