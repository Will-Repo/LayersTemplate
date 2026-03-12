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
    // Wait until all windows have initialised.
    bool notInitialised = true;
    while (notInitialised) {
        notInitialised = false;
        for (Window* window : windows) {
            if (!window->getWindow()) {
                notInitialised = true;
            }
        }
    }
    for (Window* window : windows) {
        std::cout << "Window: " << window->getWindow() << std::endl;
    }

    std::cout << "Started rendering" << std::endl;
    // Start rendering.
    while (true) {
        int numWindows = windows.size();
        int i = 0;

        //glfwPollEvents();
        while (i < numWindows) {
            GLFWwindow* window = windows[i]->getWindow();

            std::cout << "Hi" <<std::endl;
            if (glfwWindowShouldClose(window)) {
                //TODO: Fix memory leak here. Window only removed from rendering, not destroyed.
                windows.erase(windows.begin() + i);
                --numWindows;
                std::cout << "Closed window." << std::endl;
                continue;
            }
            std::cout << "ello" <<std::endl;
            glfwMakeContextCurrent(window);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            ++i;
        }
    }

    std::cout << "Ended rendering" << std::endl;
}

void Thread::addWindow(Window* window) {
    windows.push_back(window);
}
