include(FetchContent)

# Also fetch dependencies here, to ensure up correct version.
FetchContent_Declare(
    general_utils
    GIT_REPOSITORY https://github.com/Will-Repo/General-Utilities.git
    GIT_TAG e59e06a3aeea8139dee09bc9cae7360ee69aa026
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(general_utils)  
target_link_libraries(app PUBLIC General-Utils)

# Also fetch dependencies here, to ensure up correct version.
FetchContent_Declare(
    opengl_utils
    GIT_REPOSITORY https://github.com/Will-Repo/OpenGL-Utilities.git
    GIT_TAG 3009c37c05dfc67d83fddc0d6d7cdb4fcece2235
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(opengl_utils)  
target_link_libraries(app PUBLIC OpenGL-Utils)
