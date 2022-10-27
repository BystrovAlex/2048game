cmake_minimum_required(VERSION 3.16)

project(2048game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(2048game
    main.cpp
    widget.cpp
    widget.ui
)

target_link_libraries(2048game PRIVATE Qt6::Widgets)

set_target_properties(2048game PROPERTIES
    WIN32_EXECUTABLE ON
)
