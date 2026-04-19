#pragma once

#include "Event.h"
//#include "Window.h" //Using forward declaration instead, as there is a circular dependency, and this gets skipped if included already so layer doesn't know about window.
#include "FilePaths.h"
#include <chrono>
#include <memory>

class Window;

struct configuration {
    std::string name;
    int updateFrameLimit = 60; // Times onUpdate called per second.    
    int updateThreadGroup = 0; // Thread group must all have same framerate, and if value higher than available threads, will join main thread.
    int renderingFrameLimit = 60; 
};

class Layer {
    public:
        Layer() {};
        virtual ~Layer(); //Close resourses.
        virtual void loadData(Window* window, FilePaths* filePaths) {}; // Called when layer is added to window.
        virtual void onUpdate(float timestep) {};
        virtual void onEvent(std::shared_ptr<Event> event) {};
        virtual void onRender() {};
        void setupLayer(Window* window, FilePaths* filepaths);
        configuration config;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdated;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastRendered;
        unsigned int renderTexture;
        unsigned int framebuffer;
        Window* window;
        FilePaths* filepaths;
        bool renderSetupComplete = false;
    private:
};
