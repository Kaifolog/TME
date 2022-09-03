include(FetchContent)

# easyloggingpp
FetchContent_Declare(
        easyloggingpp
        GIT_REPOSITORY https://github.com/amrayn/easyloggingpp.git
        GIT_TAG        v9.96.7
)
add_subdirectory(easyloggingpp)
FetchContent_Declare(
        sqlite3
        URL http://sqlite.org/sqlite-amalgamation-3071300.zip
)
add_subdirectory(sqlite3)