#include "Application.h"
#include "Window.h"
#include "BaseLayer.h"
#include "SecondaryLayer.h"
#include <memory>

int main() {
    //TODO: Make config values make sense - base doesnt even update lol, doesnt need 120 fps.
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
    appconf->paths.srcPath = "../../example/src";
    appconf->paths.shadersPath = "../../example/shaders";
    appconf->paths.fontsPath = "../../example/fonts";
    appconf->paths.corePath = "../../core";

    /* WINDOW 1 SETUP */
    //Make window object belonging to the application, set up its attributes.
    auto window1 = std::make_shared<Window>();    
    struct Window::configuration* win1conf = &window1->config;
    win1conf->windowName = "Window 1";
    win1conf->windowDesc = "Window 1 description";
    win1conf->running = true;
    win1conf->open = true;
    win1conf->width = 1920;
    win1conf->height = 1080;
    win1conf->inputHandlingRate = 120;
    win1conf->inputHandlingGroup = 0;
    win1conf->renderingFrameLimit = 120;
    win1conf->renderingThreadGroup = 0;

    // Declare and add layers to window 1.
    auto base = std::make_shared<BaseLayer>();
    base->config.updateFrameLimit = 120; // layer logic framerate.
    base->config.updateThreadGroup = 0; 
    base->config.renderingFrameLimit = 60; // layer logic framerate.
    //window1->addLayer(&base); //TODO: Make layers copyable and add to both window 1 and 2, just need to copy config.

    auto second = std::make_shared<SecondaryLayer>();
    second->config.updateFrameLimit = 30; // layer logic framerate.
    second->config.renderingFrameLimit = 30;
    window1->addLayer(std::move(second));

    /* WINDOW 2 SETUP */
    auto window2 = std::make_shared<Window>();    
    struct Window::configuration* win2conf = &window2->config;
    win2conf->windowName = "Window 2";
    win2conf->windowDesc = "Window 2 description";
    win2conf->running = true;
    win2conf->inputHandlingRate = 30;
    win2conf->inputHandlingGroup = 1;
    win2conf->renderingFrameLimit = 60;
    win2conf->renderingThreadGroup = 1;

    // Others remain as default. Same as window 1 values.
    // Add layers to window 2
    window2->addLayer(std::move(base));

    /* SETTING UP OBJECT RELATIONS AND RUNNING APP */
    app.addWindow(std::move(window1));  
    app.addWindow(std::move(window2));

    app.run();
}
