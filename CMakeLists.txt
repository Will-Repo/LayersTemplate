cmake_minimum_required(VERSION 4.1.1) # May change later, probably don't need such an update version.

project(Template)

include(Dependencies.cmake)

add_subdirectory(app)
add_subdirectory(core)
