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
void ThreadManager::addUpdateLayer(Layer* layer) {
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
void ThreadManager::addRenderingWindow(Window* window) {
    // Check if thread group exists already. 
    int group = window->config.renderingThreadGroup;
    auto it = renderingThreadGroups.find(group);
    //If found, add window to group. If not, create group and assign layer to new group.
    if (it != renderingThreadGroups.end()) {
        // Get update thread and assign layer to it.
        it->second->addWindow(window);
    } else {        
        std::unique_ptr<RenderingThread> threadPtr = std::make_unique<RenderingThread>();
        threadPtr->addWindow(window);
        renderingThreadGroups[group] = std::move(threadPtr);
    }
}

void ThreadManager::addInputWindow(Window* window) {
    // Check if thread group exists already. 
    int group = layer->config.inputHandlingGroup;
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

void ThreadManager::startLayerUpdating() {
    for (auto& [id, thread] : updateThreadGroups) {
        // Start all update threads.
        group->startUpdating();
    }
}

void ThreadManager::startWindowRendering() {
    for (auto& [id, group] : renderingThreadGroups) {
        // Start each thread group's rendering.
        group->startRendering();
    }
}

void ThreadManager::startHandlingInputs() {
    for (auto& [id, group] : inputThreadGroups) {
        // Start each thread group's input handling.
        group->startInputHandling();
    }
}
