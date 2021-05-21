cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(TME VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(parser) 

add_subdirectory(lib) 

add_subdirectory(vendor) 

add_library(TuringMachine STATIC TuringMachine.cpp)

add_executable(main main.cpp text_functions.cpp App.cpp)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} dl pthread)
endif (UNIX)

target_link_libraries(main PARSER LIBS VENDOR TuringMachine )