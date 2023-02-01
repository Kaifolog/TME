#include <gtest/gtest.h>
#include <string>
#include <fstream>

#include "../../src/translator/translator.hpp"

TEST(ParsingTest, BasicTest)
{
        Translator translator;

        // you should start it from ./TME/build
        Tools::ProjectName pn("../tests/translator/BasicTest.txt");
        ASSERT_NO_THROW(translator.parse(pn));
}

TEST(ParsingTest, BasicFailTest)
{
        Translator translator;

        // you should start it from ./TME/build
        Tools::ProjectName pn("../tests/translator/BasicFailTest.txt");
        string output;
        try
        {
                translator.parse(pn);
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
        Translator translator;

        // you should start it from ./TME/build
        Tools::ProjectName pn("../examples/ZhegalkinOfDualFunc.txt");
        ASSERT_NO_THROW(translator.parse(pn));
}

TEST(ParsingTest, SectionDataTest)
{
        Translator translator;

        // you should start it from ./TME/build
        Tools::ProjectName pn("../tests/translator/SectionDataTest.txt");
        string output;
        try
        {
                translator.parse(pn);
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
        Translator translator;

        // you should start it from ./TME/build
        Tools::ProjectName pn("../tests/translator/SectionTextTest.txt");
        string output;
        try
        {
                translator.parse(pn);
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
        Translator translator;

        // you should start it from ./TME/build
        Tools::ProjectName pn("../tests/translator/StartTest.txt");
        string output;
        try
        {
                translator.parse(pn);
                translator.analyse(pn);
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
        Translator translator;

        // you should start it from ./TME/build
        Tools::ProjectName pn("../tests/translator/EndTest.txt");
        string output;
        try
        {
                translator.parse(pn);
                translator.analyse(pn);
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
