//framebuffer
//Seperate thread, if available, and in config.

#include <string>

class Window {  
    public:
        Window();
        struct configuration {
            int framerate = 60; //Rendering framerate.
            std::string windowName = "Window"; //Should be overwritten, or else issues can occur in multiwindowed software.
            std::string windowDesc = "Window description";
            bool independentThread = false; //If false, will join main rendering thread. Else, will have new thread created for it. If only one window, main thread is used regardless.
            bool vsync = false; //If true, will override framerate.
        };
        configuration config;

    private:
};
