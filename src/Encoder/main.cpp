#include <iostream>             // cin, cout
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
void ProcessArg(encoder::Encoder& myEncoder, std::string& option, std::string& arg)
{
    // key
    if(option == optionKeywords[0])
    {
        myEncoder.SetUserKey(arg);
    }
    // message
    if(option == optionKeywords[1])
    {
        myEncoder.SetPlainTextMsg(arg);
    }
    // help
    if(option == optionKeywords[2])
    {
        std::cout << "Simple Encoder Application.\n" <<
        "Supported Args: -k <key> -m <message> -h <help>" <<
        "Usage:\n-k mySecretKey -m my message to encode\n" << std::endl;
    }

}

void EncodeMessage(encoder::Encoder& myEncoder)
{
    if(myEncoder.GetUserKey().empty())
    {
        std::cout << "Error encoder missing a key, cannot encode message." << std::endl;
    }

    if(myEncoder.GetPlainTextMsg().empty())
    {
        std::cout << "Error encoder missing a message, key has nothing to encode." << std::endl;
    }
    else
    {
        std::cout << "Encoding would be performed here!" << std::endl;
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

        std::vector<std::string> userFlagsAndArgs;
        size_t inputStart = 0;
        size_t inputEnd   = 0;

        // Look for space delimited fields until we hit null and add them to the vector
        while ((inputEnd = consoleIn.find(' ', inputStart)) != std::string::npos)
        {
            userFlagsAndArgs.push_back(consoleIn.substr(inputStart, inputEnd - inputStart));
            inputStart = inputEnd + 1;
        }
        userFlagsAndArgs.push_back(consoleIn.substr(inputStart));

        // process flags and args
        for(const std::string &item: userFlagsAndArgs)
        {
            std::string optionVal = FindOptionSubstring(item);
            if(optionVal.empty())
            {
                // No matching option at t his point no need to check the arg value
                continue;
            }

            std::string optionArgVal = GetValueForOption(item, optionVal);
            if(!optionArgVal.empty())
            {
                ProcessArg(myEncoder, optionVal, optionArgVal);
            }
            else
            {
                std::cout << "Warning option: " << optionVal << " was provided without a value.\nPlease Enter a value." << std::endl;
            }
        }

        std::cout << "Attempting to encode" << std::endl;
        EncodeMessage(myEncoder);

    };

}
