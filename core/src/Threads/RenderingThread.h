#pragma once

#include "Window.h"
#include <vector>
#include <thread>
#include "FilePaths.h"
#include <queue>
#include <mutex>

class RenderingThread {
    public:
        RenderingThread(FilePaths* paths); // Loops through all windows on this thread group.
        ~RenderingThread();
        void addWindow(std::weak_ptr<Window> windowPtr);
        void startRendering();
        void renderWindows();
        void enqueueNewWindow(std::weak_ptr<Window> window) {
            std::lock_guard<std::mutex> lock(mutex); // Locks thread whilst this is in scope.
            newWindowQueue.push(std::move(window));
        }
        std::weak_ptr<Window> dequeueNewWindow() {
            std::lock_guard<std::mutex> lock(mutex); 
            if (newWindowQueue.empty()) {
                return std::weak_ptr<Window>();;
            } else {
               //TODO: Check if best code practice followed here. 
                auto newWindow = newWindowQueue.front();
                newWindowQueue.pop();
                return newWindow;            
            }
        }
        bool newWindowQueueIsEmpty() {
            std::lock_guard<std::mutex> lock(mutex); 
            return newWindowQueue.empty();
        }
    private:
        std::vector<std::weak_ptr<Window>> windows;
        std::thread thread;
        FilePaths* filePaths;        
        std::queue<std::weak_ptr<Window>> newWindowQueue;
        std::mutex mutex;
};
