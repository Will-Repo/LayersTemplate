#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Layer.h"
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include "TextRenderer.h"
#include <chrono>

class Window {  
    public:
        Window();
        ~Window();
        void createWindow();
        ALCdevice* getAudioDevice();
        struct configuration {
            std::string windowName = "Window"; //Should be overwritten, or else issues can occur in multiwindowed software.
            std::string windowDesc = "Window description";
            bool open = true; // True means window is displayed and data saved - if false object will be destroyed.
            bool running = false; // When false window will stop updating, rendering and recieving inputs, saving processing.
            int width = 1920;
            int height = 1080;
            int inputHandlingRate = 60; // Rate at which the window handles the inputs it recieves from the appliction callbacks.
            int inputHandlingGroup = 0;
            int renderingFrameLimit = 60;
            int renderingThreadGroup = 0;
        };
        configuration config;
        GLFWwindow* getWindow();
        template<class L> void addLayer(std::shared_ptr<L> layer) {
            static_assert(std::is_base_of<Layer, L>::value, "The added layer must be derived from Layer");
            layerStack.push_back(std::move(layer));
        }
        std::vector<std::shared_ptr<Layer>> layerStack;
        TextRenderer textRenderer;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastRendered;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastHandledInputs;
    private:
        //Framebuffer
        GLFWwindow* window;
        ALCdevice* audioDevice;
};
