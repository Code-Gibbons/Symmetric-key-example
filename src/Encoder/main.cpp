
#include <algorithm>            // transform
#include <functional>           // ptr_fun
#include <iostream>             // cin, cout
#include <sstream>              // istringstream, iss
#include <readline/readline.h>  // getline
#include <fstream>
// #include <boost/interprocess/managed_shared_memory.hpp>
// #include <boost/interprocess/mapped_region.hpp>

#include "encoder.h"  // Encoder class

using namespace encoder;

// Constants
static std::string optionKeywords[] = {"-k", "-m", "-h"};
static const unsigned int MEMORY_MAX = UINT16_MAX; // We can do dynamic allocation but we'll start here

// Takes user input and make it all lowercase for processing
std::string GetSanitizedUserInput(void) {
    std::string intput;
    std::getline(std::cin, intput);
    // Sanitize input functional edition
    std::transform(intput.begin(), intput.end(), intput.begin(),
                   std::function<int(int)>(tolower));
    return intput;
}

// Process vargs and populate encoder object
// Returns 0 for arg process or 1 for exit condition (either failure or help prompt presently)
int ParseVArgsIntoEncoder(int argc, char** argv, Encoder& myEncoder)
{
    if (argc <= 1) {
        std::cerr << "Error: Missing command-line arguments." << std::endl;
        std::cerr << "Usage: encoder -k <key> -m <message>" << std::endl;
        return 1;
    }

    std::string option;
    std::string arg;


    for (int i = 1; i < argc; ++i) {
        const std::string item = argv[i];

        if (item.empty() || item[0] != '-') {
            std::cerr << "Error: Invalid input format for option: " << item << std::endl;
            return 1;
        }

        // As long as items that contain spaces are quoted this works as we need it to I think that's
        // sufficient

        option = item;
        // Sanitize option functional edition we only want lowercase, args are allowed casing
        std::transform(option.begin(), option.end(), option.begin(),
                   std::function<int(int)>(tolower));

        // Check if the next item exists and is not an option itself; otherwise, move to the next arg
        if (i + 1 < argc && argv[i + 1][0] != '-') {
            arg = argv[i + 1];
            i++;
        }
        // help option has no arg
        else if (option == "-h") {
            std::cout << "Simple Encoder Application.\n"
                      << "Supported Args: -k <key> -m <message>\n"
                      << "Usage:\nencoder -k mySecretKey -m <cipherText>\n"
                      << std::endl;
            return 0;
        }
        else {
            std::cerr << "Error: Missing argument for option: " << option << std::endl;
            return 1;
        }

        // key arg
        if (option == "-k") {
            myEncoder.SetUserKey(arg);
        }
        // message arg
        else if (option == "-m") {
            myEncoder.SetPlainTextMsg(arg);
        }

        // Invalid arg
        else {
            std::cerr << "Error: Unknown option: " << option << std::endl;
            return 1;
        }
    }
    return 0;
}


// Driver for encoder
int main(int argc, char** argv) {

    Encoder myEncoder = encoder::Encoder();

    int rc = ParseVArgsIntoEncoder(argc, argv,myEncoder);
    if(rc != 0)
    {
        return -1;
    }

    std::cout << "Attempting to encode" << std::endl;
    myEncoder.EncodeMessage();

    // Going with filestream to prove this works maybe come back to share mem
    std::string ciphertext = myEncoder.GetCipherText();
    static const std::string filename = "ciphertext.bin";
    std::ofstream outFile(filename, std::ios::binary);
        if (!outFile)
        {
            std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
            return -1;
        }

        outFile.write(ciphertext.data(), ciphertext.size());
        outFile.close();

        std::cout << "Ciphertext has been written to the file: " << filename << std::endl;
        return 1;

}
