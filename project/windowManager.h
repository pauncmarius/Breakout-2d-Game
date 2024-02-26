#ifndef GRAPHICS_INITIALIZER_H
#define GRAPHICS_INITIALIZER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class WindowManager {
public:
    WindowManager(int screenWidth, int screenHeight, const std::string& windowTitle);
    ~WindowManager();

    bool initialize();
    GLFWwindow* getWindow() const;

private:
    int screenWidth;
    int screenHeight;
    std::string windowTitle;
    GLFWwindow* window;

    bool initializeGLFW();
    bool initializeWindow();
    bool initializeGLAD();
};

#endif // GRAPHICS_INITIALIZER_H
