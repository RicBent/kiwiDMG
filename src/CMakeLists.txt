include_directories(.)

add_subdirectory(core)

if (ENABLE_QT)
    add_subdirectory(qt)
endif()
