
//windowManager.cpp
#include "windowManager.h"
#include <iostream>

WindowManager::WindowManager(int screenWidth, int screenHeight, const std::string& windowTitle)
    : screenWidth(screenWidth), screenHeight(screenHeight), windowTitle(windowTitle), window(nullptr) {}

WindowManager::~WindowManager() {
    glfwTerminate();
}

bool WindowManager::initialize() {
    if (!initializeGLFW() || !initializeWindow() || !initializeGLAD()) {
        return false;
    }
    return true;
}

GLFWwindow* WindowManager::getWindow() const {
    return window;
}

bool WindowManager::initializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    return true;
}

bool WindowManager::initializeWindow() {
    window = glfwCreateWindow(screenWidth, screenHeight, windowTitle.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    return true;
}

bool WindowManager::initializeGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return false;
    }
    glViewport(0, 0, screenWidth, screenHeight);
    return true;
}
