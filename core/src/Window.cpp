#include "Window.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <AL/al.h>
#include <AL/alc.h>
#include "Application.h"

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


void Window::linkApplication(Application* app) {
    application = app;
}

Application* Window::getApplication() {
    return application;
}  

GLFWwindow* Window::getWindow() {
    return window;
}

std::weak_ptr<Layer> Window::getLayer(std::string name) {
    for (std::shared_ptr<Layer>& layer : layerStack) {
        if (layer->config.name == name) {
            return std::weak_ptr<Layer>(layer);
        }
    }
    return std::weak_ptr<Layer>();
}

ALCdevice* Window::getAudioDevice() {
    return audioDevice;
}

std::shared_ptr<Event> Window::dequeueEvent() {
    std::lock_guard<std::mutex> lock(mutex); 
    if (eventQueue.empty()) {
        return nullptr;
    } else {
       //TODO: Check if best code practice followed here. 
        auto event = eventQueue.front();
        eventQueue.pop();
        return event;            
    }
}

bool Window::eventQueueIsEmpty() {
    std::lock_guard<std::mutex> lock(mutex); 
    return eventQueue.empty();
}
