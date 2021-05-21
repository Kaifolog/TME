cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(VENDOR_LIST sqlite3.c)

add_library(VENDOR STATIC ${VENDOR_LIST})