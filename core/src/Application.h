#pragma once
#include <string>
#include <vector>
#include "Window.h"
#include <memory>
#include "ThreadManager.h"
#include "FilePaths.h"

class Application {
    public: 
        Application();
        struct Configuration {
            int mainThreadLimit = 240; //This is the framerate of the main thread loop, which controls (the maximum rate of) rendering and input recieving.
            int recieveInputsLimit = -1; // -1 is no limit, runs as fast as mainThreadLimit
            int renderingCallsLimit = -1;
            std::string appName = "Application";
            std::string appDesc = "Application description"; //Might remove later.
            //bool vsync = false;
            bool running = true;
            FilePaths paths;
        };
        Configuration config;
        void addWindow(std::shared_ptr<Window> window);
        void removeWindow(std::string windowName);
        bool hasWindow(std::string windowName);
        void run();
    private:
        std::vector<std::shared_ptr<Window>> windowStack;
        bool running = true;
        ThreadManager threadManager;
        bool setup = false;
};
