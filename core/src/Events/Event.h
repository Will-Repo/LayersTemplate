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
        virtual ~Event() = default; // Virtual function so class is polymorphic and dynamic casts work.
        EventType type;
        bool handled = false;
    private:

};
