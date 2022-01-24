cmake_minimum_required(VERSION 3.5)

project(TME LANGUAGES CXX)

# set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64")

set(IS_GUI FALSE CACHE BOOL "Option that choose console or gui app do you like to build.")

add_subdirectory(src)