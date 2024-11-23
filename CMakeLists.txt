cmake_minimum_required(VERSION 3.29)
project(fbbrowser LANGUAGES CXX)

file(GLOB SRC_FILES *.c *.cpp *.qrc)

set(CMAKE_PREFIX_PATH /opt/qt6.8/6.8.0/gcc_64)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui WebEngineWidgets)
add_definitions (-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
)


add_executable(fbbrowser
    ${PROJECT_SOURCES}
)

target_link_libraries(fbbrowser PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::WebEngineWidgets
)

