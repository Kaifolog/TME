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

add_subdirectory(../src/emulator/vendor ../emulator/machine) 
add_subdirectory(../src/machine ../machine)
add_executable(MachineTests_gTests ./machine/MachineTests.cpp)

target_link_libraries(
MachineTests_gTests

TuringMachine
GTest::gtest_main
VENDOR
)

if (UNIX)
        target_link_libraries(MachineTests_gTests dl pthread)
endif (UNIX)

include(GoogleTest)
gtest_discover_tests(MachineTests_gTests)