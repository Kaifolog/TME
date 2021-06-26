cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(VENDOR_LIST sqlite3/sqlite3.c easyloggingpp/easylogging++.cc)

add_library(VENDOR STATIC ${VENDOR_LIST})