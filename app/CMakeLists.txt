file(GLOB APP_SRC
    *.cpp
    src/*.cpp

    src/example/*.cpp
)

add_executable(app ${APP_SRC})

target_link_libraries(app core)

target_include_directories(app PRIVATE src)
