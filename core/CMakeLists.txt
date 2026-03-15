include(Dependencies.cmake)

file(GLOB CORE_SRC
    src/*.cpp
    src/utility/*.cpp
)

add_library(core STATIC ${CORE_SRC})

target_link_libraries(core PUBLIC 
    glm glew_s glfw OpenAL freetype
)

target_include_directories(core PUBLIC 
    src src/utility vendor ${GLMW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${OPENALSOFT_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR}
)
