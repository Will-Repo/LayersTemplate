#include "Event.h"

#pragma once

class Layer {
    public:
        virtual void onUpdate(float timestep);
        virtual void onEvent(Event& event);
        virtual void onRender();
    private:
};
