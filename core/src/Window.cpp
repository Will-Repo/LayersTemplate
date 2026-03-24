#include "Window.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <AL/al.h>
#include <AL/alc.h>

Window::Window() {
    audioDevice = alcOpenDevice(NULL);
    //TODO: Device enumeration
}

Window::~Window() {
    std::cout << "Destroyed window object" << std::endl;
    glfwDestroyWindow(window);
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

ALCdevice* Window::getAudioDevice() {
    return audioDevice;
}
