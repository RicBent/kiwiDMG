set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(kiwiDMG-qt
    main.cpp
)

target_link_libraries(kiwiDMG-qt PRIVATE core)
target_link_libraries(kiwiDMG-qt PRIVATE Qt5::Widgets)
