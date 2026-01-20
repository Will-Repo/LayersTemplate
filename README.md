# LayersTemplate
C++ code template that provides a starting point for cross plateform C++ projects (linux & windows). Offers layers, window and application management, audio, and more (see readme). OpenGL based.

## Features

## Set-up & Dependencies

## File Structure
Note: This does not include the header counterparts of most .cpp files.
```bash
LayersTemplate
-app
    -shaders
    -assets
    -layers
    -scripts
        release_build.bat
        debug_build.bat
    -build
    Main.cpp
    CMakeLists.txt                      # Requires core sub-project to be valid, controls whole project compilation.
    .gitignore
-core                                   # Encapsulates code necessary for apps implentating the template.
    -utility
    -extern
    -build
    Application.cpp
    Window.cpp
    Layer.cpp
    Event.cpp
    AudioManager.cpp
    CMakeLists.txt                      # For standalone compilation, and being built as part of an application.
    .gitignore
```

## Intended Application Structure
This is the intended structure for an application utilising this template.

## Naming Conventions

## To-do
Update CMakeFile contents and structure after reading up on modern cmake.

