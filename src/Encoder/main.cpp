
#include <algorithm>            // transform
#include <functional>           // ptr_fun
#include <iostream>             // cin, cout
#include <sstream>              // istringstream, iss
#include <readline/readline.h>  // getline

#include "encoder.h"  // Encoder class

using namespace encoder;

// Constants
static std::string optionKeywords[] = {"-k", "-m", "-h"};

// Takes user input and make it all lowercase for processing
std::string GetSanitizedUserInput(void) {
    std::string intput;
    std::getline(std::cin, intput);
    // Sanitize input functional edition
    std::transform(intput.begin(), intput.end(), intput.begin(),
                   std::function<int(int)>(tolower));
    return intput;
}

// Process arguments from the command line and passes them to encoder
void ProcessUserArg(std::string& consoleIn, encoder::Encoder& myEncoder) {
    std::istringstream iss(consoleIn);
    std::vector<std::string> userFlagsAndArgs;
    std::string strItem;

    while (iss >> strItem) {
        userFlagsAndArgs.push_back(strItem);
    }

    std::string option;
    std::string arg;

    // Parse flags and args
    for (size_t i = 0; i < userFlagsAndArgs.size(); ++i) {
        const std::string& item = userFlagsAndArgs[i];
        if (item.empty() || item[0] != '-') {
            std::cout << "Warning: Invalid input format for option: " << item
                      << std::endl;
            continue;
        }
        option = item;

        // Check if the next item exists and is not an option itself otherwise
        // move to next arg
        if (i + 1 < userFlagsAndArgs.size() &&
            userFlagsAndArgs[i + 1][0] != '-') {
            arg = userFlagsAndArgs[i + 1];
            i++;
        } else {
            arg.clear();
        }

        // key arg
        if (option == optionKeywords[0]) {
            myEncoder.SetUserKey(arg);
        }
        // message arg
        else if (option == optionKeywords[1]) {
            myEncoder.SetPlainTextMsg(arg);
        }
        // help arg
        else if (option == optionKeywords[2]) {
            std::cout << "Simple Encoder Application.\n"
                      << "Supported Args: -k <key> -m <message> -h <help>"
                      << "Usage:\n-k mySecretKey -m my message to encode\n"
                      << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    Encoder myEncoder = encoder::Encoder();

    while (1) {
        std::cout << "Please enter a key and message to encode.\nIf you wish "
                     "to exit please type exit"
                  << std::endl;
        std::string consoleIn = GetSanitizedUserInput();

        if (consoleIn == "exit") {
            break;
        }

        ProcessUserArg(consoleIn, myEncoder);

        std::cout << "Attempting to encode" << std::endl;
        myEncoder.EncodeMessage();
        std::cout << "Your ciphertext is: " << myEncoder.GetCipherText()  << std::endl;
    };
}
