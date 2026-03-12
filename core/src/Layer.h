#include "Event.h"

#pragma once

class Layer {
    public:
        virtual void onUpdate(float timestep);
        virtual void onEvent(Event& event);
        virtual void onRender();
        struct configuration {
            int framerate = 60; // Logic framerate
            int threadGroup = 0; // Thread group must all have same framerate, and if value higher than available threads, will join main thread.
        };
        configuration config;
    private:
};
