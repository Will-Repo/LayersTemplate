#include "MousePositionEvent.h"

MousePositionEvent::MousePositionEvent(double xpos, double ypos) : xpos(xpos), ypos(ypos) {
    type = EventType::MousePositionEvent;
}
