#include "Window.h"
#include <iostream>
#include <GLFW/glfw3.h>

Window::Window() {
    std::cout << "Hello window";
}

void Window::createWindow() {
    std::cout << "Creating window." << std::endl;
    window = glfwCreateWindow(config.width, config.height, config.windowName.c_str(), NULL, NULL);
    //TODO: Error check
    if (!window) {
        std::cout << "Window creation failed." << std::endl;
    }
}


GLFWwindow* Window::getWindow() {
    return window;
}
