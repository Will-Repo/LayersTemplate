#include "Window.h"
#include <iostream>

Window::Window() {
    std::cout << "Hello window";
}

void Window::start() {
    if (config.independentThread == true) {
        // Create a new thread to run this windows rendering on.
    } else {
        // Fetch the main rendering thread (if no other non-independent windows, this will create it).
    }
}
