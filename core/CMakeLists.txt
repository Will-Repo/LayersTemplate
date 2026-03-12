include(Dependencies.cmake)

file(GLOB CORE_SRC
    src/*.cpp
    src/utility/*.cpp
)

add_library(core STATIC ${CORE_SRC})

target_link_libraries(core PUBLIC 
    glm glew glfw
)

target_include_directories(core PUBLIC 
    src vendor
)
