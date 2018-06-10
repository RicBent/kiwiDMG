cmake_minimum_required(VERSION 3.8)

project(kiwiDMG)

option(ENABLE_GLFW "Enable the GLFW frontend" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_GLFW)
    find_package(glfw3 3.2 REQUIRED)
endif()

add_subdirectory(src)
