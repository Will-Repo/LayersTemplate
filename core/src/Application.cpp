#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Application.h"
#include <iostream>
#include "Window.h"
#include "Layer.h"
#include <chrono>
#include <thread>
#include "ThreadManager.h"
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

void Application::addWindow(std::shared_ptr<Window> window) {
    std::cout << "Added window: " << window->config.windowName << "." << std::endl;
    window->createWindow();
    windowStack.push_back(std::move(window));
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
    glfwMakeContextCurrent(NULL);

    // Load the data for each layer with the correct context. Basic rendering data. TODO: Check if necessary.
    for (const auto& window : windowStack) {
        glfwMakeContextCurrent(window->getWindow());
        for (auto& layer : window->layerStack) {
            layer->loadData(window, &config.paths); //Pass in window pointer so the layer can access window specific data.
        }
    }
    glfwMakeContextCurrent(NULL); // Make sure contexts are released for use in multithreaded rendering.

    // Add each window to its thread for layer updating.
    for (const auto& window : windowStack) {
        for (const auto& layer : window->layerStack) {
            threadManager.addUpdateLayer(layer); // Will be cast to weak ptr before being stored.
        }
    }

    // Add each window for rendering and input handling. Passing in weak pointer so it can automaically tell when object is destroyed.
    for (const auto& window : windowStack) {
        threadManager.addRenderingWindow(window, &config.paths);
        threadManager.addInputWindow(window);
    }

    // Start logic update threads.
    threadManager.startAllThreads();


    float frameTime = 1.0 / config.recieveInputsLimit;
    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;
    // Poll events, and check if each window has closed.
    while (config.running) {
        int numWindows = windowStack.size();
        if (numWindows == 0) {
            //config.running = false;
            //continue; //TODO: Proper exit script, this still runs as other threads are still running.
            exit(1);
        }
        int i = 0;

        glfwPollEvents();
        while (i < numWindows) {
            GLFWwindow* window = windowStack[i]->getWindow();

            if (glfwWindowShouldClose(window)) {
                //TODO: Fix memory leak here. Window only removed from rendering, not destroyed.
                //TODO: Instead pass weak pointers to other threads, and check if window has been destroyed.
                // Destroy window once other threads stop using it. Other threads have weak pointers and will check if window has been destoyed before doing operations.
                windowStack[i].reset();
                windowStack.erase(windowStack.begin() + i);
                --numWindows;
                std::cout << "Closed window." << std::endl;
                continue;
            }
            
            ++i;
        }

        deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - oldDelta).count();
        if (frameTime - deltaTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float> (frameTime - deltaTime));
        }
        oldDelta = std::chrono::high_resolution_clock::now();
    }
}
