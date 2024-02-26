#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "level.h"
#include <vector>
#include <string>

class LevelManager {
public:
    LevelManager();
    void LoadLevel(const char* levelFile, unsigned int width, unsigned int height);
    void ResetCurrentLevel();
    bool IsCurrentLevelCompleted();
    void DrawCurrentLevel(ObjectRenderer& renderer);
    unsigned int GetCurrentLevel();
    size_t GetLevelCount() const;
    void GoToNextLevel();
    Level& GetCurrentLevelCons();

private:
    std::vector<Level> levels;
    unsigned int Width, Height;
    unsigned int currentLevelIndex;
};

#endif 
