#pragma once 

enum class EventType {
    KeyEvent,
    CursorWindowEvent,
    MouseButtonEvent,
    ScrollEvent
};

class Event {
    public:
        Event();
        EventType type;
        bool handled = false;
    private:

};
