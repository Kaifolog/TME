#include <gtest/gtest.h>
#include <string>
#include <fstream>

#include "../../src/machine/TuringMachine.hpp"

TEST(ExecutionTest, BasicTest)
{
        TuringMachine tm;

        // preparing test
        ofstream datasec;
        datasec.open("datasection.tmp");
        datasec << "|lambda|";
        datasec.close();

        // you should start it from ./TME/build
        string path = "../tests/machine/BasicTest.txt";
        ASSERT_NO_THROW(tm.execute(path, 0));

        // preparing test
        string result;
        ifstream output("../tests/machine/BasicTest_out.txt");
        getline(output, result);
        ASSERT_STREQ("1 1 1 |lambda| ", result.c_str());
}

TEST(ExecutionTest, LambdaTest)
{
        TuringMachine tm;

        // preparing test
        ofstream datasec;
        datasec.open("datasection.tmp");
        datasec << "|lambda|";
        datasec.close();

        // you should start it from ./TME/build
        string path = "../tests/machine/BasicTest.txt";
        ASSERT_NO_THROW(tm.execute(path, 1));

        // preparing test
        string result;
        ifstream output("../tests/machine/BasicTest_out.txt");
        getline(output, result);
        ASSERT_STREQ("1 1 1 | | ", result.c_str());
}

TEST(ExecutionTest, HardTest)
{
        TuringMachine tm;

        // preparing test
        ofstream datasec;
        datasec.open("datasection.tmp");
        datasec << "|1| 0 1 0 1 0 1 0 0 1 0 1 0 1 0 1";
        datasec.close();

        // you should start it from ./TME/build
        string path = "../examples/ZhegalkinOfDualFunc.txt";
        ASSERT_NO_THROW(tm.execute(path, 1));

        // preparing test
        string result;
        ifstream output("../examples/ZhegalkinOfDualFunc_out.txt");
        getline(output, result);
        ASSERT_STREQ("1 0 1 0 1 0 1 0 0 1 0 1 0 1 0 1 =                                   x4 + |x1|                                                                                                                                                             ", result.c_str());
}

TEST(DebuggingTest, LambdaTest)
{
        TuringMachine tm;
        MachineState result;

        ofstream datasec;
        datasec.open("datasection.tmp");
        datasec << "|lambda|";
        datasec.close();

        // you should start it from ./TME/build
        string path = "../tests/machine/BasicTest.txt";
        ASSERT_NO_THROW(tm.lazyStart(path, 0));
        ASSERT_NO_THROW(result = tm.lazyDebug(););
        ASSERT_EQ(result.line, "6");
        ASSERT_NO_THROW(result = tm.lazyDebug(););
        ASSERT_EQ(result.current_state, "end");
        // ASSERT_NO_THROW(result = tm.lazyDebug();); // remember it for writing destructive test
        // ASSERT_EQ(result.current_state, "");
        tm.lazyFinalize();
}