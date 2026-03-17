#include "Application.h"
#include "Window.h"

#include "example/BaseLayer.h"
#include "example/SecondaryLayer.h"

int main() {
    //Make application object, set up attributes.
    Application app = Application();
    struct Application::configuration* appconf = &app.config; //Or access directly.
    appconf->framerate = 120; // Rendering framerate.
    appconf->appName = "App";
    appconf->appDesc = "App description";
    appconf->vsync = false;
    appconf->running = true;
    //Set up data about file locations for the project - multiple reads are thread safe, don't combining reading and writing on multiple threads. Relatve to executable location.
    appconf->paths.srcPath = "../../app/src";
    appconf->paths.shadersPath = "../../app/shaders";
    appconf->paths.fontsPath = "../../app/fonts";
    appconf->paths.corePath = "../../core";

    //Make window object belonging to the application, set up its attributes.
    Window window1 = Window();
    struct Window::configuration* win1conf = &window1.config;
    win1conf->windowName = "Window 1";
    win1conf->windowDesc = "Window 1 description";
    win1conf->running = true;
    win1conf->width = 1920;
    win1conf->height = 1080;
    // Add layers to window 1.
    BaseLayer base;
    base.config.framerate = 60; // layer logic framerate.
    base.config.threadGroup = 0;
    window1.addLayer(&base);
    SecondaryLayer second;
    second.config.framerate = 60; // layer logic framerate.
    second.config.threadGroup = 0;
    window1.addLayer(&second);


    Window window2 = Window();
    struct Window::configuration* win2conf = &window2.config;
    win2conf->windowName = "Window 2";
    win2conf->windowDesc = "Window 2 description";
    win2conf->running = true;
    // Others remain as default. Same as window 1 values.
    // Add layers to window 2
    window2.addLayer(&base);


    app.addWindow(window1); //Added window1 to thread group 0 for rendering.
    app.addWindow(window2);

    app.run();
}
