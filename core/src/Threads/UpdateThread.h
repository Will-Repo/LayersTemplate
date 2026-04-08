#pragma once

#include "Window.h"
#include <vector>
#include <thread>
#include <memory>
#include <mutex>

class UpdateThread {
    public:
        UpdateThread(); // Loops through all windows on this thread group.
        ~UpdateThread();
        void addLayer(std::weak_ptr<Layer> layer);
        void startUpdating();
        void updateLayers();
        void enqueueNewLayer(std::weak_ptr<Layer> layer) {
            std::lock_guard<std::mutex> lock(mutex); // Locks thread whilst this is in scope.
            newLayerQueue.push(std::move(layer));
        }
        std::weak_ptr<Layer> dequeueNewLayer() {
            std::lock_guard<std::mutex> lock(mutex); 
            if (newLayerQueue.empty()) {
                return std::weak_ptr<Layer>();;
            } else {
               //TODO: Check if best code practice followed here. 
                auto newLayer = newLayerQueue.front();
                newLayerQueue.pop();
                return newLayer;            
            }
        }
        bool newLayerQueueIsEmpty() {
            std::lock_guard<std::mutex> lock(mutex); 
            return newLayerQueue.empty();
        }
    private:
        std::vector<std::weak_ptr<Layer>> layers;
        std::thread thread;
        std::mutex mutex;
        std::queue<std::weak_ptr<Layer>> newLayerQueue;
};
