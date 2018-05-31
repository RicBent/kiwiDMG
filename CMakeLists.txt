cmake_minimum_required(VERSION 3.8)

project(kiwiDMG)

option(ENABLE_QT "Enable the Qt frontend" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (ENABLE_QT)
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
endif()

add_subdirectory(src)
