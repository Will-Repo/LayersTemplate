#include <string>
#include <vector>
#include "Window.h"
#include <memory>
#include "Layer.h"
#include "ThreadManager.h"

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
        template<class L> void addLayer(L* layer) {
            static_assert(std::is_base_of<Layer, L>::value, "The added layer must be derived from Layer");
            layerStack.push_back(layer);
        }
        void run();
    private:
        std::vector<std::unique_ptr<Layer>> layerStack;
        std::vector<Window> windowStack;
        bool running = true;
        ThreadManager threadManager;
};
