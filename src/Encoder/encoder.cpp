#include <iostream>
#include <readline/readline.h>

#include "encoder.h"

const char keyArgChar = 'k';
const char msgArgChar = 'm';
const char helpArgChar = 'h';
const char flagChar =  '-';

//******************************************************************************
// Constructors
//******************************************************************************
encoder::Encoder::Encoder(std::string userKey, std::string userMsg)
{
    encoder::Encoder::userKey = userKey;
    encoder::Encoder::plainTextMsg = userMsg;
    encoder::Encoder::cipherText = ""; // Empty cipher text until processed
}

//******************************************************************************
// Setters
//******************************************************************************

void encoder::Encoder::SetPlainTextMsg(std::string userMsg)
{
    encoder::Encoder::plainTextMsg = userMsg;
}

void encoder::Encoder::SetUserKey(std::string userKey)
{
    encoder::Encoder::userKey = userKey;
}

void encoder::Encoder::SetCipherText(std::string cipherText)
{
    encoder::Encoder::cipherText = cipherText;
}

//******************************************************************************
// Getters
//******************************************************************************
std::string encoder::Encoder::GetUserKey()
{
    return encoder::Encoder::userKey;
}

std::string encoder::Encoder::GetPlainTextMsg()
{
    return encoder::Encoder::plainTextMsg;
}

std::string encoder::Encoder::GetCipherText()
{
    return encoder::Encoder::cipherText;
}


int main(int argc, char **argv)
{
    std::string passKeyArg;
    std::string messageArg;

    // Don't forget to start at one so we skip program name
    for(int i = 1; i <argc; ++i)
    {
        // Look for the flag and check if it's a supported one
        if(argv[i][0] == flagChar)
        {
            // Don't overflow past max inputs
            if( i + 1 < argc)
            {
                if(argv[i][1] == 'k')
                {
                    passKeyArg = argv[i +1];
                    i++;
                }
                else if(argv[i][1] == 'm')
                {
                    messageArg = argv[i+1];
                    i++;
                }
                else
                {
                    std::cout << "Option: " << argv[i] << " requires a value." << std::endl;
                }
            }
            else
            {
                std::cout << "Unsupported argument entered: " << argv[i] << std::endl;
            }

        }
        else
        {
            std::cout << "Error.\n Flag provided: " << argv[i]
            << " is not a supported flag.\n Please input a supported flag." << std::endl;
        }



    }

}