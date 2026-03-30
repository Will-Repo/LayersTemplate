include(FetchContent)

# Also fetch dependencies here, to ensure up correct version.
if (LINK_GENERAL_UTILS)
    FetchContent_Declare(
    	general_utils
        GIT_REPOSITORY https://github.com/Will-Repo/General-Utilities.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(general_utils)  
    target_link_libraries(app PUBLIC General-Utils)
endif (LINK_GENERAL_UTILS)

# Also fetch dependencies here, to ensure up correct version.
if (LINK_OPENGL_UTILS)
    FetchContent_Declare(
    	opengl_utils
        GIT_REPOSITORY https://github.com/Will-Repo/OpenGL-Utilities.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(opengl_utils)  
    target_link_libraries(app PUBLIC OpenGL-Utils)
endif (LINK_OPENGL_UTILS)
