cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)  
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Template)

include(cmake/dependencies.cmake)

add_subdirectory(core)

# Take user input of folder to add to project.
set(DIR "app" CACHE STRING "Project directory to build")

message(STATUS "Adding ${DIR} project")
add_subdirectory(${DIR}) # Links with core so must be done secondarily.
