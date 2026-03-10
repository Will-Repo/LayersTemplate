#include <string>

#pragma once

class Window {  
    public:
        Window();
        struct configuration {
            int framerate = 60; //Rendering framerate.
            std::string windowName = "Window"; //Should be overwritten, or else issues can occur in multiwindowed software.
            std::string windowDesc = "Window description";
            bool independentThread = false; //If false, will join main rendering thread. Else, will have new thread created for it. If only one window, main thread is used regardless.
            bool vsync = false; //If true, will override framerate.
            bool running = false;
        };
        configuration config;
        void start(); //Manages which thread window is displayed on, and adds it to that threads queue.
    private:
        //Framebuffer
};
