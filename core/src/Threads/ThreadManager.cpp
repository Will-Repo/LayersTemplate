#include "ThreadManager.h"
#include "UpdateThread.h"
#include "RenderingThread.h"
#include "InputThread.h"
#include <map>
#include <iostream>
#include <memory>
//TODO: check thread count is less than whats available.

// This class must support threads for layer updating, window rendering, event management, event recieving - anything that happens on seperate thread to main.
ThreadManager::ThreadManager() {}

// Add layer to any thread group, no limitations. Issues may occur if the same window is assigned to multiple groups. TODO: Add a check for this?
void ThreadManager::addUpdateLayer(std::shared_ptr<Layer> layer) {
    // Check if thread group exists already. 
    int group = layer->config.updateThreadGroup;
    auto it = updateThreadGroups.find(group);
    //If found, add window to group. If not, create group and assign layer to new group.
    if (it != updateThreadGroups.end()) {
        // Get update thread and assign layer to it.
        it->second->addLayer(layer);
    } else {        
        std::unique_ptr<UpdateThread> threadPtr = std::make_unique<UpdateThread>();
        threadPtr->addLayer(layer);
        updateThreadGroups[group] = std::move(threadPtr);
    }
}

// Add window to any thread group. Window added must be unique.
void ThreadManager::addRenderingWindow(std::shared_ptr<Window> window, FilePaths* filePaths) {
    // Check if thread group exists already. 
    int group = window->config.renderingThreadGroup;
    auto it = renderingThreadGroups.find(group);
    //If found, add window to group. If not, create group and assign layer to new group.
    if (it != renderingThreadGroups.end()) {
        // Get update thread and assign layer to it.
        it->second->addWindow(window);
    } else {        
        std::unique_ptr<RenderingThread> threadPtr = std::make_unique<RenderingThread>(filePaths);
        threadPtr->addWindow(window);
        renderingThreadGroups[group] = std::move(threadPtr);
    }
}

void ThreadManager::addInputWindow(std::shared_ptr<Window> window) {
    // Check if thread group exists already. 
    int group = window->config.inputHandlingGroup;
    auto it = inputThreadGroups.find(group);
    //If found, add window to group. If not, create group and assign layer to new group.
    if (it != inputThreadGroups.end()) {
        // Get update thread and assign layer to it.
        it->second->addWindow(window);
    } else {        
        std::unique_ptr<InputThread> threadPtr = std::make_unique<InputThread>();
        threadPtr->addWindow(window);
        inputThreadGroups[group] = std::move(threadPtr);
    }
}

void ThreadManager::startAllThreads() {
    startLayerUpdating();
    startWindowRendering();
    startHandlingInputs();
}

void ThreadManager::startLayerUpdating() {
    for (auto& [id, group] : updateThreadGroups) {
        // Start all update threads.
        if (!group->started) {
            group->startUpdating();
        }
    }
}

void ThreadManager::startWindowRendering() {
    for (auto& [id, group] : renderingThreadGroups) {
        // Start each thread group's rendering.
        if (!group->started) {
            group->startRendering();
        }
    }
}

void ThreadManager::startHandlingInputs() {
    for (auto& [id, group] : inputThreadGroups) {
        // Start each thread group's input handling.
        if (!group->started) {
            group->startEventHandling();
        }
    }
}
