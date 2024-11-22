cmake_minimum_required(VERSION 3.30)

file(GLOB SRC_FILES *.c *.cpp *.qrc)

set(KF5_MIN_VERSION, "5.50.0")

IF(POLICY CMP0048)
    CMAKE_POLICY(SET CMP0048 NEW)
ENDIF(POLICY CMP0048)

project(FBrowser LANGUAGES CXX)
project(FBrowser VERSION "1.0.0")

IF(POLICY CMP0063)
    CMAKE_POLICY(SET CMP0063 NEW)
ENDIF(POLICY CMP0063)

IF(POLICY CMP0160)
    CMAKE_POLICY(SET CMP0160 OLD)
ENDIF(POLICY CMP0160)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(ECM 5.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

set(FULLBROWSER OFF CACHE BOOL "Full Featured Browser Support")
configure_file(config.h.in config.h)

find_package(Qt5 REQUIRED COMPONENTS Core Gui WebEngineWidgets QuickWidgets)
find_package(KF5I18n ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n)

find_package(KF5Plasma ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5DBusAddons ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} REQUIRED) # FIXME look for "KIOWidgets" (KRun) explicitly

add_definitions (-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

# add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)
# add_definitions(-DQT_NO_URL_CAST_FROM_STRING -DQT_USE_QSTRINGBUILDER -DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII)

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
    Qt5::Core
    Qt5::Gui
    Qt5::WebEngineWidgets
    Qt5::QuickWidgets
    KF5I18n
    KF5Plasma
    KF5DBusAddons
)
