cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(TME VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(parser) 

add_subdirectory(libs)

add_subdirectory(vendor) 

add_library(TuringMachine STATIC TuringMachine.cpp)

add_executable(TME main.cpp App.cpp)

target_link_libraries(TME PARSER VENDOR LIBS TuringMachine )

if (UNIX)
    target_link_libraries(TME dl pthread)
endif (UNIX)