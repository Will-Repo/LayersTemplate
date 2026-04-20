#include "UpdateThread.h"
#include <thread>
#include <iostream>
#include "Layer.h"
#include <mutex>

UpdateThread::UpdateThread() {
}

UpdateThread::~UpdateThread() {
    if (thread.joinable()) {
        thread.join(); // Wait for thread to finish before destroying class object to prevent core dump.
    }
}

void UpdateThread::startUpdating() {
    started = true;
    thread = std::thread(&UpdateThread::updateLayers, this);
    thread.detach();
}

void UpdateThread::updateLayers() {
    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> now;

    do {
        while (!newLayerQueueIsEmpty()) {
            auto layerPtr = dequeueNewLayer();
            if (auto layer = layerPtr.lock()) {                
                layers.push_back(layer);
            }
        }

        std::lock_guard<std::mutex> lock(mutex); 
        // Update each layer.
        for (auto it = layers.begin(); it != layers.end();) {
            // If window is not destroyed, handle its events. Else, destroy the window pointer.
            if (auto layer = it->lock()) {
                if (layer->config.updateFrameLimit == 0) {
                    continue;
                }
                // See if enough time has elapsed to call for update.
                now = std::chrono::high_resolution_clock::now();
                timestep = std::chrono::duration<float>(now - layer->lastUpdated).count();
                float updateFrameLimit = 1.0 / layer->config.updateFrameLimit;
                if (timestep >= updateFrameLimit) {
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
    } while (layers.size() > 0);
    started = false;
}

void UpdateThread::addLayer(std::weak_ptr<Layer> layer) {
    enqueueNewLayer(std::move(layer));
}
