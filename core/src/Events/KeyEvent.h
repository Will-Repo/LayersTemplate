#pragma once

#include "Event.h"

class KeyEvent : public Event {
    public:
        KeyEvent(int pKey, int pAction);
        int key;
        int action;
};
