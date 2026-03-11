#include "ThreadManager.h"
#include "Thread.h"
#include <map>
#include <iostream>
#include <memory>
//TODO: check thread count is less than whats available.

void ThreadManager::startWindow(Window* window) {
    std::cout << "Window started" << std::endl;
    // Check if thread group exists in the map.
    int group = window->config.threadGroup;
    std::cout << "Thread group: " << group << std::endl;
    auto it = threadGroups.find(group);
    if (it != threadGroups.end()) { //If found, add window to group.
        std::cout << "Thread group found." << std::endl;
        it->second->addWindow(window);
    } else { //If this thread group doesnt exist yet, create it and add the window.
        std::cout << "Thread group not found." << std::endl;
        std::unique_ptr<Thread> threadPtr = std::make_unique<Thread>();
        threadPtr->addWindow(window);
        threadGroups[group] = std::move(threadPtr);
    }
}
