#include "RenderingThread.h"
#include <thread>
#include <iostream>
#include "Window.h"
#include "FilePaths.h"

RenderingThread::RenderingThread(FilePaths* paths) : filePaths(paths) {
}

RenderingThread::~RenderingThread() {
    if (thread.joinable()) {
        thread.join(); // Wait for thread to finish before destroying class object to prevent core dump.
    }
}

void RenderingThread::startRendering() {
    thread = std::thread(&RenderingThread::renderWindows, this);
}

// Loop at max window render limit, but limit each window to its own, and each layer to its own limit.
void RenderingThread::renderWindows() {
    int maxFrameLimit = 0;
    for (Window* window : windows) {
        if (window->config.renderingFrameLimit > maxFrameLimit) {
            maxFrameLimit = window->config.renderingFrameLimit;
        }
    }
    float frameTime = 1.0 / maxFrameLimit;

    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> now;

    while (windows.size() > 0) {
        for (Window* window : windows) {
            float windowFrameTime = 1.0 / window->config.renderingFrameLimit;
            now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration<float>(now - window->lastRendered).count() >= windowFrameTime) {            
                //std::cout << "Making window " << window->config.windowName << " current." << std::endl;
                //std::cout << "Window: " << window->getWindow() << "." << std::endl;
                glfwMakeContextCurrent(window->getWindow());
                glClear(GL_COLOR_BUFFER_BIT);

                // Render each layer, if its passed their frame time.
                for (auto& layer : window->layerStack) {
                    // See if enough time has elapsed to call for update.
                    now = std::chrono::high_resolution_clock::now();
                    timestep = std::chrono::duration<float>(now - layer->lastRendered).count();
                    if (timestep >= (1.0 / layer->config.renderingFrameLimit)) {
                        layer->lastRendered = now;
                        layer->onRender(window, filePaths); 
                    }
                }
                glfwSwapBuffers(window->getWindow());
            }
        }

        // Calculating deltatime, in seconds.
        deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - oldDelta).count();
        if (frameTime - deltaTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float> (frameTime - deltaTime));
        }
        oldDelta = std::chrono::high_resolution_clock::now();
    }
}

void RenderingThread::addWindow(Window* window) {
    windows.push_back(window);
}
