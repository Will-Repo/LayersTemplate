# LayersTemplate
C++ code template that provides a starting point for cross plateform C++ projects (linux & windows). Offers layers, window and application management, audio, and more (see readme). OpenGL based.

## Features

## Set-up, Dependencies & Build
To build, run 'cmake -S . -B build/ && cmake --build build' from the root of the project.
The app file must be run from the build directory root, for relative paths in shader setup to work correctly (this will be configurable in later versions).

### User required dependencies:
*OpenGL - Minimum Version: 4.3

### CMake automatic dependencies:
CMake will fetch these dependencies if they are not already on your system at the minumum required level.
*GLM - Minimum Version: 1.0.3
*GLEW - Minimum Version: 2.3.1 (note, this is incompatible with FetchContent so is instead included as a subdirectory within the build folder).
*GLFW - Minimum Version: 3.4
*OpenAL-Soft - Minimum Version: 1.25.1
*FreeType - Minimum Version: 2.14.2

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
*create an Application object
*modify the objects config struct
*create one or more Window objects
*modify the window objects' config structs
*create one or more layer objects
*modify the layer objects' config structrs
*add the layers to each Window object's layer stack
*run the application

## Multi-threading
All windows render on the same thread, though the app project can implement other OpenGL setup on seperate threads.
The app can get updates for different layers on different threads, this allows for seperate frame amounts and higher performance.

~~The following classes require a seperate instance per thread, and have data stored in the thread class:
- TextRendering, freetype face objects can only be used from a single thread at a time, so duplicate objects are required.
Use of these objects should be done through querying the thread manager using the thread group in the layers config, and the name of the class required.~~ This is not necessary as rendering is currently on one thread.

## Naming Conventions

## To-do
Update CMakeFile contents and structure after reading up on modern cmake.
Create test application (using imgui and basic rendering), add template as upstream remote and can fetch and merge changes as I update the template.
Ensure onUpdate function calaculates deltatime to ensure consistent frame times (actually, let user do this on an app-basis, instead just give access to timestep. Also: https://vodacek.zvb.cz/archiv/681.html). Perhaps not actually, perhaps this should be in the applicatons run method.
Change each window to be multithreaded, or let app control how threads are split.
Seperate application framerate and window framerate - check with OS how many available, join rendering if not. Min 2 threads needed, one for logic and one for rendering.
Have extra layers join main thread if no threads available? Might not be good idea, should probably just throw error.
Add relative paths config struct accessible to app.
Add text rendering support.
Check CMake, if user has older version installed it may not register this and cause issues.
Update base classes based on how they are used in the app - e.g. add data loading to base layer class.
