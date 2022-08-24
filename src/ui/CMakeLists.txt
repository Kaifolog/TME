cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
qt_add_executable(TME
        ${PROJECT_SOURCES}
)
else()
add_executable(TME WIN32
        ${PROJECT_SOURCES}
)
endif()

target_link_libraries( TME PRIVATE APP PARSER LIBS VENDOR TuringMachine  Qt${QT_VERSION_MAJOR}::Widgets ${CMAKE_DL_LIBS} pthread )