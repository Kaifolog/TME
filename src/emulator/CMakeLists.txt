add_subdirectory(libs)

add_library(APP STATIC App.cpp)

if(NOT IS_GUI)
    add_executable(TME main.cpp )

    target_link_libraries(TME APP PARSER easyloggingpp sqlite3 LIBS TuringMachine )

    if (UNIX)
        target_link_libraries(TME dl pthread)
    endif (UNIX)

endif()