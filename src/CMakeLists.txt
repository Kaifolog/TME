if(IS_GUI)

        set(CMAKE_INCLUDE_CURRENT_DIR ON)

        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)

        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include_directories(${CMAKE_SOURCE_DIR}/include)
#link_directories(${CMAKE_SOURCE_DIR}/lib)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

        find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

        set(PROJECT_SOURCES
                ui/main.cpp
                ui/mainwindow.cpp
                ui/mainwindow.h
                ui/mainwindow.ui
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

        add_subdirectory(tme)

        target_link_libraries( TME PRIVATE APP PARSER LIBS VENDOR TuringMachine  Qt${QT_VERSION_MAJOR}::Widgets ${CMAKE_DL_LIBS} pthread )
else()
        add_subdirectory(tme)
endif()