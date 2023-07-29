
#include <algorithm>            // transform
#include <functional>           // ptr_fun
#include <iostream>             // cin, cout
#include <sstream>              // istringstream, iss
#include <readline/readline.h>  // getline

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>

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

// // Process arguments from the command line and passes them to encoder
// // This might be way more trouble than worth lets stick to the prompt and do command line vargs only no prompt
// void ProcessUserArg(std::string& consoleIn, encoder::Encoder& myEncoder) {
//     std::istringstream iss(consoleIn);
//     std::vector<std::string> userFlagsAndArgs;
//     std::string strItem;

//     while (iss >> strItem) {
//         userFlagsAndArgs.push_back(strItem);
//     }

//     std::string option;
//     std::string arg;

//     // Parse flags and args
//     for (size_t i = 0; i < userFlagsAndArgs.size(); ++i) {
//         const std::string& item = userFlagsAndArgs[i];
//         if (item.empty() || item[0] != '-') {
//             std::cout << "Warning: Invalid input format for option: " << item
//                       << std::endl;
//             continue;
//         }
//         option = item;

//         // Check if the next item exists and is not an option itself otherwise
//         // move to next arg
//         if (i + 1 < userFlagsAndArgs.size() &&
//             userFlagsAndArgs[i + 1][0] != '-') {
//             arg = userFlagsAndArgs[i + 1];
//             i++;
//         } else {
//             arg.clear();
//         }

//         // key arg
//         if (option == optionKeywords[0]) {
//             myEncoder.SetUserKey(arg);
//         }
//         // message arg
//         else if (option == optionKeywords[1]) {
//             myEncoder.SetPlainTextMsg(arg);
//         }
//         // help arg
//         else if (option == optionKeywords[2]) {
//             std::cout << "Simple Encoder Application.\n"
//                       << "Supported Args: -k <key> -m <message> -h <help>"
//                       << "Usage:\n-k mySecretKey -m my message to encode\n"
//                       << std::endl;
//         }
//     }
// }

int main(int argc, char** argv) {
    Encoder myEncoder = encoder::Encoder();

    if (argc <= 1) {
        std::cerr << "Error: Missing command-line arguments." << std::endl;
        std::cerr << "Usage: encoder -k <key> -m <message>" << std::endl;
        return 1;
    }

    std::string option;
    std::string arg;
    encoder::Encoder myEncoder;

    for (int i = 1; i < argc; ++i) {
        const std::string item = argv[i];

        if (item.empty() || item[0] != '-') {
            std::cerr << "Error: Invalid input format for option: " << item << std::endl;
            return 1;
        }

        option = item;
        // Sanitize option functional edition we only want lowercase, args are allowed casing
        std::transform(option.begin(), option.end(), option.begin(),
                   std::function<int(int)>(tolower));

        // Check if the next item exists and is not an option itself; otherwise, move to the next arg
        if (i + 1 < argc && argv[i + 1][0] != '-') {
            arg = argv[i + 1];
            i++;
        } else {
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
        // help arg
        else if (option == "-h") {
            std::cout << "Simple Encoder Application.\n"
                      << "Supported Args: -k <key> -m <message>\n"
                      << "Usage:\nencoder -k mySecretKey -m <cipherText>\n"
                      << std::endl;
            return 0;
        }
        // Invalid arg
        else {
            std::cerr << "Error: Unknown option: " << option << std::endl;
            return 1;
        }
    }

    // while (1) {
    //     std::cout << "Please enter a key and message to encode.\nIf you wish "
    //                  "to exit please type exit"
    //               << std::endl;
    //     std::string consoleIn = GetSanitizedUserInput();

    //     if (consoleIn == "exit") {
    //         break;
    //     }
    // ProcessUserArg(consoleIn, myEncoder);

        std::cout << "Attempting to encode" << std::endl;
        myEncoder.EncodeMessage();

        try {

            boost::interprocess::managed_shared_memory shareCipherMem(boost::interprocess::create_only, "CipherTextMemory", MEMORY_MAX);
            // Set the construct to CipherText for decoder find
            char* sharedMemoryAddress = shareCipherMem.construct<char>("CipherText")[myEncoder.GetCipherText().size()]();

            // Copy to memory using C style
            std::memcpy(sharedMemoryAddress,
                        myEncoder.GetCipherText().c_str(),
                        myEncoder.GetCipherText().size() + 1);

            // No need to detatch I believe the decode can detach but the encoder if it terminates needs to
            // leave the shared memory alone so that the decoder can access it
        }
        // Might be a better exception here but not super familiar with what to throw so just any catch is good for now
        catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
        }
    // };
}
