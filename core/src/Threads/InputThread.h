#pragma once

#include "Window.h"
#include <vector>
#include <thread>
#include "FilePaths.h"

class InputThread {
    public:
        InputThread(); // Loops through all windows on this thread group.
        ~InputThread();
        void addWindow(std::weak_ptr<Window> window);
        void startEventHandling();
        void handleEvents();
    private:
        std::vector<std::weak_ptr<Window>> windows;
        std::thread thread;
        FilePaths* filePaths;
};
