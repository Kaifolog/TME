cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(PARSER_LIST "command.cpp" "parser.cpp")

add_library(PARSER STATIC ${PARSER_LIST})