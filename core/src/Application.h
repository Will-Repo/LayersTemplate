#include <string>
#include <vector>
#include "Window.h"

#pragma once

class Application {
    public: 
        Application();
        struct configuration {
            int framerate = 60; //This is the framerate game logic runs at, not rendering rate.
            std::string appName = "Application";
            std::string appDesc = "Application description"; //Might remove later.
        };
        configuration config;
        void addWindow(Window window);
        //Add layer
    private:
        //Layer stack
        std::vector<Window> windowStack;
};
