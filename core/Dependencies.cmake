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
message(STATUS "Set up glm")

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
message(STATUS "Set up GLEW")

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
message(STATUS "Set up GLFW")

# OpenAL-Soft - Version 1.21.5
find_package(OpenAL 1.25.1 QUIET) # Can't really differentiate between OpenAL and OpenAL soft, both use same target name.
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
message(STATUS "Set up OpenAL")

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
message(STATUS "Set up FreeType")

# whereami - latest - gets exe file location.
FetchContent_Declare(
    whereami
	GIT_REPOSITORY	https://github.com/gpakosz/whereami.git
    GIT_TAG 	dcb52a058dc14530ba9ae05e4339bd3ddfae0e0e
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(whereami)
set(WHEREAMI_INCLUDE_DIR ${whereami_SOURCE_DIR}/src)
#whereami doesn't define its cmake target, so must do manually, but as its a single file library, i will just add it as a source for the core library/
message(STATUS "Set up whereami")

# General utils
FetchContent_Declare(
    general_utils
    GIT_REPOSITORY https://github.com/Will-Repo/General-Utilities.git
    # GIT_TAG main
    GIT_TAG 59c8af11e04b4f1a6feaeaf642119f134f2af897
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(general_utils)  
set(GENERAL_UTILS_INCLUDE_DIR ${general_utils_SOURCE_DIR}/src)
message(STATUS "Set up general_utils")

# OpenGL utils
FetchContent_Declare(
    opengl_utils
    GIT_REPOSITORY https://github.com/Will-Repo/OpenGL-Utilities.git
    GIT_TAG 70a93e5ba3d97a8cfdc8fb6fb4c70f64560e572d
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(opengl_utils)  
set(OPENGL_UTILS_INCLUDE_DIR ${opengl_utils_SOURCE_DIR}/src)
message(STATUS "Set up opengl_utils")
