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
#include <queue>
#include <mutex>
#include "Event.h"

// Forward declare window class.
class Application;

class Window {  
    public:
        Window();
        ~Window();
        void createWindow();
        void linkApplication(Application* app); // Links this window with an app instance, allowing this window to modify the app.
        Application* getApplication();
        ALCdevice* getAudioDevice();
        struct Configuration {
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
        // Data for rendering layers to the screen.
        struct Quad {
            unsigned int program, VAO, EBO;
        };
        Configuration config;
        Quad quad;
        GLFWwindow* getWindow();
        template<class L> void addLayer(std::shared_ptr<L> layer) {
            static_assert(std::is_base_of<Layer, L>::value, "The added layer must be derived from Layer");
            layerStack.push_back(std::move(layer));
        }
        std::vector<std::shared_ptr<Layer>> layerStack;
        std::weak_ptr<Layer> getLayer(std::string name);
        TextRenderer textRenderer;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastRendered;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastHandledInputs;
        template<class E> void enqueueEvent(std::shared_ptr<E> event) {
            std::lock_guard<std::mutex> lock(mutex); // Locks thread whilst this is in scope.
            static_assert(std::is_base_of<Event, E>::value, "The added event must be derived from Event");
            eventQueue.push(std::move(event));
        }
        std::shared_ptr<Event> dequeueEvent();
        bool eventQueueIsEmpty();
        bool callbacksSetup = false;
    private:
        //Framebuffer
        GLFWwindow* window;
        ALCdevice* audioDevice;
        std::queue<std::shared_ptr<Event>> eventQueue;
        std::mutex mutex;
        Application* application;
};
