cmake_minimum_required(VERSION 3.1.2)
set(CMAKE_CXX_STANDARD 17)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)  

project(Template)

add_subdirectory(core)
add_subdirectory(app) # Links with core so must be done secondarily.
