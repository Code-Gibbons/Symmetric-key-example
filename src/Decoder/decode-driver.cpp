#include <algorithm>            // transform
#include <functional>           // ptr_fun
#include <iostream>             // cin, cout
#include <sstream>              // istringstream, iss
#include <readline/readline.h>  // getline

#include <boost/interprocess/managed_shared_memory.hpp>

#include "decoder.h"

using namespace decoder;

// Process vargs and populate decoder object
// Returns 0 for arg process or 1 for exit condition (either failure or help prompt presently)
int ParseVArgsIntoDecoder(int argc, char** argv, Decoder& myDecoder)
{
    if (argc <= 1) {
        std::cerr << "Error: Missing command-line arguments." << std::endl;
        std::cerr << "Usage: decoder -k <key>" << std::endl;
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

        option = item;

        // Check if the next item exists and is not an option itself; otherwise, move to the next arg
        if (i + 1 < argc && argv[i + 1][0] != '-') {
            arg = argv[i + 1];
            i++;
        }
        // help arg
        else if (option == "-h") {
            std::cout << "Simple Decoder Application.\n"
                      << "Supported Args: -k <key>\n"
                      << "Usage:\ndecoder -k \"my SecretKey\"\n"
                      << std::endl;
            return 1;
        }
        else {
            std::cerr << "Error: Missing argument for option: " << option << std::endl;
            return 1;
        }

        // key arg
        if (option == "-k") {
            myDecoder.SetUserKey(arg);

        }
        // Invalid arg
        else {
            std::cerr << "Error: Unknown option: " << option << std::endl;
            return 1;
        }
    }
    return 0;
}


int main(int argc, char** argv)
{
    Decoder myDecoder = decoder::Decoder();

    int rc = ParseVArgsIntoDecoder(argc, argv, myDecoder);
    if(rc !=0)
    {
        return -1; // Something signaled end of program
    }

    // We only want this to read not modify
    try
    {
        boost::interprocess::managed_shared_memory cipherMem(boost::interprocess::open_only, "CipherTextMemory");

        auto cipherTextPtr = cipherMem.find<char>("CipherText");
        if (cipherTextPtr.first) {
            const char* sharedMemoryAddress = cipherTextPtr.first;
            myDecoder.SetCipherText(sharedMemoryAddress);
            boost::interprocess::shared_memory_object::remove("CipherTextMemory");
        }
        else
        {
            std::cout << "Error no cipher text was found please run the encoder to generate the ciphertext." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }


    std::cout << "Received cipher text from shared memory: " << myDecoder.GetCipherText() << std::endl;
    myDecoder.DecodeMessage();
    std::cout << "Using the key: "<< myDecoder.GetUserKey() << ". Decoded the following message: \n" << myDecoder.GetPlainTextMsg() << std::endl;


    // Shared memory segment will be automatically deleted when all processes using it exit


}