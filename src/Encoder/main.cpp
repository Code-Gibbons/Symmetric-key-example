#include <iostream>             // cin, cout
#include <sstream>              // istringstream, iss
#include <readline/readline.h>  // getline
#include <algorithm>            // transform
#include <functional>           // ptr_fun

#include "encoder.h" // Encoder class

using namespace encoder;

// Constants
static std::string optionKeywords[] = {"-k", "-m", "-h"};

// Searches a proved string for any of the given option keywords
// Returns an empty string if any of the option is not found
std::string FindOptionSubstring(const std::string& input)
{
    // Lambda time search any of matching args
    auto substr = std::find_if(std::begin(optionKeywords), std::end(optionKeywords),
                                        [&](const std::string& str){
                                            return input.find(str) != std::string::npos;
                                        }
    );

    if(substr != std::end(optionKeywords))
    {
        return *substr;
    }
    else
    {
        return "";
    }
}

// This function will return the argument value after an option is detected
// Return an empty string if the option specified is not found
std::string GetValueForOption(const std::string& input, const std::string& option)
{
    size_t pos = input.find(option);
    if (pos != std::string::npos)
    {
        size_t valueStart = pos + option.length();
        return input.substr(valueStart);
    }
    return "";
}

// Takes user input and make it all lowercase for processing
std::string GetSanitizedUserInput(void)
{
    std::string intput;
    std::getline(std::cin, intput);
    // Sanitize input functional edition
    std::transform(intput.begin(), intput.end(), intput.begin(), std::function<int(int)> (tolower));
    return intput;
}

// Process arguments from the command line and updates the provided encoder
void ProcessArg(std::string& consoleIn, encoder::Encoder& myEncoder)
{
    std::istringstream iss(consoleIn);
    std::vector<std::string> userFlagsAndArgs;
    std::string strItem;

    while (iss >> strItem)
    {
        userFlagsAndArgs.push_back(strItem);
    }

    std::string option;
    std::string arg;

    // Parse flags and args
    for (size_t i = 0; i < userFlagsAndArgs.size(); ++i)
    {
        const std::string& item = userFlagsAndArgs[i];
        if (item.empty() || item[0] != '-')
        {
            std::cout << "Warning: Invalid input format for option: " << item << std::endl;
            continue;
        }
        option = item;

        // Check if the next item exists and is not an option itself otherwise move to next arg
        if (i + 1 < userFlagsAndArgs.size() && userFlagsAndArgs[i + 1][0] != '-')
        {
            arg = userFlagsAndArgs[i + 1];
            i++;
        }
        else
        {
            arg.clear();
        }

        // key arg
        if(option == optionKeywords[0])
        {
            myEncoder.SetUserKey(arg);
        }
        // message arg
        else if(option == optionKeywords[1])
        {
            myEncoder.SetPlainTextMsg(arg);
        }
        // help arg
        else if(option == optionKeywords[2])
        {
            std::cout << "Simple Encoder Application.\n" <<
            "Supported Args: -k <key> -m <message> -h <help>" <<
            "Usage:\n-k mySecretKey -m my message to encode\n" << std::endl;
        }
    }
}

void EncodeMessage(encoder::Encoder& myEncoder)
{
    if(myEncoder.GetUserKey().empty())
    {
        std::cout << "Error encoder missing a key, cannot encode message." << std::endl;
        return;
    }

    if(myEncoder.GetPlainTextMsg().empty())
    {
        std::cout << "Error encoder missing a message, key has nothing to encode." << std::endl;
        return;
    }
    else
    {
        std::cout << "Encoding would be performed here!" << std::endl;
        std::cout << "Using key: " << myEncoder.GetUserKey() << std::endl;
        std::cout << "For   message: " << myEncoder.GetPlainTextMsg() << std::endl;
    }
}

int main(int argc, char **argv)
{
    Encoder myEncoder = encoder::Encoder();

    while(1)
    {
        std::cout << "Please enter a key and message to encode.\nIf you wish to exit please type exit" << std::endl;
        std::string consoleIn = GetSanitizedUserInput();

        if(consoleIn == "exit")
        {
            break;
        }

        ProcessArg(consoleIn , myEncoder);

        std::cout << "Attempting to encode" << std::endl;
        EncodeMessage(myEncoder);
    };
}
