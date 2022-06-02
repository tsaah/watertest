cmake_minimum_required(VERSION 3.22)

project(watertest)

set(CMAKE_CXX_STANDARD 20)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

include(cmake/glad.cmake)
include(cmake/glfw.cmake)
include(cmake/glm.cmake)
include(cmake/imgui.cmake)
include(cmake/stb.cmake)

add_subdirectory(src)

