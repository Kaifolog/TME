#include <gtest/gtest.h>
#include <string>
#include <fstream>

#include "../../src/analyser/parser.hpp"

TEST(ParsingTest, BasicTest)
{
        Parser parser;

        // you should start it from ./TME/build
        string path = "../tests/analyser/BasicTest.txt";
        ASSERT_NO_THROW(parser.parse(path));
}

TEST(ParsingTest, BasicFailTest)
{
        Parser parser;

        // you should start it from ./TME/build
        string path = "../tests/analyser/BasicFailTest.txt";
        string output;
        try
        {
                parser.parse(path);
        }
        catch (const char *message)
        {
                output = message;
        }
        catch (string message)
        {
                output = message;
        }
        ASSERT_STREQ("parse error at line #6", output.c_str());
}

TEST(ParsingTest, HardTest)
{
        Parser parser;

        // you should start it from ./TME/build
        string path = "../examples/ZhegalkinOfDualFunc.txt";
        ASSERT_NO_THROW(parser.parse(path));
}

TEST(ParsingTest, SectionDataTest)
{
        Parser parser;

        // you should start it from ./TME/build
        string path = "../tests/analyser/SectionDataTest.txt";
        string output;
        try
        {
                parser.parse(path);
        }
        catch (const char *message)
        {
                output = message;
        }
        catch (string message)
        {
                output = message;
        }
        ASSERT_STREQ("SECTION parse error at line #1", output.c_str());
}

TEST(ParsingTest, SectionTextTest)
{
        Parser parser;

        // you should start it from ./TME/build
        string path = "../tests/analyser/SectionTextTest.txt";
        string output;
        try
        {
                parser.parse(path);
        }
        catch (const char *message)
        {
                output = message;
        }
        catch (string message)
        {
                output = message;
        }
        ASSERT_STREQ("SECTION parse error at line #4", output.c_str());
}

TEST(AnalysisTest, StartTest)
{
        Parser parser;

        // you should start it from ./TME/build
        string path = "../tests/analyser/StartTest.txt";
        string output;
        try
        {
                parser.parse(path);
                parser.analyse(path);
        }
        catch (const char *message)
        {
                output = message;
        }
        catch (string message)
        {
                output = message;
        }
        ASSERT_STREQ("ANALYSER FATAL ERROR! There are no \"start\" command.", output.c_str());
}

TEST(AnalysisTest, EndTest)
{
        Parser parser;

        // you should start it from ./TME/build
        string path = "../tests/analyser/EndTest.txt";
        string output;
        try
        {
                parser.parse(path);
                parser.analyse(path);
        }
        catch (const char *message)
        {
                output = message;
        }
        catch (string message)
        {
                output = message;
        }
        ASSERT_STREQ("ANALYSER FATAL ERROR! There are no \"end\" command.", output.c_str());
}
