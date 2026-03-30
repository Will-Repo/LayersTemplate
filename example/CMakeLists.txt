file(GLOB APP_SRC
    *.cpp
    src/*.cpp

    src/example/*.cpp
)

add_executable(app ${APP_SRC})


# Optionally link the app with my OpenGL and general utilities libraries.
option(LINK_OPENGL_UTILS "Links the app executable with the opengl utilities library" OFF)
option(LINK_GENERAL_UTILS "Links the app executable with the general utilities library" OFF)

include(Dependencies.cmake)

target_link_libraries(app PUBLIC 
    core # Utils are linked if enabled in dependencies.cmake
)

target_include_directories(app PRIVATE src)
