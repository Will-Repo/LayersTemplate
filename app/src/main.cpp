#include "Application.h"
#include "Window.h"

#include "example/BaseLayer.h"
#include "example/SecondaryLayer.h"

int main() {
    //Note: This is an example of what you can do, not what you should do (in terms of frame groups etc), this is dependent on the application being created.
    /* APPLICATION SETUP */
    //Make application object, set up attributes.
    Application app = Application();
    struct Application::configuration* appconf = &app.config; //Or access directly.
    appconf->mainThreadLimit = 120; // Limit the main number of executations of the main thread per second - main thread calls rendering functions (if framerate of that layer has been reached) and recieves inputs.
    appconf->recieveInputsLimit = -1; // Framerate for recieving inputs.
    appconf->renderingCallsLimit = -1; // Framerate for calling rendering functions (these have their own time checks so this is a maximum).
    appconf->appName = "App";
    appconf->appDesc = "App description";
    //appconf->vsync = false;
    appconf->running = true;
    //Set up data about file locations for the project - multiple reads are thread safe, don't combining reading and writing on multiple threads. Relatve to executable location.
    appconf->paths.srcPath = "../../app/src";
    appconf->paths.shadersPath = "../../app/shaders";
    appconf->paths.fontsPath = "../../app/fonts";
    appconf->paths.corePath = "../../core";

    /* WINDOW 1 SETUP */
    //Make window object belonging to the application, set up its attributes.
    Window window1 = Window();
    struct Window::configuration* win1conf = &window1.config;
    win1conf->windowName = "Window 1";
    win1conf->windowDesc = "Window 1 description";
    win1conf->running = true;
    win1conf->width = 1920;
    win1conf->height = 1080;
    win1conf->inputHandlingRate = 120;

    // Declare and add layers to window 1.
    BaseLayer base;
    base.config.updateFrameLimit = 60; // layer logic framerate.
    base.config.updateThreadGroup = 0; 
    base.config.renderingFrameLimit = 60; // layer logic framerate.
    base.config.renderingThreadGroup = 0;
    window1.addLayer(&base);

    SecondaryLayer second;
    second.config.updateFrameLimit = 120; // layer logic framerate.
    second.config.renderingFrameLimit = 120;
    second.config.renderingThreadGroup = 1;
    window1.addLayer(&second);

    /* WINDOW 2 SETUP */
    Window window2 = Window();
    struct Window::configuration* win2conf = &window2.config;
    win2conf->windowName = "Window 2";
    win2conf->windowDesc = "Window 2 description";
    win2conf->running = true;
    win2conf->inputHandlingRate = 30;
    // Others remain as default. Same as window 1 values.
    // Add layers to window 2
    window2.addLayer(&base);

    /* SETTING UP OBJECT RELATIONS AND RUNNING APP */
    app.addWindow(window1);  
    app.addWindow(window2);

    app.run();
}
