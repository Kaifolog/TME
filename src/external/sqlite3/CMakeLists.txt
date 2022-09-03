FetchContent_MakeAvailable(sqlite3)

file(COPY ${sqlite3_SOURCE_DIR}/sqlite3.c DESTINATION ${CMAKE_SOURCE_DIR}/src/external/sqlite3/)
file(COPY ${sqlite3_SOURCE_DIR}/sqlite3.h DESTINATION ${CMAKE_SOURCE_DIR}/src/external/sqlite3/)

add_library(sqlite3 STATIC sqlite3.c)