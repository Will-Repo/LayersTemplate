#include "Application.h"
#inlcude "Window.h"

int main() {
    //Make application object, set up attributes.
    Application app = new Application();
    app.config = {};

    //Make window object belonging to the application, set up its attributes.
    Window window = new Window();
    window.config = {};

    app.addWindow(window);
}
