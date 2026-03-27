cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)  

project(Template)

# Optionally link the app with my OpenGL utilities library.
option(LINK_OPENGL_UTILS "Links the app executable with the opengl utilities library" OFF)

add_subdirectory(core)

add_subdirectory(app) # Links with core so must be done secondarily.

if (LINK_OPENGL_UTILS)
    include(FetchContent)
    FetchContent_Declare(
    	opengl_utils
        GIT_REPOSITORY https://github.com/Will-Repo/OpenGL-Utilities.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(opengl_utils)  
    target_link_libraries(app PUBLIC opengl_utils)
endif (LINK_OPENGL_UTILS)
