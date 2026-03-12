#include "Event.h"

#pragma once

struct configuration {
    int framerate = 60; // Logic framerate
    int threadGroup = 0; // Thread group must all have same framerate, and if value higher than available threads, will join main thread.
};

class Layer {
    public:
        Layer() {};
        virtual ~Layer() {}; //Close resourses.
        virtual void onUpdate(float timestep) {};
        virtual void onEvent(Event& event) {};
        virtual void onRender() {};
        configuration config;
    private:
};
