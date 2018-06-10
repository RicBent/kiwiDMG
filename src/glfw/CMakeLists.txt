add_executable(kiwiDMG-glfw
        main.cpp
        glad.c
        glad.h
        tinyfiledialogs.c
        tinyfiledialogs.h
        emuwindow.cpp
        emuwindow.h)

target_link_libraries(kiwiDMG-glfw PRIVATE core)
target_link_libraries(kiwiDMG-glfw PRIVATE glfw)
