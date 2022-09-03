enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_subdirectory(../src/analyser ../analyser) 
add_subdirectory(../src/machine ../machine)

add_executable(AllTests_gTests AllTests.cpp)

target_link_libraries(
AllTests_gTests

TuringMachine
sqlite3
easyloggingpp
PARSER
GTest::gtest_main
)

if (UNIX)
        target_link_libraries(AllTests_gTests dl pthread)
endif (UNIX)

include(GoogleTest)
gtest_discover_tests(AllTests_gTests)