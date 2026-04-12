#include "Application.h"
#include "Window.h"
#include "Layers/MainLayer.h"
#include "Layers/DebugLayer.h"
#include <memory>

/*
    Car simulation game, basic physics, circular arena, single goal.
    Second window (appears and disappears at hotkey press) showing statistics.
*/

int main() {
    //TODO: Make config values make sense - base doesnt even update lol, doesnt need 120 fps.
    //Note: This is an example of what you can do, not what you should do (in terms of frame groups etc), this is dependent on the application being created.
    
    /* APPLICATION SETUP */
    //Make application object, set up attributes.
    Application app = Application();
    struct Application::Configuration* appconf = &app.config; //Or access directly.
    appconf->mainThreadLimit = 120; // Limit the main number of executations of the main thread per second - main thread calls rendering functions (if framerate of that layer has been reached) and recieves inputs.
    appconf->recieveInputsLimit = -1; // Framerate for recieving inputs.
    appconf->renderingCallsLimit = -1; // Framerate for calling rendering functions (these have their own time checks so this is a maximum).
    appconf->appName = "Example";
    appconf->appDesc = "App showcasing basic app setup and optional utility functions.";
    //appconf->vsync = false;
    appconf->running = true;
    //Set up data about file locations for the project - multiple reads are thread safe, don't combining reading and writing on multiple threads. Relatve to executable location.
    appconf->paths.srcPath = "../../example/src";
    appconf->paths.shadersPath = "../../example/shaders";
    appconf->paths.fontsPath = "../../example/fonts";
    appconf->paths.assetsPath = "../../example/assets";
    appconf->paths.corePath = "../../core";

    /* WINDOW 1 SETUP */
    //Make window object belonging to the application, set up its attributes.
    auto window1 = std::make_shared<Window>();    
    struct Window::Configuration* win1conf = &window1->config;
    win1conf->windowName = "Game";
    win1conf->windowDesc = "Basic car game/simulation";
    win1conf->running = true;
    win1conf->open = true;
    win1conf->width = 1920;
    win1conf->height = 1080;
    win1conf->inputHandlingRate = 120;
    win1conf->inputHandlingGroup = 0;
    win1conf->renderingFrameLimit = 120;
    win1conf->renderingThreadGroup = 0; // Entirely seperate thread group than input handling groups.
                                        
    // Declare and add layers to window 1.
    auto main = std::make_shared<MainLayer>();
    main->config.updateFrameLimit = 120; // layer logic framerate.
    main->config.updateThreadGroup = 0; 
    main->config.renderingFrameLimit = 60; // layer logic framerate.
    window1->addLayer(std::move(main));

    auto debug = std::make_shared<DebugLayer>();
    debug->config.updateFrameLimit = 30; // layer logic framerate.
    debug->config.renderingFrameLimit = 30;
    window1->addLayer(std::move(debug));

    /* SETTING UP OBJECT RELATIONS AND RUNNING APP */
    // Adding window to app also gives the window a pointer to the application.
    app.addWindow(std::move(window1));  
//    app.addWindow(std::move(window2));

    app.run();
}
