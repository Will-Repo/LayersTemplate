#include "Application.h"
#include <iostream>
#include "Window.h"
#include "Layer.h"
#include <chrono>
#include <thread>
#include "ThreadManager.h"
#include <GLFW/glfw3.h>

Application::Application() {
    std::cout << "Hello world";

    if (!glfwInit()) {
        //TODO: Error handling
    }
}

void Application::addWindow(Window window) {
    window.createWindow();
    windowStack.push_back(window);
    threadManager.startWindow(&window);
    std::cout << "Added and started window: " << window.config.windowName << "." << std::endl;
}

void Application::run() {
    float frameTime = 1.0 / config.framerate;

    // Timestep and deltatime are completely seperate.
    //Deltatime variables.
    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

    while (running) {
        //Calculating timestep.
        timestep = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastTime).count();
        lastTime = std::chrono::high_resolution_clock::now();
        // Update each layer.
        for (auto& layer : layerStack) {
            layer->onUpdate(timestep);
        }

        // Calculating deltatime, in seconds.
        deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - oldDelta).count();
        if (frameTime - deltaTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float> (frameTime - deltaTime));
        }
        oldDelta = std::chrono::high_resolution_clock::now();
    }
}
