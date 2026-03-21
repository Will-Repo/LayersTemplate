#include "LogicThreadManager.h"
#include "LogicThread.h"
#include <map>
#include <iostream>
#include <memory>
//TODO: check thread count is less than whats available.

void LogicThreadManager::addLayer(Layer* layer) {
    //std::cout << "Window started" << std::endl;
    // Check if thread group exists in the map.
    int group = layer->config.updateThreadGroup;
    //std::cout << "Thread group: " << group << std::endl;
    auto it = threadGroups.find(group);
    if (it != threadGroups.end()) { //If found, add window to group.
        //std::cout << "Thread group found." << std::endl;
        it->second->addLayer(layer);
    } else { //If this thread group doesnt exist yet, create it and add the window.
        //std::cout << "Thread group not found." << std::endl;
        std::unique_ptr<LogicThread> threadPtr = std::make_unique<LogicThread>();
        threadPtr->addLayer(layer);
        threadGroups[group] = std::move(threadPtr);
    }
}

void LogicThreadManager::startThreads() {
    for (auto& [id, group] : threadGroups) {
        group->startUpdating();
    }
}
