#include <iostream>             // cin, cout
#include <readline/readline.h>  // getline
#include <algorithm>            // transform
#include <functional>           // ptr_fun

#include "encoder.h" // Encoder class

using namespace encoder;

    // Constants
    const char keyArgChar   = 'k';
    const char msgArgChar   = 'm';
    const char helpArgChar  = 'h';
    const char flagChar     =  '-';

int main(int argc, char **argv)
{
    std::string consoleIn;
    std::string passKeyArg;
    std::string messageArg;

    while(1)
    {
        std::cout << "Please enter a key and message to encode.\nIf you wish to exit please type exit" << std::endl;
        std::getline(std::cin, consoleIn);
        // Sanitize input functional edition
        std::transform(consoleIn.begin(), consoleIn.end(), consoleIn.begin(), std::function<int(int)> (tolower));

        if(consoleIn == "exit")
        {
            break;
        }

        std::vector<std::string> userFlagsAndArgs;
        size_t inputStart = 0;
        size_t inputEnd   = 0;

        return 0;
    }

}

