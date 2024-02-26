
//game_main.cpp
#include "game.h"
#include <iostream>
#include "textureManager.h"
#include "shaderManager.h"
#include <stb_image/stb_image.h>
#include "windowManager.h"

// Forward declarations for GLFW callback functions
void setWindowIcon(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Global game instance
Game AnimalPlanet;

int main(int argc, char* argv[])
{
    WindowManager windowManager(SCREEN_WIDTH, SCREEN_HEIGHT, "AnimalPlanet");
    if (!windowManager.initialize()) {
        std::cerr << "Failed to initialize graphics." << std::endl;
        return -1;
    }

    GLFWwindow* window = windowManager.getWindow();
    setWindowIcon(window);

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // OpenGL configuration
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize game
    AnimalPlanet.SetupGame();

    // Game loop
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();

        // Manage user input
        AnimalPlanet.HandleInput(deltaTime);

        // Update game state
        AnimalPlanet.UpdateGameState(deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        AnimalPlanet.DrawFrame();

        glfwSwapBuffers(window);

        // Check if quit was requested
        if (AnimalPlanet.quitRequested) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    // Deallocate resources
    TextureManager::Clear();
    ShaderManager::Clear();


    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Key event handling
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            AnimalPlanet.Keys[key] = true;
        else if (action == GLFW_RELEASE) {
            AnimalPlanet.Keys[key] = false;
            AnimalPlanet.keysProcessed[key] = false;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Resize OpenGL viewport
    glViewport(0, 0, width, height);
}

void setWindowIcon(GLFWwindow* window) {
    // Path to your icon file
    const char* iconPath = "assets/textures/favicon.png";

    int width, height, nrChannels;
    // stbi_load loads the image and fills width, height, and nrChannels
    unsigned char* data = stbi_load(iconPath, &width, &height, &nrChannels, 0);

    if (data) {
        GLFWimage icon;
        icon.width = width;
        icon.height = height;
        icon.pixels = data;

        // Set the window icon
        glfwSetWindowIcon(window, 1, &icon);

        // Free the image memory
        stbi_image_free(data);
    }
    else {
        std::cerr << "Failed to load icon image." << std::endl;
    }
}
