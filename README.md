# LayersTemplate
C++ code template that provides a starting point for cross plateform C++ projects (linux & windows). Offers layers, window and application management, audio, and more (see readme). OpenGL based.

## Features

## Set-up, Dependencies & Build
To build, run 'cmake -S . -B build/ && cmake --build build' from the root of the project.

### User required dependencies:
*OpenGL - Minimum Version:

### CMake automatic dependencies:
CMake will fetch these dependencies if they are not already on your system at the minumum required level.
*GLM - Minimum Version: 1.0.3

*GLEW - Minimum Version: 2.3.1 (note, this is incompatible with FetchContent so is instead included as a subdirectory within the build folder).
*freeglut- Minimum Version: 3.8.0 (same as above).

## File Structure
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
        -vendor
            -glew-2.3.1
            -freeglut-3.8.0
            stb_image.h

## Core abstraction
The app project is intended to interact with the core libarary primarily through the Application and Layers classes, with additional functions being provided by other utility classes (e.g. compiling shaders).
The application should:
*create an Application object
*modify the objects config struct
*create one or more Window objects
*modify the window objects' config structs
*add some layers to the Application object's layer stack, assigning each layer to a specific window
*run the application


## Naming Conventions

## To-do
Update CMakeFile contents and structure after reading up on modern cmake.
Create test application (using imgui and basic rendering), add template as upstream remote and can fetch and merge changes as I update the template.
Ensure onUpdate function calaculates deltatime to ensure consistent frame times (actually, let user do this on an app-basis, instead just give access to timestep. Also: https://vodacek.zvb.cz/archiv/681.html). Perhaps not actually, perhaps this should be in the applicatons run method.
Change each window to be multithreaded, or let app control how threads are split.
