cmake_minimum_required(VERSION 3.13)

project(TME LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)

# set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64")

# for vendor libs
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(IS_GUI FALSE CACHE BOOL "An option that chooses a console or gui app do you like to build.")

set(TEST FALSE CACHE BOOL "An option that manages compilation flow between building test and release versions of a program.")

add_subdirectory(src)