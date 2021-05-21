cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(LIBS_LIST logging.c svwm.c)

add_library(LIBS STATIC ${LIBS_LIST})