#include "Application.h"
#include <iostream>
#include "Window.h"
#include "Layer.h"
#include <chrono>
#include <thread>
#include "LogicThreadManager.h"
#include <GLFW/glfw3.h>

Application::Application() {
    std::cout << "Hello world";

    if (!glfwInit()) {
        //TODO: Error handling
    }
}

void Application::addWindow(Window& window) {
    window.createWindow();
    windowStack.push_back(&window);
    std::cout << "Added window: " << window.config.windowName << "." << std::endl;
}

void Application::run() {
    // Add each window to its thread for layer updating.
    for (Window* window : windowStack) {
        threadManager.startWindow(window);
    }

    // Start logic update threads.
    threadManager.startThreads();

    // Begin rendering.
    while (true) {
        int numWindows = windowStack.size();
        int i = 0;

        glfwPollEvents();
        while (i < numWindows) {
            GLFWwindow* window = windowStack[i]->getWindow();

            if (glfwWindowShouldClose(window)) {
                //TODO: Fix memory leak here. Window only removed from rendering, not destroyed.
                windowStack.erase(windowStack.begin() + i);
                --numWindows;
                std::cout << "Closed window." << std::endl;
                continue;
            }
            glfwMakeContextCurrent(window);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            ++i;
        }
    }
}
