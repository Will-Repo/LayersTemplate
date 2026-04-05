# LayersTemplate
C++ code template that provides a starting point for cross plateform C++ projects (linux & windows). Offers layers, window and application management, audio, and more (see readme). OpenGL based.

## Features
In development, will update at initial release.
Here is a list of features provided by my template:
* Basic rendering setup - automatically sets up OpenGL abstraction libraries and each window's OpenGL context.
* Basic audio setup - links OpenAL-soft library.
* Thread management

Optionally, the application can use functions provided by my OpenGL utilities library by using adding the following to the CMake build command: ```-DLINK_OPENGL_UTILS=ON```. This must be added before the path (..).
The complete list of features provided by this library can be found here: [OpenGL Utilities](https://github.com/Will-Repo/OpenGL-Utilities.git). To be clear, this doesn't provide any functionality not included in this repository, it just abstracts some commonly used features typical of OpenGL and OpenAL projects.
Here is a basic list of features provided by this optional extention:
* Common rendering abstractions - rendering quads, triangles, circles, 3d shapes, loading models, etc.
Additionally, I also provide an optional general utilities repository that provides the following features (this can be added with ```-DLINK_GENERAL_UTILS=ON```, in the same format as above)(the complete feature list can be found here: [General Utilities](https://github.com/Will-Repo/General-Utilities.git)):
* File reading.

## Set-up, Dependencies & Build
### General commands
To build, run ```cmake -S (relative path to root CMakeLists.txt) -D (path to app project to build) -B (relative path to build directory) && cmake --build (relative path to buid directory)``` from anywhere on your system.

### App project commands
To build the app project from the root directory, you can simply run ```cmake build/``` followed by ```cmake --build build```, assuming the build folder it a direct child of the root (as is default).
Note: If a different folder has been built previously, this will be built again, as this builds the default DIR value (app) only if DIR has not got a value cached. To build the app project, run ```cmake -D DIR=app -B build/``` instead, or delete CMakeCache.txt and rerun the original command.

The app executable (found at ```build/app/app```) can be run from anywhere, as file path behaviour is defined relative to the executable location (this can be configured in the app config).

### Example project commands 
To build the example project from the root directory, you can simply run ```cmake -D DIR=example -B build/``` followed by ```cmake --build build```, assuming the build folder it a direct child of the root (as is default).

### User required dependencies:
* **OpenGL** - Minimum Version: 4.3

### CMake automatic dependencies:
CMake will fetch these dependencies if they are not already on your system at the minumum required level.
* **GLM** - Minimum Version: 1.0.3
* **GLEW** - Minimum Version: 2.3.1
* **GLFW** - Minimum Version: 3.4
* **OpenAL-Soft** - Minimum Version: 1.25.1
* **FreeType** - Minimum Version: 2.14.2

### Dependency search locations
CMake will search the default [locations](https://cmake.org/cmake/help/latest/command/find_package.html#search-procedure) for config package files. It will also search the build/_deps folder under root, allowing offline builds without installing all packages to the system (if dependencies have previously been installed).

## Example Project
This project is included with the default template.
It consists of two windwos:
* A simple car sandbox arena with basic physics.
* A project introduction page, features a button that can be used to alter shader or object.

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

## Framerate
The core project handles the framerate as follows:
* **Each windows' logic** - defined on a per layer basis, layers can choose to do this logic on a seperate thread (other layers can choose to join this thread group too). The application can cap this framerate.
* **Each windows' rendering** - rendering happens on the window specific threads (or window groups), but layers can specify what framerate they want to render at.
    ~~*Currently only the main thread can render, due to OpenGL only allowing one context to be current at once (app-wide), I might look into this more at a later date.*~~
* **Input events** - these are recieved by polling on the main thread, at a framerate defined in the app config (can be uncapped), but seperate from other app logic. Each window defines a frame rate for its events to be passed to its layers. it is up to the implementation of each layer to decide whether it delays the effect of this event until onUpdate is called, or changes are applied immediately.

Note: Limiting framerate values can reduce system usage.

## Events
Events are polled on the main thread. 
Each window has a default set of callbacks, that pass event objects to the InputThread which holds the window where the event happened. The InputThread handles the event by dispatching it down the layer stack until it has been handled.

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
* Fix memory leak [ I believe this to be fixed, but haven't yet verified ].
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
* Add type enum to base event class, allowing using switch to easily get and cast to correct event type.
* Add screenshots to example project section, and clean up text.
* Add ninja build option (faster compile times).
* Add option to link with seperate github repo that provides utility rendering functions - e.g. drawing quads, drawing textures, etc.
* Review and fix cmake public and private visibility mismatches.
* Change to using modern cmake methods for adding include directories - propogation from the library.
* Check and update documentation - specifically build commands, and remove duplicate and complete to-do tasks.
* Make general commands section make sense with updated system.

## Bugs and known issues

## License
This project is licensed under the MIT license.

## Third-Party Licenses
This project uses some dependencies (named above) under permissive licences. Thank you to the authors and maintainers of these projects!
License requirements are included in LICENSE.
