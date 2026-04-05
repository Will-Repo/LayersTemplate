#include "UpdateThread.h"
#include <thread>
#include <iostream>
#include "Layer.h"

UpdateThread::UpdateThread() {
}

UpdateThread::~UpdateThread() {
    if (thread.joinable()) {
        thread.join(); // Wait for thread to finish before destroying class object to prevent core dump.
    }
}

void UpdateThread::startUpdating() {
    thread = std::thread(&UpdateThread::updateLayers, this);
}

void UpdateThread::updateLayers() {
    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> now;

    while (layers.size() > 0) {
        // Update each layer.
        for (auto it = layers.begin(); it != layers.end();) {
            // If window is not destroyed, handle its events. Else, destroy the window pointer.
            if (auto layer = it->lock()) {
                // See if enough time has elapsed to call for update.
                now = std::chrono::high_resolution_clock::now();
                timestep = std::chrono::duration<float>(now - layer->lastUpdated).count();
                if (timestep >= (1.0 / layer->config.updateFrameLimit)) {
                    layer->lastUpdated = now;
                    layer->onUpdate(timestep);
                }
                ++it;
            } else {
                it = layers.erase(it);
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

void UpdateThread::addLayer(std::weak_ptr<Layer> layer) {
    layers.push_back(layer);
}
