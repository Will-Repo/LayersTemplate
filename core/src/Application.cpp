#include "Application.h"
#include <iostream>
#include "Window.h"

Application::Application() {
    std::cout << "Hello world";
}

void Application::addWindow(Window window) {
    windowStack.push_back(window);
    std::cout << "Added window";
}
