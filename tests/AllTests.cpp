#include <gtest/gtest.h>

#include "translator/AnalyserTests.cpp"
#include "machine/MachineTests.cpp"

int main(int argc, char **argv)
{
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}