#include "KeyEvent.h"

KeyEvent::KeyEvent(int pKey, int pAction) : key(pKey), action(pAction) {
    Event();
    type = EventType::KeyEvent;
}
