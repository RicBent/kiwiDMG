#include "emuwindow.h"
#include "tinyfiledialogs.h"

#include <iostream>

// TODO: Use Display constants from DMG

int EmuWindow::run()
{
    initialize();

    while (!glfwWindowShouldClose(window))
    {
        update();
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanup();

    return EXIT_SUCCESS;
}

void EmuWindow::initialize()
{
    if (!glfwInit())
    {
        std::cerr << "Fatal Error: Initing GLFW failed!"<< std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(160*2, 144*2, "kiwiDMG", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Fatal Error: Creating GLFW window failed!"<< std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeLimits(window, 160, 144, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwGetWindowSize(window, &width, &height);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetKeyCallback(window, windowKeyCallback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        std::cerr << "Fatal Error: Initializing GL functions failed!"<< std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);


    paused = true;
    running = false;


    frameData = new uint32_t[160*144];

    glGenTextures(1, &texture);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 160, 144, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, frameData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void EmuWindow::cleanup()
{
    glDeleteTextures(1, &texture);

    delete[] frameData;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void EmuWindow::update()
{
    if (!running || paused)
        return;

    dmg.runToVBlank();

    // Update Framebuffer
    for (int y = 0; y < 144; y++)
    {
        for (int x = 0; x < 160; x++)
        {
            uint32_t palette[4] = {0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};

            uint32_t c = palette[dmg.getFrame()[y*160 + x]];
            frameData[y*160 + x] = c;
        }
    }
}

void EmuWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!running)
        return;

    if (!paused)
    {
        // Update Texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 160, 144, GL_RGBA, GL_UNSIGNED_BYTE, frameData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    // Update Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);

    // Render quad with texture
    glBegin(GL_QUADS);
        glTexCoord2d(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2d(0.0f, 1.0f);
        glVertex2f(0.0f, height);
        glTexCoord2d(1.0f, 1.0f);
        glVertex2f(width, height);
        glTexCoord2d(1.0f, 0.0f);
        glVertex2f(width, 0.0f);
    glEnd();
}

void EmuWindow::reset()
{
    dmg.reset();
    running = true;
    paused = false;
}

void EmuWindow::pause(bool pause)
{
    if (running)
        paused = pause;
}

void EmuWindow::loadDialog()
{
    // tinyfd may not be the best solution
    // Native dialogs for settings/etc are planned so it would be replaced anyways

    const char* path = tinyfd_openFileDialog("kiwiDMG - Load ROM", "roms", 0, nullptr, nullptr, false);
    if (!path)
        return;

    if (dmg.mmu->loadCartridge(path))
    {
        dmg.reset();
        running = true;
        paused = false;
    }
}

void EmuWindow::windowSizeCallback(GLFWwindow* window, int width, int height)
{
    EmuWindow* emuWindow = static_cast<EmuWindow*>(glfwGetWindowUserPointer(window));
    emuWindow->sizeChanged(width, height);
}

void EmuWindow::sizeChanged(int width, int height)
{
    this->width = width;
    this->height = height;
}

void EmuWindow::windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    EmuWindow* emuWindow = static_cast<EmuWindow*>(glfwGetWindowUserPointer(window));
    emuWindow->keyPressed(key, scancode, action, mods);
}

void EmuWindow::keyPressed(int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
    case GLFW_KEY_L:
        loadDialog();
        break;

    case GLFW_KEY_R:
        reset();
        break;

    case GLFW_KEY_P:
        pause(!paused);
        break;

    default:
        break;
    }
}
