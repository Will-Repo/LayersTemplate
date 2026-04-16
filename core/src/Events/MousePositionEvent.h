#pragma once

#include "Event.h"

class MousePositionEvent : public Event {
    public:
        MousePositionEvent(double xpos, double ypos);
        double xpos, ypos;
};
