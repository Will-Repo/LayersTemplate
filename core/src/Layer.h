#pragma once

#include "Event.h"
//#include "Window.h" //Using forward declaration instead, as there is a circular dependency, and this gets skipped if included already so layer doesn't know about window.

class Window;

struct configuration {
    int framerate = 60; // Logic framerate
    int threadGroup = 0; // Thread group must all have same framerate, and if value higher than available threads, will join main thread.
};

class Layer {
    public:
        Layer() {};
        virtual ~Layer() {}; //Close resourses.
        virtual void loadData(Window* window) {};
        virtual void onUpdate(float timestep) {};
        virtual void onEvent(Event& event) {};
        virtual void onRender(Window* window) {};
        configuration config;
    private:
};
