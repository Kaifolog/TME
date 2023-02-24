cmake_minimum_required(VERSION 2.9...3.16)

# project(TME LANGUAGES CXX C)
project(TME
        VERSION 2.0.7.1
        DESCRIPTION "The best turing machine emulator for Windows"
        HOMEPAGE_URL "https://kaifolog.github.io/TME-website/"
        LANGUAGES CXX C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GUI FALSE CACHE BOOL "An option that defines whether CLI or GUI app would you build.")

set(TME_GUI_WIN_STATIC FALSE CACHE BOOL "An option that defines whether GUI_WIN_STATIC app would you build.")

set(AUTOTEST FALSE CACHE BOOL "An option that manages compilation flow between building test and release version of the program.")

add_subdirectory(src)