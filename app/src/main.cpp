#include "Application.h"
#include "Window.h"
#include <memory>

int main() {
    /* APPLICATION SETUP */
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
    appconf->paths.assetsPath = "../../app/assets";
    appconf->paths.corePath = "../../core";

    /* WINDOW 1 SETUP */
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
    /*
    auto base = std::make_shared<BaseLayer>();
    base->config.updateFrameLimit = 120; // layer logic framerate.
    base->config.updateThreadGroup = 0; 
    base->config.renderingFrameLimit = 60; // layer logic framerate.
    window1->addLayer(&base);
    */

    /* SETTING UP OBJECT RELATIONS AND RUNNING APP */
    app.addWindow(std::move(window1));  

    app.run();
}
