# LayersTemplate
C++ code template that provides a starting point for cross plateform C++ projects (linux & windows). Offers layers, window and application management, audio, and more (see readme). OpenGL based.

## Features

## Set-up, Dependencies & Build
To build, run 'cmake -S . -B build/ && cmake --build build' from the root of the project.
The app file must be run from its build location, in build/app for relative paths in shader setup to work correctly.

### User required dependencies:
*OpenGL - Minimum Version:

### CMake automatic dependencies:
CMake will fetch these dependencies if they are not already on your system at the minumum required level.
*GLM - Minimum Version: 1.0.3
*GLEW - Minimum Version: 2.3.1 (note, this is incompatible with FetchContent so is instead included as a subdirectory within the build folder).
*GLFW - Minimum Version: 3.4

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
            Application
            Window
            Layer
            Event
            AudioManager
            LogicThreadManager              # Class for managing thread allocations
            LogicThread                     # Thread that runs layer logic for app defined layers with same frame rate.
        -vendor
            -glew-2.3.1
            -freeglut-3.8.0
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

## Naming Conventions

## To-do
Update CMakeFile contents and structure after reading up on modern cmake.
Create test application (using imgui and basic rendering), add template as upstream remote and can fetch and merge changes as I update the template.
Ensure onUpdate function calaculates deltatime to ensure consistent frame times (actually, let user do this on an app-basis, instead just give access to timestep. Also: https://vodacek.zvb.cz/archiv/681.html). Perhaps not actually, perhaps this should be in the applicatons run method.
Change each window to be multithreaded, or let app control how threads are split.
Seperate application framerate and window framerate - check with OS how many available, join rendering if not. Min 2 threads needed, one for logic and one for rendering.
Have extra layers join main thread if no threads available? Might not be good idea, should probably just throw error.
