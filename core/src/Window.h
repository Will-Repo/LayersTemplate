#include <string>
#include <GLFW/glfw3.h>

#pragma once

class Window {  
    public:
        Window();
        void createWindow();
        struct configuration {
            int framerate = 60; //Rendering framerate.
            std::string windowName = "Window"; //Should be overwritten, or else issues can occur in multiwindowed software.
            std::string windowDesc = "Window description";
            int threadGroup = 0; // If higher than available threads, will join main thread group 0.
            bool vsync = false; //If true, will override framerate.
            bool running = false;
            int width = 1920;
            int height = 1080;
        };
        configuration config;
    private:
        //Framebuffer
        GLFWwindow* window;
};
