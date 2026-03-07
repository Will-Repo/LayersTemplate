# LayersTemplate
C++ code template that provides a starting point for cross plateform C++ projects (linux & windows). Offers layers, window and application management, audio, and more (see readme). OpenGL based.

## Features

## Set-up, Dependencies & Build
To build, run 'cmake -S . -B build/ && cmake --build build' from the root of the project.

### User required dependencies:

### CMake automatic dependencies:
OpenGL - required for sucessful compilation

## File Structure
```bash
LayersTemplate
    CMakeLists.txt                          # File that manages project wide dependencies - mostly OpenGL - and combines app and core (two subdirectories).
    Depencencies.cmake                      # Fetches and compiles external dependencies.
    -build
    -app
        -shaders
        -assets
            -audio
            -textures
        -layers
        Main
        CMakeLists.txt                      # Requires core sub-project to be valid, controls whole project compilation.
        .gitignore
    -core                                   # Seperate project that encapsulates code necessary for apps implentating the template.
        -src
            -utility
            -extern
            -build
            Application
            Window
            Layer
            Event
            AudioManager
            CMakeLists.txt                      # For standalone compilation, and being built as part of an application.
            .gitignore
        -vendor
            stb_image.h

## Intended Application Structure
This is the intended structure of an application utilising this template.

## Naming Conventions

## To-do
Update CMakeFile contents and structure after reading up on modern cmake.

