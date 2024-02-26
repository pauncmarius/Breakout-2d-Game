
//game.cpp
#include "game.h"

ISoundEngine* SoundEngine = createIrrKlangDevice();

Game::Game() : State(GAME_MENU), Keys(), quitRequested(false), playerSpeed(500.0f), confuseTime(0.0f){}

Game::~Game(){
    delete Renderer;
    delete Player;
    delete ball;
    delete Text;
    delete pauseMenu;
    delete moreMenu;
    delete Effects;
    delete crtFrame;
    SoundEngine->drop();
}

void Game::SetupGame()
{
    // load shaders
    ShaderManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", "sprite");
    ShaderManager::LoadShader("shaders/effect_processing.vs", "shaders/effect_processing.frag", "effectProcessing");
    ShaderManager::LoadShader("shaders/text_2d.vs", "shaders/text_2d.frag", "text");
    // configure shaders
    Shader objectShader = ShaderManager::GetShader("sprite");
    Shader effectShader = ShaderManager::GetShader("effectProcessing");
    Renderer = new ObjectRenderer(objectShader, SCREEN_WIDTH, SCREEN_HEIGHT);
    Effects = new EffectRenderer(effectShader, SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f);
    // load textures
    TextureManager::LoadTexture("assets/textures/bglevel1done.png", "bglevel1");
    TextureManager::LoadTexture("assets/textures/bglevel2done.png", "bglevel2");
    TextureManager::LoadTexture("assets/textures/bglevel3done.png", "bglevel3");
    TextureManager::LoadTexture("assets/textures/bglevel1.png", "bglevel1Finished");
    TextureManager::LoadTexture("assets/textures/bglevel2.png", "bglevel2Finished");
    TextureManager::LoadTexture("assets/textures/bglevel3.png", "bglevel3Finished");
    TextureManager::LoadTexture("assets/textures/bgMenu.png", "backgroundMenu");
    TextureManager::LoadTexture("assets/textures/bgMore.png", "backgroundMore");
    TextureManager::LoadTexture("assets/textures/pause.png", "pause");
    TextureManager::LoadTexture("assets/textures/ball1.png", "ball1");
    TextureManager::LoadTexture("assets/textures/ball2.png", "ball2");
    TextureManager::LoadTexture("assets/textures/ball3.png", "ball3");
    TextureManager::LoadTexture("assets/textures/ball4.png", "ball4");
    TextureManager::LoadTexture("assets/textures/ball5.png", "ball5");
    TextureManager::LoadTexture("assets/textures/block.png", "block");
    TextureManager::LoadTexture("assets/textures/paddle.png", "paddle");
    TextureManager::LoadTexture("assets/textures/upgrades/speed+.png", "powerup_speed_increase");
    TextureManager::LoadTexture("assets/textures/upgrades/size+.png", "powerup_size_increase");
    TextureManager::LoadTexture("assets/textures/upgrades/speed-.png", "powerup_speed_decrease");
    TextureManager::LoadTexture("assets/textures/upgrades/size-.png", "powerup_size_decrease");
    TextureManager::LoadTexture("assets/textures/upgrades/ColorChange.png", "powerup_color_changing");
    TextureManager::LoadTexture("assets/textures/tv.png", "CRTframe");
    //load Ball's images
    vector<Texture2D> ballAnimationFrames;
    ballAnimationFrames.push_back(TextureManager::GetTexture("ball1"));
    ballAnimationFrames.push_back(TextureManager::GetTexture("ball2"));
    ballAnimationFrames.push_back(TextureManager::GetTexture("ball3"));
    ballAnimationFrames.push_back(TextureManager::GetTexture("ball4"));
    ballAnimationFrames.push_back(TextureManager::GetTexture("ball5"));
    // load levels
    levelManager.LoadLevel("assets/levels/level1.json", SCREEN_WIDTH, SCREEN_HEIGHT / 2);
    levelManager.LoadLevel("assets/levels/level2.json", SCREEN_WIDTH, SCREEN_HEIGHT / 2);
    levelManager.LoadLevel("assets/levels/level3.json", SCREEN_WIDTH, SCREEN_HEIGHT / 2);
    // configure game objects
    vec2 playerPos = vec2(SCREEN_WIDTH / 2.0f - PLAYER_SIZE.x / 2.0f, SCREEN_HEIGHT - PLAYER_SIZE.y);
    Player = new AssetObject(playerPos, PLAYER_SIZE, TextureManager::GetTexture("paddle"), vec3(1.0f));

    vec2 ballPos = playerPos + vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS*2);
    ball = new Ball(ballPos, BALL_RADIUS, INITIAL_BALL_SPEED, ballAnimationFrames, 3.0f);

    Texture2D crtTexture = TextureManager::GetTexture("CRTframe");
    crtFrame = new CrtFrame(vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT), crtTexture, 0.4f, 0.6f, 0.1f);

    pauseMenu = new AssetObject(vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT), TextureManager::GetTexture("pause"));
    moreMenu = new AssetObject(vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT), TextureManager::GetTexture("backgroundMore"));
    //music
    SoundEngine->play2D("assets/audio/music.mp3", true);
    SoundEngine->setSoundVolume(0.2f);
    //text
    Shader textShader = ShaderManager::GetShader("text");
    Text = new TextRenderer(textShader, SCREEN_WIDTH, SCREEN_HEIGHT);
    Text->Load("assets/font/PublicPixel.ttf", 24);
    // Load menu textures
    menuTexturesNormal.push_back(TextureManager::LoadTexture("assets/textures/buttons/play.png", "play_normal"));
    menuTexturesHover.push_back(TextureManager::LoadTexture("assets/textures/buttons/play2.png", "play_hover"));
    menuTexturesNormal.push_back(TextureManager::LoadTexture("assets/textures/buttons/more.png", "more_normal"));
    menuTexturesHover.push_back(TextureManager::LoadTexture("assets/textures/buttons/more2.png", "more_hover"));
    menuTexturesNormal.push_back(TextureManager::LoadTexture("assets/textures/buttons/quit.png", "quit_normal"));
    menuTexturesHover.push_back(TextureManager::LoadTexture("assets/textures/buttons/quit2.png", "quit_hover"));
}

void Game::UpdateGameState(float deltaTime)
{
    if (this->State == GAME_ACTIVE) {
        ball->Move(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);
        ball->UpdateAnimation(deltaTime);
        //
        for (PowerUp& powerUp : this->PowerUps) {
            powerUp.Update(deltaTime, *this);
        }
    }
    //  crt effect
    crtFrame->Update(deltaTime);
    // check for collisions
    this->ResolveCollisions();
    // check loss condition
    if (ball->Position.y >= SCREEN_HEIGHT)
    {
        SoundEngine->play2D("assets/audio/fail.mp3", false);
        this->ResetLevel();
    }
    // check win cond
    if (this->State == GAME_ACTIVE && levelManager.IsCurrentLevelCompleted())
    {
            SoundEngine->play2D("assets/audio/completed.mp3", false);
            this->State = GAME_WIN;
            SoundEngine->setSoundVolume(0.3f);
    }
    //  music for pause state
    if (this->State == GAME_PAUSE) {
        SoundEngine->setSoundVolume(0.2f);
    }
    //  confuse effect opt
    if (confuseTime > 0.0f)
    {
        confuseTime -= deltaTime;
        if (confuseTime <= 0.0f)
            Effects->Confuse = false;
    }
}

void Game::HandleInput(float deltaTime)
{
    // game menu
    if (this->State == GAME_MENU) {
        // Cycle up through menu items
        if (this->Keys[GLFW_KEY_UP] && !this->keysProcessed[GLFW_KEY_UP]) {
            selectedItem = static_cast<MenuItem>((selectedItem - 1 + 3) % 3);
            this->keysProcessed[GLFW_KEY_UP] = true;
        }
        // Cycle down through menu items
        if (this->Keys[GLFW_KEY_DOWN] && !this->keysProcessed[GLFW_KEY_DOWN]) {
            selectedItem = static_cast<MenuItem>((selectedItem + 1) % 3);
            this->keysProcessed[GLFW_KEY_DOWN] = true;
        }
        //
        if (this->Keys[GLFW_KEY_ENTER] && !this->keysProcessed[GLFW_KEY_ENTER]) {
            // Execute the action of the selected item
            switch (selectedItem) {
            case PLAY:
                this->State = GAME_ACTIVE;
                SoundEngine->setSoundVolume(0.6f);
                break;
            case MORE:
                this->State = GAME_MORE;
                break;
            case EXIT:
                this->quitRequested = true; // Set the flag to indicate a quit request
                break;
            }
            this->keysProcessed[GLFW_KEY_ENTER] = true;
        }
    }
    // game active
    if (this->State == GAME_ACTIVE)
    {
        float speed = playerSpeed * deltaTime;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
            {
                Player->Position.x -= speed;
                if (ball->Stopped)
                    ball->Position.x -= speed;
            }
        }
        //
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= SCREEN_WIDTH - Player->Size.x)
            {
                Player->Position.x += speed;
                if (ball->Stopped)
                    ball->Position.x += speed;
            }
        }
        //
        if (this->Keys[GLFW_KEY_SPACE])
            ball->Stopped = false;
        //
        if (this->Keys[GLFW_KEY_ESCAPE] && !this->keysProcessed[GLFW_KEY_ESCAPE]) {
            this->State = GAME_PAUSE;
            SoundEngine->setSoundVolume(0.2f);
            this->keysProcessed[GLFW_KEY_ESCAPE] = true;
        }
    }
    // game pause
    else if (this->State == GAME_PAUSE) {
        if (this->Keys[GLFW_KEY_ESCAPE] && !this->keysProcessed[GLFW_KEY_ESCAPE]) {
            this->State = GAME_ACTIVE;
            this->keysProcessed[GLFW_KEY_ESCAPE] = true;
        }
        //
        if (this->Keys[GLFW_KEY_M] && !this->keysProcessed[GLFW_KEY_M])
        {
            this->State = GAME_MENU;
            this->keysProcessed[GLFW_KEY_M] = true;
        }
    }
    // game win
    if (this->State == GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_RIGHT_SHIFT] && !this->keysProcessed[GLFW_KEY_RIGHT_SHIFT]) {
            if (this->levelManager.GetLevelCount() - 1 > this->levelManager.GetCurrentLevel()) {
                this->State = GAME_ACTIVE;
                this->levelManager.GoToNextLevel();
                this->ResetLevel();
            }

            else {
                this->State = GAME_MENU;
                SoundEngine->play2D("assets/audio/completed.mp3", false);
                SoundEngine->setSoundVolume(0.3f);
            }
            this->keysProcessed[GLFW_KEY_RIGHT_SHIFT] = true;
        } else if (this->Keys[GLFW_KEY_ENTER] && !this->keysProcessed[GLFW_KEY_ENTER]) {
            if (this->levelManager.GetCurrentLevel() == 0) {
                TextureManager::SaveImageToDownloads("assets/textures/bglevel1.png", "SavedGameImage1.png");
            }
            else if(this->levelManager.GetCurrentLevel() == 1){
                TextureManager::SaveImageToDownloads("assets/textures/bglevel2.png", "SavedGameImage2.png");
            }
            else if(this->levelManager.GetCurrentLevel() == 2){
                TextureManager::SaveImageToDownloads("assets/textures/bglevel3.png", "SavedGameImage3.png");
            }
            //
            if (this->levelManager.GetCurrentLevel() < this->levelManager.GetLevelCount() - 1)
            {
                this->State = GAME_ACTIVE;
                this->levelManager.GoToNextLevel();
                this->ResetLevel();
            } 
            else {
                this->State = GAME_MENU;
                SoundEngine->play2D("assets/audio/completed.mp3", false);
                SoundEngine->setSoundVolume(0.3f);
            }
            this->keysProcessed[GLFW_KEY_ENTER] = true;
        }
    }
    //game more
    if (this->State == GAME_MORE) {
        if (this->Keys[GLFW_KEY_ESCAPE] && !this->keysProcessed[GLFW_KEY_ESCAPE]) {
            this->State = GAME_MENU;
            this->keysProcessed[GLFW_KEY_ESCAPE] = true;
        }
    }
}

void Game::DrawFrame()
{
    // game active
    if (this->State == GAME_ACTIVE || this->State == GAME_PAUSE)
    {
       Effects->BeginRender();
            // draw background
            Texture2D myTexture;
            if (levelManager.GetCurrentLevel() == 0) {
                Texture2D myTexture = TextureManager::GetTexture("bglevel1");
                Renderer->DrawObject(myTexture, vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            }
            else if (levelManager.GetCurrentLevel() == 1) {
                Texture2D myTexture = TextureManager::GetTexture("bglevel2");
                Renderer->DrawObject(myTexture, vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            }
            else if (levelManager.GetCurrentLevel() == 2) {
                Texture2D myTexture = TextureManager::GetTexture("bglevel3");
                Renderer->DrawObject(myTexture, vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            }
            levelManager.DrawCurrentLevel(*Renderer);
            // draw player
            Renderer->DrawObject(Player->Sprite, Player->Position, Player->Size, Player->Color, 1.0f);
            // draw PowerUps
            for (PowerUp& powerUp : this->PowerUps)
                if (!powerUp.Destroyed)
                    powerUp.Draw(*Renderer);
            // draw ball
            ball->DrawAnimation(*Renderer);
            //render text
            string levelText = "Level: " + to_string(this->levelManager.GetCurrentLevel() + 1);
            float textWidth = Text->CalculateTextWidth(levelText, 1.0f);
            Text->RenderText(levelText, SCREEN_WIDTH / 2 - textWidth / 2.0f, 30.0f, 1.0f);
            //render crt
            crtFrame->Draw(*Renderer);
        Effects->EndRender();
        Effects->Render(glfwGetTime());

    }
    // game menu
    if (this->State == GAME_MENU) {
        // draw background
        Texture2D myTexture2 = TextureManager::GetTexture("backgroundMenu");
        Renderer->DrawObject(myTexture2, vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        // Render each menu item
        for (int i = 0; i < 3; i++) {
            Texture2D texture = (selectedItem == i) ? menuTexturesHover[i] : menuTexturesNormal[i];
            Renderer->DrawObject(texture, vec2((SCREEN_WIDTH/2) - BUTTON_WIDTH/2, 70.0f + i * 100.0f), vec2(BUTTON_WIDTH, BUTTON_HEIGHT));
        }
        crtFrame->Draw(*Renderer);
    }
    //game win
    if (this->State == GAME_WIN)
    {
        Texture2D winTexture;
        string message = "Press R Shift to Continue without downloading the image!";
        float textWidth = Text->CalculateTextWidth(message, 0.5f);
        string message2 = "Press Enter to Download and continue the game!";
        float textWidth2 = Text->CalculateTextWidth(message2, 0.5f);
        if (this->levelManager.GetCurrentLevel() == 0) {
            Texture2D winTexture = TextureManager::GetTexture("bglevel1Finished");
            Renderer->DrawObject(winTexture, vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            Text->RenderText(message, SCREEN_WIDTH/2 - textWidth/ 2.0f, 25.0f, 0.5f, vec3(0.0f));
            Text->RenderText(message2, SCREEN_WIDTH / 2 - textWidth2 / 2.0f, 50.0f, 0.5f, vec3(0.0f));

        }
        else if (this->levelManager.GetCurrentLevel() == 1) {
            Texture2D winTexture = TextureManager::GetTexture("bglevel2Finished");
            Renderer->DrawObject(winTexture, vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            Text->RenderText(message, SCREEN_WIDTH / 2 - textWidth / 2.0f, 15.0f, 0.5f, vec3(0.0f));
            Text->RenderText(message2, SCREEN_WIDTH / 2 - textWidth2 / 2.0f, 40.0f, 0.5f, vec3(0.0f));

        }
        else if (this->levelManager.GetCurrentLevel() == 2) {
            Texture2D winTexture = TextureManager::GetTexture("bglevel3Finished");
            Renderer->DrawObject(winTexture, vec2(0.0f, 0.0f), vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            Text->RenderText(message, SCREEN_WIDTH / 2 - textWidth / 2.0f, 15.0f, 0.5f, vec3(0.0f));
            Text->RenderText(message2, SCREEN_WIDTH / 2 - textWidth2 / 2.0f, 40.0f, 0.5f, vec3(0.0f));
        }
        crtFrame->Draw(*Renderer);

    }
    //game pause
    if (this->State == GAME_PAUSE) {
        // Assuming you have a function to render images with alpha
        Renderer->DrawObject(pauseMenu->Sprite, pauseMenu->Position, pauseMenu->Size, pauseMenu->Color, 0.5f);

        // Render the pause text
        string message3 = "Game Paused";
        float textWidth3 = Text->CalculateTextWidth(message3, 1.5f);
        string message4 = "Press M to return to MENU";
        float textWidth4 = Text->CalculateTextWidth(message4, 1.3f);
        Text->RenderText(message3, (SCREEN_WIDTH / 2) - textWidth3/2, SCREEN_HEIGHT / 2, 1.5f);
        Text->RenderText(message4, (SCREEN_WIDTH / 2) - textWidth4/2, SCREEN_HEIGHT / 2 + 80, 1.3f);
    }
    //game more
    if (this->State == GAME_MORE) {
        Renderer->DrawObject(moreMenu->Sprite, moreMenu->Position, moreMenu->Size, moreMenu->Color, 1.0f);
        crtFrame->Draw(*Renderer);

    }
}

void Game::ResetLevel()
{
    //reset player
    Player->Size = PLAYER_SIZE;
    Player->Position = vec2(SCREEN_WIDTH / 2.0f - PLAYER_SIZE.x / 2.0f, SCREEN_HEIGHT - PLAYER_SIZE.y);
    ball->Reset(Player->Position + vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS*2), INITIAL_BALL_SPEED);
    this->playerSpeed = INITIAL_PLAYER_SPEED;
    Effects->Confuse = false;
    Player->Color = vec3(1.0f);

    levelManager.ResetCurrentLevel();
    for (PowerUp& powerUp : this->PowerUps) {
        powerUp.Activated = false;
        powerUp.Destroyed = true;
    }
}

void Game::ResolveCollisions() {
    // Iterate over each brick in the current level
    Level& currentLevel = this->levelManager.GetCurrentLevelCons();

    for (AssetObject& box : currentLevel.Blocks) {
        if (!box.Destroyed) {

            CollisionManager::Collision collision = CollisionManager::CheckBallAABBCollision(*ball, box);

            if (collision.hasCollided) { // Collision detected
                // Play sound effect for collision
                SoundEngine->play2D("assets/audio/impact.mp3", false);
                // Handle non-solid bricks and power-ups
                if (!box.IsSolid) {
                    box.Destroyed = true;
                    PowerUp::SpawnPowerUps(box, this->PowerUps);
                }

                CollisionManager::resolveDirectionCollision(*ball, box, collision);
            }
        }

        // Check for collision with the player paddle
        if (!ball->Stopped) {
            CollisionManager::Collision result = CollisionManager::CheckBallAABBCollision(*ball, *Player);
            if (result.hasCollided) {
                CollisionManager::adjustBallAfterPlayerCollision(*ball, *Player);
                // Play sound effect for paddle collision
                SoundEngine->play2D("assets/audio/impact.mp3", false);
            }
        }

        // Iterate over each power-up
        for (PowerUp& powerUp : this->PowerUps) {
            if (!powerUp.Destroyed) {
                if (powerUp.Position.y >= SCREEN_HEIGHT) {
                    powerUp.Destroyed = true;
                }
                else if (CollisionManager::CheckCollisionPowerUp(*Player, powerUp)) {
                    // Activate power-up and play sound
                    powerUp.Destroyed = true;
                    powerUp.Activated = true;
                    powerUp.Activate(*this);
                    SoundEngine->play2D("assets/audio/powerup.mp3", false);
                }
            }
        }
    }
}
