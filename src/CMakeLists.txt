include_directories(.)

add_subdirectory(core)

if (ENABLE_GLFW)
    add_subdirectory(glfw)
endif()
