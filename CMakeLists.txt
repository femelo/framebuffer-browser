cmake_minimum_required(VERSION 3.5)
project(FBrowser LANGUAGES CXX)

file(GLOB source_files *.c *.cpp *.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(FULLBROWSER OFF CACHE BOOL "Full Featured Browser Support")
configure_file(config.h.in config.h)


find_package(Qt5 REQUIRED COMPONENTS Core Gui WebEngineWidgets)
add_definitions (-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

if (FULLBROWSER)
set(PROJECT_SOURCES
    browser.cpp browser.h
    browserwindow.cpp browserwindow.h
    certificateerrordialog.ui
    downloadmanagerwidget.cpp downloadmanagerwidget.h downloadmanagerwidget.ui
    downloadwidget.cpp downloadwidget.h downloadwidget.ui
    main.cpp
    passworddialog.ui
    tabwidget.cpp tabwidget.h
    webpage.cpp webpage.h
    webpopupwindow.cpp webpopupwindow.h
    webview.cpp webview.h
    data/simplebrowser.qrc
)
else()
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)
endif()


add_executable(FBrowser
    ${PROJECT_SOURCES}
)

target_link_libraries(FBrowser PUBLIC
    Qt::Core
    Qt::Gui
    Qt::WebEngineWidgets
)
