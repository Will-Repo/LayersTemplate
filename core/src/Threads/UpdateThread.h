#pragma once

#include "Window.h"
#include <vector>
#include <thread>
#include <memory>

class UpdateThread {
    public:
        UpdateThread(); // Loops through all windows on this thread group.
        ~UpdateThread();
        void addLayer(std::weak_ptr<Layer> layer);
        void startUpdating();
        void updateLayers();
    private:
        std::vector<std::weak_ptr<Layer>> layers;
        std::thread thread;
};
