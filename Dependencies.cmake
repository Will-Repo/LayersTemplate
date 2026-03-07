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
    FetchContent_GetProperties(glm)
    if (NOT glm_POPULATED)
        FetchContent_Populate(glm)
        add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
    endif()
endif()
set_target_properties(glm PROPERTIES FOLDER "Dependencies") # Only affects IDE folder placement, not actual file location, so not really necessary for me.

# GLEW - Version 2.3.1
find_package(glew 2.3.1 QUIET) # Config mode
if (NOT glew_FOUND)
    FetchContent_Declare(
    	glew
    	GIT_REPOSITORY	 https://github.com/nigels-com/glew
        GIT_TAG 	285c35d395979771f9409710b2d401db8026e15a
    )
    # Get saved content detauls from the above declaration.
    FetchContent_GetProperties(glew)
    if (NOT glew_POPULATED)
        FetchContent_Populate(glew)
        add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR})
    endif()
endif()
set_target_properties(glew PROPERTIES FOLDER "Dependencies")

# Freeglut - Version 3.8.0
find_package(FreeGLUT 3.8.0 QUIET) # Config mode
if (NOT FreeGLUT_FOUND)
    FetchContent_Declare(
    	FreeGLUT
    	GIT_REPOSITORY	https://github.com/freeglut/freeglut
    	GIT_TAG 	3db1649ce1f5e42f1338b51e3fa14849be547d5d
    )
    # Get saved content detauls from the above declaration.
    FetchContent_GetProperties(FreeGLUT)
    if (NOT FreeGLUT_POPULATED)
        FetchContent_Populate(FreeGLUT)
        add_subdirectory(${FreeGLUT_SOURCE_DIR} ${FreeGLUT_BINARY_DIR})
    endif()
endif()
set_target_properties(FreeGLUT PROPERTIES FOLDER "Dependencies")
