#include <string>
#include <vector>
#include "Window.h"
#include <memory>
#include "LogicThreadManager.h"
#include "FilePaths.h"

#pragma once

class Application {
    public: 
        Application();
        struct configuration {
            int framerate = 60; //This is the framerate game logic runs at, not rendering rate.
            std::string appName = "Application";
            std::string appDesc = "Application description"; //Might remove later.
            bool vsync = false;
            bool running = true;
            FilePaths paths;
        };
        configuration config;
        void addWindow(Window& window);
        void run();
    private:
        std::vector<Window*> windowStack;
        bool running = true;
        LogicThreadManager threadManager;
};
