#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Application.h"
#include <iostream>
#include "Window.h"
#include "Layer.h"
#include <chrono>
#include <thread>
#include "LogicThreadManager.h"
#include "TextRenderer.h"

Application::Application() {
    if (!glfwInit()) {
        //TODO: Error handling
        std::cout << "GLFW initialisation failed." << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    TextRenderer textRenderer;
}

void Application::addWindow(Window& window) {
    window.createWindow();
    windowStack.push_back(&window);
    std::cout << "Added window: " << window.config.windowName << "." << std::endl;
}

void Application::run() {
    if (windowStack.size() == 0) {
        std::cout << "The application requires at least one window to run." << std::endl;
        exit(1);
    }
    glfwMakeContextCurrent(windowStack[0]->getWindow());
    glewExperimental = true; // Expose all extensions with valid entry points.
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        //TODO: Error handling.
        std::cout << "GLEW initialisation failed: " << glewGetErrorString(err) << "." << std::endl;
        exit(1);
    }

    // Load the data for each layer with the correct context.
    for (Window* window : windowStack) {
        glfwMakeContextCurrent(window->getWindow());
        for (auto& layer : window->layerStack) {
            layer->loadData(window, &config.paths); //Pass in window pointer so the layer can access window specific data.
        }
    }

    // Add each window to its thread for layer updating.
    for (Window* window : windowStack) {
        for (auto& layer : window->layerStack) {
            threadManager.addLayer(layer.get());
        }
    }

    // Start logic update threads.
    threadManager.startThreads();

    // Begin rendering.
    while (config.running) {
        int numWindows = windowStack.size();
        int i = 0;

        glfwPollEvents();
        while (i < numWindows) {
            GLFWwindow* window = windowStack[i]->getWindow();

            if (glfwWindowShouldClose(window)) {
                //TODO: Fix memory leak here. Window only removed from rendering, not destroyed.
                windowStack.erase(windowStack.begin() + i);
                glfwDestroyWindow(window);
                --numWindows;
                std::cout << "Closed window." << std::endl;
                continue;
            }

            glfwMakeContextCurrent(window);
            glClear(GL_COLOR_BUFFER_BIT);

            for (auto& layer : windowStack[i]->layerStack) {
                layer->onRender(windowStack[i], &config.paths);
            }

            glfwSwapBuffers(window);
            ++i;
        }
    }
}
