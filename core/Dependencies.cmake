# These are set up above both subdirectories as both projects require these libraries to function.
include(FetchContent)

# OpenGL
find_package(OpenGL 4.3 REQUIRED)

# GLM - Version 1.0.3
find_package(glm 1.0.3 QUIET) # Module mode
if (NOT glm_FOUND)
    FetchContent_Declare(
    	glm
    	GIT_REPOSITORY	https://github.com/g-truc/glm.git
    	GIT_TAG 	8d1fd52e5ab5590e2c81768ace50c72bae28f2ed
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glm)
    set(GLM_INCLUDE_DIR ${glm_SOURCE_DIR}/include)
endif()
set_target_properties(glm PROPERTIES FOLDER "Dependencies") # Only affects IDE folder placement, not actual file location, so not really necessary for me.

# GLEW - Version 2.3.1
#add_subdirectory(vendor/glew-2.3.1/build/cmake)
find_package(GLEW 2.3.1 QUIET) # Module mode
if (NOT GLEW_FOUND)
    FetchContent_Declare(
    	glew
        #GIT_REPOSITORY	https://github.com/nigels-com/glew.git
        #GIT_TAG 	glew-2.3.1
        URL https://github.com/nigels-com/glew/releases/download/glew-2.3.1/glew-2.3.1.tgz
        SOURCE_SUBDIR  build/cmake
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glew)  
    set(GLEW_INCLUDE_DIR ${glew_SOURCE_DIR}/include)
endif()
set_target_properties(glew_s PROPERTIES FOLDER "Dependencies")

# Freeglut - Version 3.8.0
#add_subdirectory(vendor/freeglut-3.8.0)

# GLFW - Version 3.4
find_package(glfw 3.4 QUIET) # Module mode
if (NOT glfw_FOUND)
    FetchContent_Declare(
    	glfw
    	GIT_REPOSITORY	https://github.com/glfw/glfw.git
        GIT_TAG 	7b6aead9fb88b3623e3b3725ebb42670cbe4c579
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw)
    set(GLFW_INCLUDE_DIR ${glfw_SOURCE_DIR}/include)
endif()
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")

# OpenAL-Soft - Version 1.21.5
#find_package(OpenAL-Soft 1.25.1 QUIET) # Can't really differentiate between OpenAL and OpenAL soft, both use same target name.
if (NOT OpenAL-Soft_FOUND)
    FetchContent_Declare(
        OpenAL-Soft
    	GIT_REPOSITORY	https://github.com/kcat/openal-soft.git
        GIT_TAG 	c41d64c6a35f6174bf4a27010aeac52a8d3bb2c6
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(OpenAL-Soft)
    set(FREETYPE_INCLUDE_DIR ${OpenAL-Soft_SOURCE_DIR}/include)
endif()
set_target_properties(OpenAL PROPERTIES FOLDER "Dependencies")

# FreeType - Version 2.14.1
find_package(FreeType 2.14.1 QUIET)
if (NOT FreeType_FOUND)
    FetchContent_Declare(
        FreeType
    	GIT_REPOSITORY	https://gitlab.com/freetype/freetype.git
        GIT_TAG 	f4205da14867c5387cd6a329b90ee10a6df6eeff
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(FreeType)
    set(FREETYPE_INCLUDE_DIR ${FreeType_SOURCE_DIR}/include)
endif()
set_target_properties(freetype PROPERTIES FOLDER "Dependencies")
