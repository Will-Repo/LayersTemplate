file(GLOB APP_SRC
    *.cpp
    src/*.cpp
    src/Layers/*.cpp
)

add_executable(app ${APP_SRC})

include(Dependencies.cmake)

target_link_libraries(app PUBLIC 
    core # Utils are linked if enabled in dependencies.cmake
)

target_include_directories(app PRIVATE src)
