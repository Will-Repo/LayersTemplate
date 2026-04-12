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
#include "eventsSetup.h"

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
    window->linkApplication(this);
    for (const auto& layer : window->layerStack) {
        threadManager.addUpdateLayer(layer); // Will be cast to weak ptr before being stored.
    }
    // Add each window for rendering and input handling. Passing in weak pointer so it can automaically tell when object is destroyed.
    threadManager.addRenderingWindow(window, &config.paths);
    threadManager.addInputWindow(window);

    windowStack.push_back(std::move(window));

    // Start new threads.
    if (setup) {
        // Start any new threads, and callbacks for new windows.
        setCallbacks(windowStack);
        glfwMakeContextCurrent(NULL);
        threadManager.startAllThreads();
        glfwMakeContextCurrent(NULL);
    }
}

void Application::removeWindow(std::string windowName) {
    std::cout << "Erasing window: " << windowName << std::endl;
    std::erase_if(windowStack, [&](const std::shared_ptr<Window>& windowPtr) {
        return (windowPtr->config.windowName == windowName);
    });
}

bool Application::hasWindow(std::string windowName) {
    for (const auto& windowPtr : windowStack) {
        if (windowPtr->config.windowName == windowName) {
            return true;
        }
    }
    return false;
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

    // Set up input callbacks.
    setCallbacks(windowStack);

    glfwMakeContextCurrent(NULL); // Make sure contexts are released for use in multithreaded rendering.

    // Start logic update threads.
    threadManager.startAllThreads();
    setup = true;


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
