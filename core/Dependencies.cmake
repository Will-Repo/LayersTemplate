# These are set up above both subdirectories as both projects require these libraries to function.
include(FetchContent)

# OpenGL
find_package(OpenGL REQUIRED)

# GLM - Version 1.0.3
find_package(glm 1.0.3 QUIET) # Module modea
if (NOT glm_FOUND)
    FetchContent_Declare(
    	glm
    	GIT_REPOSITORY	https://github.com/g-truc/glm.git
    	GIT_TAG 	8d1fd52e5ab5590e2c81768ace50c72bae28f2ed
    )
    # Get saved content detauls from the above declaration.
    #FetchContent_GetProperties(glm)
    #if (NOT glm_POPULATED)
    #    FetchContent_Populate(glm)
    #    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
    #endif()a
    FetchContent_MakeAvailable(glm)
endif()
set_target_properties(glm PROPERTIES FOLDER "Dependencies") # Only affects IDE folder placement, not actual file location, so not really necessary for me.

# GLEW - Version 2.3.1
add_subdirectory(vendor/glew-2.3.1/build/cmake)

# Freeglut - Version 3.8.0
#add_subdirectory(vendor/freeglut-3.8.0)

# GLFW - Version 3.4
find_package(glfw 3.4 QUIET) # Module modea
if (NOT glfw_FOUND)
    FetchContent_Declare(
    	glfw
    	GIT_REPOSITORY	https://github.com/glfw/glfw.git
        GIT_TAG 	7b6aead9fb88b3623e3b3725ebb42670cbe4c579
    )
    # Get saved content detauls from the above declaration.
    #FetchContent_GetProperties(glm)
    #if (NOT glm_POPULATED)
    #    FetchContent_Populate(glm)
    #    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
    #endif()a
    FetchContent_MakeAvailable(glfw)
endif()
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
