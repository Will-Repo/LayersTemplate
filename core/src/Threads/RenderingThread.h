#pragma once

#include "Window.h"
#include <vector>
#include <thread>
#include "FilePaths.h"

class RenderingThread {
    public:
        RenderingThread(FilePaths* paths); // Loops through all windows on this thread group.
        ~RenderingThread();
        void addWindow(std::weak_ptr<Window> window);
        void startRendering();
        void renderWindows();
    private:
        std::vector<std::weak_ptr<Window>> windows;
        std::thread thread;
        FilePaths* filePaths;
};
