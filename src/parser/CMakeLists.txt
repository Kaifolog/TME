cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(PARSER_LIST lexer.c)

add_library(PARSER STATIC ${PARSER_LIST})