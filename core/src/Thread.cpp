#include "Thread.h"
#include <thread>
#include <iostream>

Thread::Thread() {
    thread = std::thread(&Thread::renderWindows, this);
}

Thread::~Thread() {
    if (thread.joinable()) {
        thread.join(); // Wait for thread to finish before destroying class object to prevent core dump.
    }
}

void Thread::renderWindows() {
    while (true) {
        //std::cout << "Rendering window";
    }
}

void Thread::addWindow(Window* window) {
    windows.push_back(window);
}
