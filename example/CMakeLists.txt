file(GLOB APP_SRC
    *.cpp
    src/*.cpp
    src/Layers/*.cpp
)

add_executable(app ${APP_SRC})

target_link_libraries(app PRIVATE 
    core # Utils are linked if enabled in dependencies.cmake
)

target_include_directories(app PRIVATE src)
