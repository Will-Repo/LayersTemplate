#pragma once

#include "Window.h"
#include <vector>
#include <thread>

class UpdateThread {
    public:
        UpdateThread(); // Loops through all windows on this thread group.
        ~UpdateThread();
        void addLayer(Layer* layer);
        void startUpdating();
        void updateLayers();
    private:
        std::vector<Layer*> layers;
        std::thread thread;
};
