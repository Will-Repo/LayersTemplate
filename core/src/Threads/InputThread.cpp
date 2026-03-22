#include "InputThread.h"
#include <thread>
#include <iostream>
#include "Window.h"
#include "FilePaths.h"

InputThread::InputThread() {}

InputThread::~InputThread() {
    if (thread.joinable()) {
        thread.join(); // Wait for thread to finish before destroying class object to prevent core dump.
    }
}

void InputThread::startEventHandling() {
    thread = std::thread(&InputThread::handleEvents, this);
}

// Loop at max window render limit, but limit each window to its own, and each layer to its own limit.
void InputThread::handleEvents() {
    int maxHandlingLimit = 0;
    for (Window* window : windows) {
        if (window->config.inputHandlingRate > maxHandlingLimit) {
            maxHandlingLimit = window->config.inputHandlingRate;
        }
    }
    float frameTime = 1.0 / maxHandlingLimit;

    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> now;

    while (windows.size() > 0) {
        for (Window* window : windows) {
            float windowHandlingTime = 1.0 / window->config.inputHandlingRate;
            now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration<float>(now - window->lastHandledInputs).count() >= windowHandlingTime) {            

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

void InputThread::addWindow(Window* window) {
    windows.push_back(window);
}
