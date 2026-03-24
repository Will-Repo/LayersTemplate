# LayersTemplate
C++ code template that provides a starting point for cross plateform C++ projects (linux & windows). Offers layers, window and application management, audio, and more (see readme). OpenGL based.

## Features
In development, will update at initial release.

## Set-up, Dependencies & Build
To build, run ```cmake -S . -B build/``` && ```cmake --build build``` from the root of the project.
By default, the app file must be run from the build directory root, for relative paths in shader setup to work correctly. The app can modify the default file paths to change this behaviour.

### User required dependencies:
* **OpenGL** - Minimum Version: 4.3

### CMake automatic dependencies:
CMake will fetch these dependencies if they are not already on your system at the minumum required level.
* **GLM** - Minimum Version: 1.0.3
* **GLEW** - Minimum Version: 2.3.1 (note, this is incompatible with FetchContent so is instead included as a subdirectory within the build folder).
* **GLFW** - Minimum Version: 3.4
* **OpenAL-Soft** - Minimum Version: 1.25.1
* **FreeType** - Minimum Version: 2.14.2

## File Structure - UPDATE THIS
```bash
LayersTemplate
    CMakeLists.txt                          # File that manages project wide dependencies - mostly OpenGL - and combines app and core (two subdirectories).
    .gitignore
    -build
    -app
        -shaders
        -assets
            -audio
            -textures
        -layers
        main
        CMakeLists.txt                      # Requires core sub-project to be valid, controls whole project compilation.
        .gitignore
    -core                                   # Seperate project that encapsulates code necessary for apps implentating the template.
        Depencencies.cmake                      # Fetches and compiles external dependencies.
        CMakeLists.txt                      # For standalone compilation, and being built as part of an application.
        .gitignore
        -src
            -utility
                shaderLoader
                TextRenderer
            Application
            Window
            Layer
            Event
            LogicThreadManager              # Class for managing thread allocations
            LogicThread                     # Thread that runs layer logic for app defined layers with same frame rate.
        -vendor
            stb_image.h
```
## Core abstraction
The app project is intended to interact with the core libarary primarily through the Application and Layers classes, with additional functions being provided by other utility classes (e.g. compiling shaders).
The application should:
* create an Application object
* modify the objects config struct
* create one or more Window objects
* modify the window objects' config structs
* create one or more layer objects
* modify the layer objects' config structrs
* add the layers to each Window object's layer stack
* run the application

## Multi-threading
* **Windows** can **render** on seperate threads, or join with others in a frame group. All rendering for each window will be done within its thread group, as each OpenGL context will only be made current on a single thread.
* **Events** are recieved on the main thread, but passed to window specific threads to be managed. Again, each window can have its own thread, or join with others in a frame group.
* **Layers** can **update** on seperate threads, or join with others in a thread group.

Thread groups can handle processes with different frame rates perfectly fine.
Each layer has its own framebuffer (rendered to texture, which are rendered sequentially), so each pass can have some windows not render anything and still retain their previous state.

~~The following classes require a seperate instance per thread, and have data stored in the thread class:~~
~~- TextRendering, freetype face objects can only be used from a single thread at a time, so duplicate objects are required.~~
~~Use of these objects should be done through querying the thread manager using the thread group in the layers config, and the name of the class required.~~ This is not necessary as rendering is currently on one thread.

## Framerate
The core project handles the framerate as follows:
* **Each windows' logic** - defined on a per layer basis, layers can choose to do this logic on a seperate thread (other layers can choose to join this thread group too). The application can cap this framerate.
* **Each windows' rendering** - rendering happens on the window specific threads (or window groups), but layers can specify what framerate they want to render at.
    ~~*Currently only the main thread can render, due to OpenGL only allowing one context to be current at once (app-wide), I might look into this more at a later date.*~~
* **Input events** - these are recieved by polling on the main thread, at a framerate defined in the app config (can be uncapped), but seperate from other app logic. Each window defines a frame rate for its events to be passed to its layers. it is up to the implementation of each layer to decide whether it delays the effect of this event until onUpdate is called, or changes are applied immediately.

Note: Limiting framerate values can reduce system usage.

## Naming Conventions

## To-do
* Update CMakeFile contents and structure after reading up on modern cmake.
* Create test application (using imgui and basic rendering), add template as upstream remote and can fetch and merge changes as I update the template.
* Ensure onUpdate function calaculates deltatime to ensure consistent frame times (actually, let user do this on an app-basis, instead just give access to timestep. Also: https://vodacek.zvb.cz/archiv/681.html). Perhaps not actually, perhaps this should be in the applicatons run method.
* Change each window to be multithreaded, or let app control how threads are split.
* Seperate application framerate and window framerate - check with OS how many available, join rendering if not. Min 2 threads needed, one for logic and one for rendering.
* Have extra layers join main thread if no threads available? Might not be good idea, should probably just throw error.
* Add relative paths config struct accessible to app.
* Add text rendering support.
* Check CMake, if user has older version installed it may not register this and cause issues.
* Update base classes based on how they are used in the app - e.g. add data loading to base layer class.
* Make consistent whether i use getter functions or public - think more on specific instances what is appropriate.
* Add opengl debugging.
* Rewrite text renderering myself, moving more into initialisation rather than current inefficient implementation.
* Move unecessary member functions out of their classes.
* Set up release guidelines - e.g. tag naming scheme.
* Stop working on main thread after 1st release.
* Clean up getExecutablePath() in FilePaths.cpp
* Store pointers to filePaths in class, rather than passing it in always.
* Fix memory leak.
* Need better framerate title name.
* Add warning when assigning layer to thread where thread limit is lower than layers value (e.g. when a layer's rendering framerate is higher than its window's).
* The update thread has no limit - so uses much more processing power than necessary - add max limit, perhaps in app config, or on a thread group basis - new config?
* Multiple references to same layer across windows? Surely not great idea, but perhaps useful and should be accounted for. Add to docs, updating where multiple windows reference one layer can have issues - take into account.
* How can layers be deleted, all references going out of scope? But then if multiple one will keep updating even after closed.
* Change Inputs to Events in InputThread - as it handles events, not just user inputs.
* Need way to close windows from the proper thread for it - or perhaps its fine on the main thread?
* -1 means no limit for framerate, but application and threads dont take this into account yet.
* Make framebuffer texture size same as window automatically. Currently using fixed values (1920x1080).
* Currently can only have one window per thread as only one context makes the quad for rendering textures, fix this.
* Make sure window open and running booleans functions correctly - if not running, inputs, updates and rendering should be paused, but window should still be shown.
* There is currently no way to copy layers to other windows, and reusing pointers will result in rendering issues, perhaps add deep copy of layers, for use in main.cpp when initialising windows.
* Currently no proper exit method (as threads still running, just calls exit(1)).

## Bugs and known issues

## License
This project is licensed under the MIT license.

## Third-Party Licenses
This project uses some dependencies (named above) under permissive licences. Thank you to the authors and maintainers of these projects!
License requirements are included in LICENSE.
