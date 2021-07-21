#include "tm_pic.h"
#include <iostream>
#include <string>
using namespace std;

void text_v_func()
{
     cout << tm_pic << "TME - Turing Machine Emulator V1.001\n\n"
          << "(c)"
          << "Bulak A.S." << endl;
}

void text_start_func()
{
     cout << "CTRL+C OR CTRL+D TO CLOSE" << endl
          << endl
          << "TME - Turing Machine Emulator V1.001\n\n";

     cout << "How to?" << endl
          << "Link to github repository with documentation and examples: https://github.com/Kaifolog/TME" << endl
          << endl
          << endl;

     cout
         << "The first argument is positional and should be either \" - v \" or the path of the interpretation file."
         << endl
         << endl
         << "Non-positional arguments list:" << endl
         << "\t\"-g\" - generate db and temporary data files," << endl
         << "\t\"-a\" - analysis, really recommend once before \"-e\". Can help you to find some logical gaps," << endl
         << "\t\"-e\" - emulate (u have to use \"-g\" at least one time before emulating)," << endl
         << "\t\"-d\" - debug, executing your programm step by step," << endl
         << "\t\"-l\" - this one gonna change all \"lambda\" to \" \"." << endl;

     cout
         << endl
         << endl
         << endl
         << "(c)"
         << "Bulak A.S." << endl;
}