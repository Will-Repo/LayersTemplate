#include <string>
#include <GLFW/glfw3.h>
#include <memory>
#include "Layer.h"
#include <vector>

#pragma once

class Window {  
    public:
        Window();
        void createWindow();
        struct configuration {
            std::string windowName = "Window"; //Should be overwritten, or else issues can occur in multiwindowed software.
            std::string windowDesc = "Window description";
            bool running = false;
            int width = 1920;
            int height = 1080;
        };
        configuration config;
        GLFWwindow* getWindow();
        template<class L> void addLayer(L* layer) {
            static_assert(std::is_base_of<Layer, L>::value, "The added layer must be derived from Layer");
            layerStack.push_back(layer);
        }
        std::vector<std::unique_ptr<Layer>> layerStack;
    private:
        //Framebuffer
        GLFWwindow* window;
};
