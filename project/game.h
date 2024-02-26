
//game.h
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "level.h"
#include "objectPowerUp.h"
#include "rendererObject.h"
#include "objectAsset.h"
#include "objectBall.h"
#include <irrklang/irrKlang.h>
#include "rendererText.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "rendererEffect.h"
#include "objectCrtFrame.h"
#include "levelManager.h"
#include "shaderManager.h"
#include "collisionManager.h"


using namespace irrklang;
using namespace std;
using namespace glm;

class PowerUp;

const float             INITIAL_PLAYER_SPEED(500.0f);
const vec2              INITIAL_BALL_SPEED(100.0f, -350.0f);
const unsigned int      SCREEN_WIDTH = 1200;
const unsigned int      SCREEN_HEIGHT = 800;
const vec2              PLAYER_SIZE(SCREEN_WIDTH / 7.0f, SCREEN_HEIGHT / 30.0f);
const float             BALL_RADIUS = SCREEN_HEIGHT / 50.0f;
const float             BUTTON_WIDTH = SCREEN_WIDTH / 2.7f;
const float             BUTTON_HEIGHT = SCREEN_HEIGHT / 9.0f;

class Game
{
public:
    enum MenuItem {
        PLAY,
        MORE,
        EXIT
    };
    enum GameState {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN,
        GAME_PAUSE,
        GAME_MORE
    };

    GameState               State;
    bool                    Keys[1024];
    bool                    keysProcessed[1024];
    bool                    quitRequested;
    float                   playerSpeed;
    float                   confuseTime;
    //
    MenuItem                selectedItem;
    vector<Texture2D>       menuTexturesNormal;
    vector<Texture2D>       menuTexturesHover;
    vector<PowerUp>         PowerUps;
    //
    ObjectRenderer*         Renderer;
    TextRenderer*           Text;
    EffectRenderer*         Effects;
    Ball*                   ball;
    AssetObject*            pauseMenu;
    AssetObject*            moreMenu;
    CrtFrame*               crtFrame;
    AssetObject*            Player;
    LevelManager            levelManager;

    Game();
    ~Game();

    void SetupGame();
    void HandleInput(float deltaTime);
    void UpdateGameState(float deltaTime);
    void DrawFrame();

private:
    void        ResolveCollisions();
    void        ResetLevel();
};

#endif