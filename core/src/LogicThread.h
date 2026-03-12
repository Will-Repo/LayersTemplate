#pragma once

#include "Window.h"
#include <vector>
#include <thread>

class LogicThread {
    public:
        LogicThread(); // Loops through all windows on this thread group.
        ~LogicThread();
        void addWindow(Window* window);
        void startUpdating();
        void updateWindows();
    private:
        std::vector<Window*> windows;
        std::thread thread;
};
