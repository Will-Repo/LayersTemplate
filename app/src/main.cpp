#include "Application.h"
#include "Window.h"

#include "BaseLayer.h"

int main() {
    //Make application object, set up attributes.
    Application app = Application();
    struct Application::configuration* appconf = &app.config; //Or access directly.
    appconf->framerate = 120; // Rendering framerate.
    appconf->appName = "App";
    appconf->appDesc = "App description";
    appconf->vsync = false;
    appconf->running = true;

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

    Window window2 = Window();
    struct Window::configuration* win2conf = &window1.config;
    win2conf->windowName = "Window 2";
    win2conf->windowDesc = "Window 2 description";
    win2conf->running = true;
    // Others remain as default. Same as window 1 values.
    // Add layers to window 2


    app.addWindow(window1); //Added window1 to thread group 0 for rendering.
    //app.addWindow(window2);

    app.run();
}
