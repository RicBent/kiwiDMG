#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>

#include "core/dmg.h"

struct GLFWwindow;

class EmuWindow
{
public:
    int run();

private:
    void initialize();
    void cleanup();

    void update();
    void render();

    void reset();
    void pause(bool pause = true);
    void loadDialog();

    static void windowSizeCallback(GLFWwindow* window, int width, int height);
    void sizeChanged(int width, int height);

    static void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void keyPressed(int key, int scancode, int action, int mods);

    GLFWwindow* window;
    int width, height;

    DMG dmg;
    bool running;
    bool paused;

    GLuint texture;
    uint32_t* frameData;
};