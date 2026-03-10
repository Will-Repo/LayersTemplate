#include "Application.h"
#include "Window.h"

int main() {
    //Make application object, set up attributes.
    Application app = Application();
    struct Application::configuration* appconf = &app.config; //Or access directly.
    appconf->framerate = 120; //App logic framerate, not rendering.
    appconf->appName = "App";
    appconf->appDesc = "App description";

    //Make window object belonging to the application, set up its attributes.
    Window window1 = Window();
    struct Window::configuration* win1conf = &window1.config;
    win1conf->framerate = 60;
    win1conf->windowName = "Window 1";
    win1conf->windowDesc = "Window 1 description";
    win1conf->independentThread = false;
    win1conf->vsync = false;
    win1conf->running = true;

    Window window2 = Window();
    struct Window::configuration* win2conf = &window1.config;
    win2conf->windowName = "Window 2";
    win2conf->windowDesc = "Window 2 description";
    win2conf->running = true;
    // Others remain as default. Same as window 1 values.

    app.addWindow(window1);
    app.addWindow(window2);

    app.run();
}
