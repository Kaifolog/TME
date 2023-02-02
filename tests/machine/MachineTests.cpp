#include <fstream>
#include <gtest/gtest.h>
#include <string>

#include "../../src/machine/TuringMachine.hpp"
#include "../../src/translator/Translator.hpp"

TEST(ExecutionTest, BasicTest)
{
    machine::TuringMachine tm;

    // preparing test
    std::ofstream datasec;
    datasec.open("datasection.tmp");
    datasec << "|lambda|";
    datasec.close();

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/machine/BasicTest.txt");
    ASSERT_NO_THROW(tm.execute(pn, 0));

    // preparing test
    std::string result;
    std::ifstream output("../tests/machine/BasicTest_out.txt");
    getline(output, result);
    ASSERT_STREQ("1 1 1 |lambda| ", result.c_str());
}

TEST(ExecutionTest, LambdaTest)
{
    machine::TuringMachine tm;

    // preparing test
    std::ofstream datasec;
    datasec.open("datasection.tmp");
    datasec << "|lambda|";
    datasec.close();

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/machine/BasicTest.txt");
    ASSERT_NO_THROW(tm.execute(pn, 1));

    // preparing test
    std::string result;
    std::ifstream output("../tests/machine/BasicTest_out.txt");
    getline(output, result);
    ASSERT_STREQ("1 1 1 | | ", result.c_str());
}

TEST(ExecutionTest, HardTest)
{
    machine::TuringMachine tm;

    // preparing test
    std::ofstream datasec;
    datasec.open("datasection.tmp");
    datasec << "|1| 0 1 0 1 0 1 0 0 1 0 1 0 1 0 1";
    datasec.close();

    // you should start it from ./TME/build
    tools::ProjectName pn("../examples/ZhegalkinOfDualFunc.txt");
    ASSERT_NO_THROW(tm.execute(pn, 1));

    // preparing test
    std::string result;
    std::ifstream output("../examples/ZhegalkinOfDualFunc_out.txt");
    getline(output, result);
    ASSERT_STREQ("1 0 1 0 1 0 1 0 0 1 0 1 0 1 0 1 =                                   x4 + |x1|                        "
                 "                                                                                                     "
                 "                                ",
                 result.c_str());
}

TEST(DebuggingTest, LambdaTest)
{
    machine::TuringMachine tm;
    machine::MachineState result;

    std::ofstream datasec;
    datasec.open("datasection.tmp");
    datasec << "|lambda|";
    datasec.close();

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/machine/BasicTest.txt");
    ASSERT_NO_THROW(tm.lazyStart(pn, 0));
    ASSERT_NO_THROW(result = tm.lazyDebug(););
    ASSERT_EQ(result.line, "6");
    ASSERT_NO_THROW(result = tm.lazyDebug(););
    ASSERT_EQ(result.current_state, "end");
    // ASSERT_NO_THROW(result = tm.lazyDebug();); // remember it for writing destructive test
    // ASSERT_EQ(result.current_state, "");
    tm.lazyFinalize();
}