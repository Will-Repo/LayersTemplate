#include "Window.h"
#include <iostream>
#include <GLFW/glfw3.h>

Window::Window() {
    std::cout << "Hello window";
}

void Window::createWindow() {
    window = glfwCreateWindow(config.width, config.height, config.windowName.c_str(), NULL, NULL);

    //TODO: Error check
}
