#pragma once

#include "Window.h"
#include <vector>
#include <thread>

class Thread {
    public:
        Thread(); // Loops through all windows on this thread group.
        ~Thread();
        void addWindow(Window* window);
        void renderWindows();
    private:
        std::vector<Window*> windows;
        std::thread thread;
};
