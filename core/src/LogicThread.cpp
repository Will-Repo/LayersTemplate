#include "LogicThread.h"
#include <thread>
#include <iostream>
#include "Layer.h"

LogicThread::LogicThread() {
    }

LogicThread::~LogicThread() {
    if (thread.joinable()) {
        thread.join(); // Wait for thread to finish before destroying class object to prevent core dump.
    }
}

void LogicThread::startUpdating() {
    thread = std::thread(&LogicThread::updateWindows, this);
}

void LogicThread::updateWindows() {
    // Check all windows in the thread group share the same framerate.

    int framerate = layers[0]->config.updateFrameLimit;
    for (int i = 1; i < layers.size(); ++i) {
        if (layers[i]->config.updateFrameLimit != framerate) {
            std::cout << "Layers on same thread have different framerates." << std::endl;
            //TODO: Error handling.
        }
    }

    float frameTime = 1.0 / framerate;

    // Timestep and deltatime are completely seperate.
    //Deltatime variables.
    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

    //TODO: ADD RUNNING CHECK, AND HANDLE WHEN WINDOWS ARE DELETED.
    while (true) {
        //Calculating timestep.
        timestep = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastTime).count();
        lastTime = std::chrono::high_resolution_clock::now();

        // Update each layer.
        for (auto& layer : layers) {
            layer->onUpdate(timestep);
        }

        // Calculating deltatime, in seconds.
        deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - oldDelta).count();
        if (frameTime - deltaTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float> (frameTime - deltaTime));
        }
        oldDelta = std::chrono::high_resolution_clock::now();
    }
    
    /*// Wait until all windows have initialised.
    bool notInitialised = true;
    while (notInitialised) {
        notInitialised = false;
        for (Window* window : windows) {
            if (!window->getWindow()) {
                notInitialised = true;
            }
        }
    }
    for (Window* window : windows) {
        std::cout << "Window: " << window->getWindow() << std::endl;
    }

    std::cout << "Started rendering" << std::endl;
    // Start rendering.
    while (true) {
        int numWindows = windows.size();
        int i = 0;

        //glfwPollEvents();
        while (i < numWindows) {
            GLFWwindow* window = windows[i]->getWindow();

            std::cout << "Hi" <<std::endl;
            if (glfwWindowShouldClose(window)) {
                //TODO: Fix memory leak here. Window only removed from rendering, not destroyed.
                windows.erase(windows.begin() + i);
                --numWindows;
                std::cout << "Closed window." << std::endl;
                continue;
            }
            std::cout << "ello" <<std::endl;
            glfwMakeContextCurrent(window);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            ++i;
        }
    }

    std::cout << "Ended rendering" << std::endl;*/
}

void LogicThread::addLayer(Layer* layer) {
    layers.push_back(layer);
}
