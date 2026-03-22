#pragma once

#include "Window.h"
#include <vector>
#include <thread>

class LogicThread {
    public:
        LogicThread(); // Loops through all windows on this thread group.
        ~LogicThread();
        void addLayer(Layer* layer);
        void startUpdating();
        void updateLayers();
    private:
        std::vector<Layer*> layers;
        std::thread thread;
};
