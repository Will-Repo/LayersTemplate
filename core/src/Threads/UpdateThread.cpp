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
    thread = std::thread(&LogicThread::updateLayers, this);
}

void LogicThread::updateLayers() {
    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> now;

    while (layers.size() > 0) {
        // Update each layer.
        for (auto& layer : layers) {
            // See if enough time has elapsed to call for update.
            now = std::chrono::high_resolution_clock::now();
            timestep = std::chrono::duration<float>(now - layer->lastUpdated).count();
            if (timestep >= (1.0 / layer->updateFrameLimit)) {
                layer->lastUpdated = now;
                layer->onUpdate(timestep);
            }
        }

        /*// Calculating deltatime, in seconds.
        deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - oldDelta).count();
        if (frameTime - deltaTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float> (frameTime - deltaTime));
        }
        oldDelta = std::chrono::high_resolution_clock::now();*/
    }
}

void LogicThread::addLayer(Layer* layer) {
    layers.push_back(layer);
}
