#include <fstream>
#include <gtest/gtest.h>
#include <string>

#include "../../src/translator/Translator.hpp"

TEST(ParsingTest, BasicTest)
{
    translator::Translator translator;

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/translator/BasicTest.txt");
    ASSERT_NO_THROW(translator.parse(pn));
}

TEST(ParsingTest, BasicFailTest)
{
    translator::Translator translator;

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/translator/BasicFailTest.txt");
    std::string output;
    try
    {
        translator.parse(pn);
    }
    catch (const char *message)
    {
        output = message;
    }
    catch (std::string message)
    {
        output = message;
    }
    ASSERT_STREQ("Parse error at line #6", output.c_str());
}

TEST(ParsingTest, HardTest)
{
    translator::Translator translator;

    // you should start it from ./TME/build
    tools::ProjectName pn("../examples/ZhegalkinOfDualFunc.txt");
    ASSERT_NO_THROW(translator.parse(pn));
}

TEST(ParsingTest, SectionDataTest)
{
    translator::Translator translator;

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/translator/SectionDataTest.txt");
    std::string output;
    try
    {
        translator.parse(pn);
    }
    catch (const char *message)
    {
        output = message;
    }
    catch (std::string message)
    {
        output = message;
    }
    ASSERT_STREQ("Section parse error at line #1", output.c_str());
}

TEST(ParsingTest, SectionTextTest)
{
    translator::Translator translator;

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/translator/SectionTextTest.txt");
    std::string output;
    try
    {
        translator.parse(pn);
    }
    catch (const char *message)
    {
        output = message;
    }
    catch (std::string message)
    {
        output = message;
    }
    ASSERT_STREQ("Section parse error at line #4", output.c_str());
}

TEST(AnalysisTest, StartTest)
{
    translator::Translator translator;

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/translator/StartTest.txt");
    std::string output;
    try
    {
        translator.parse(pn);
        translator.analyse(pn);
    }
    catch (const char *message)
    {
        output = message;
    }
    catch (std::string message)
    {
        output = message;
    }
    ASSERT_STREQ("Analyser FATAL ERROR: There are no \"start\" command.", output.c_str());
}

TEST(AnalysisTest, EndTest)
{
    translator::Translator translator;

    // you should start it from ./TME/build
    tools::ProjectName pn("../tests/translator/EndTest.txt");
    std::string output;
    try
    {
        translator.parse(pn);
        translator.analyse(pn);
    }
    catch (const char *message)
    {
        output = message;
    }
    catch (std::string message)
    {
        output = message;
    }
    ASSERT_STREQ("Analyser FATAL ERROR: There are no \"end\" command.", output.c_str());
}
