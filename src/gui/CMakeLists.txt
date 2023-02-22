cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(TME_GUI_WIN_STATIC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
        set(CMAKE_EXE_LINKER_FLAGS "-Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

endif()

find_package(OpenSSL REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        settingswindow.cpp
        settingswindow.h
        settingswindow.ui
        triggers/title_bar.cpp
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

if(TME_GUI_WIN_STATIC)
        target_link_libraries( TME PRIVATE PARSER easyloggingpp sqlite3 TuringMachine Qt5::Widgets Qt5::Network D:/utils/openssl_x64_static/lib/libssl.a D:/utils/openssl_x64_static/lib/libcrypto.a)
else()
        target_link_libraries( TME PRIVATE PARSER easyloggingpp sqlite3 TuringMachine  Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network ${CMAKE_DL_LIBS} pthread)
endif()