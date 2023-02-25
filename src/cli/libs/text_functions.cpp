#include "tm_pic.h"
#include <iostream>
#include <string>

namespace cli
{

void textFuncVersion()
{
    std::cout << tm_pic << "TME - Turing Machine Emulator v2.0.0b\n\n"
              << "(c)"
              << "Bulak A.S." << std::endl;
}

void textStartFunc()
{
    std::cout << "CTRL+C OR CTRL+D TO CLOSE" << std::endl << std::endl << "TME - Turing Machine Emulator v2.0.0b\n\n";

    std::cout << "How to?" << std::endl
              << "Link to github repository with documentation and examples: https://github.com/Kaifolog/TME"
              << std::endl
              << std::endl
              << std::endl;

    std::cout
        << "The first argument is positional and should be either \" - v \" or the path of the interpretation file."
        << std::endl
        << std::endl
        << "Non-positional arguments list:" << std::endl
        << "\t\"-g\" - generate db and temporary data files," << std::endl
        << "\t\"-a\" - analysis, really recommend once before \"-e\". Can help you to find some logical gaps,"
        << std::endl
        << "\t\"-e\" - emulate (u have to use \"-g\" at least one time before emulating)," << std::endl
        << "\t\"-d\" - debug, executing your programm step by step," << std::endl
        << "\t\"-l\" - this one gonna change all \"lambda\" to \" \'.'" << std::endl;

    std::cout << std::endl
              << std::endl
              << std::endl
              << "(c)"
              << "Bulak A.S." << std::endl;
}
}