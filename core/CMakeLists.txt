include(Dependencies.cmake)

file(GLOB CORE_SRC
    src/*.cpp
)

add_library(core STATIC ${CORE_SRC})

target_link_libraries(core PUBLIC 
    glfw glew freeglut
)

target_include_directories(core PUBLIC src vendor)
