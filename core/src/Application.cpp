#include "Application.h"
#include <iostream>
#include "Window.h"

Application::Application() {
    std::cout << "Hello world";
}

void Application::addWindow(Window window) {
    windowStack.push_back(window);
    window.start();
    std::cout << "Added and started window: " << window.config.windowName << "." << std::endl;
}

void Application::run() {
    while (running) {
        // Update each layer.
        for (auto& layer : layerStack) {
            //layer->
        }
    }
}
