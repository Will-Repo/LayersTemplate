#pragma once

#include "Event.h"
//#include "Window.h" //Using forward declaration instead, as there is a circular dependency, and this gets skipped if included already so layer doesn't know about window.
#include "FilePaths.h"

class Window;

struct configuration {
    int updateFrameLimit = 60; // Times onUpdate called per second.    
    int updateThreadGroup = 0; // Thread group must all have same framerate, and if value higher than available threads, will join main thread.
    int renderingFrameLimit = 60;  
};

class Layer {
    public:
        Layer() {};
        virtual ~Layer() {}; //Close resourses.
        virtual void loadData(Window* window, FilePaths* filePaths) {};
        virtual void onUpdate(float timestep) {};
        virtual void onEvent(Event& event) {};
        virtual void onRender(Window* window, FilePaths* filePaths) {};
        configuration config;
    private:
};
