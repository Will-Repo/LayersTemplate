#include "InputThread.h"
#include <thread>
#include <iostream>
#include "Window.h"
#include "FilePaths.h"
#include "Event.h"
#include <ranges>

InputThread::InputThread() {}

InputThread::~InputThread() {
    if (thread.joinable()) {
        thread.join(); // Wait for thread to finish before destroying class object to prevent core dump.
    }
}

void InputThread::startEventHandling() {
    started = true;
    thread = std::thread(&InputThread::handleEvents, this);
    thread.detach();
}

// Loop at max window render limit, but limit each window to its own, and each layer to its own limit.
void InputThread::handleEvents() {
    int maxHandlingLimit = 0;    
    bool frameSet = false; // Check for new windows at 10 fps if no window present, or only window renders at 0 fps, then use that window's fps.
    for (const auto& windowPtr : windows) {
        frameSet = true;
        // Make shared pointer from weak pointer, if window has not closed.
        if (auto window = windowPtr.lock()) {
            if (window->config.inputHandlingRate < 0) {
                maxHandlingLimit = -1;
                break;
            }
            if (window->config.inputHandlingRate > maxHandlingLimit) {
                maxHandlingLimit = window->config.inputHandlingRate;
            }

        }
    }
    float frameTime;
    if (!frameSet || maxHandlingLimit == 0) {
        frameTime = 1.0f / 10.0f;
    } else if (maxHandlingLimit == -1) {
        frameTime = 0;
    } else {
        frameTime = 1.0f / maxHandlingLimit;
    }

    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> now;

    //TODO: Best way? Perhaps just iterate through events and handle them for their respective window. Revisit once event recieving is implemented.
    do {
        while (!newWindowQueueIsEmpty()) {
            auto windowPtr = dequeueNewWindow();
            if (auto window = windowPtr.lock()) {
                std::cout << "Adding new window to event loop: " << window->config.windowName << std::endl;
                int frameLimit = window->config.inputHandlingRate;
                if (!frameSet) {
                    if (frameLimit < 0) {
                        frameTime = 0;
                    } else if (frameLimit > 0) {
                        frameTime = 1.0f / frameLimit;
                    }
                } else {
                    if (frameLimit < 0) {
                        frameTime = 0;
                    } else if (frameLimit > 0) {
                        if (1.0f / frameLimit < frameTime) {
                            frameTime = 1.0f / frameLimit;
                        }
                    }
                }
                windows.push_back(window);
            }
        }
        for (auto it = windows.begin(); it != windows.end();) {
            // If window is not destroyed, handle its events. Else, destroy the window pointer.
            if (auto window = it->lock()) {
                if (window->config.inputHandlingRate == 0) {
                    continue;
                }
                now = std::chrono::high_resolution_clock::now();
                //std::cout << "Checking inputs for window: " << window->config.windowName << std::endl;
                float windowHandlingTime = 1.0 / window->config.inputHandlingRate;
                if (std::chrono::duration<float>(now - window->lastHandledInputs).count() >= windowHandlingTime) {            
                    // Check for new events, If so, pass event down all running layers until handled.
                    while(!window->eventQueueIsEmpty()) {
                        //std::cout << "Dequeueing event" << std::endl;
                        // Dequeue an event
                        auto event = window->dequeueEvent();
                        // Handle the event
                        std::ranges::reverse_view reverse {window->layerStack};
                        for (auto& layer : reverse) {
                            layer->onEvent(event);
                            if (event->handled) {
                                break;
                            }
                        }
                    }
                }
                ++it; // Move onto next element.
            } else {
                it = windows.erase(it);
            }
        }

        // Calculating deltatime, in seconds.
        deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - oldDelta).count();
        if (frameTime - deltaTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float> (frameTime - deltaTime));
        }
        oldDelta = std::chrono::high_resolution_clock::now();
    } while (windows.size() > 0);
    started = false;
}

void InputThread::addWindow(std::weak_ptr<Window> window) {
    enqueueNewWindow(std::move(window));
}
