#include <algorithm>            // transform
#include <functional>           // ptr_fun
#include <iostream>             // cin, cout
#include <sstream>              // istringstream, iss
#include <readline/readline.h>  // getline
#include <fstream>
// #include <boost/interprocess/managed_shared_memory.hpp>

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

    // Let's just do filestream to get this working boost has proven alot more effort than I expected
    std::string ciphertext;
    static const std::string filename = "ciphertext.bin";
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile)
    {
        std::cerr << "Error: Unable to open file for reading: " << filename << std::endl;
        return -1;
    }

    // Determine the size of the file
    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    // Read the ciphertext from the file into a buffer
    std::vector<char> buffer(fileSize);
    inFile.read(buffer.data(), fileSize);
    inFile.close();

    // Set the ciphertext in the decoder
    myDecoder.SetCipherText(buffer.data());

    std::cout << "Ciphertext has been read from the file: " << filename << std::endl;

    std::cout << "Received cipher text from shared memory: " << myDecoder.GetCipherText() << std::endl;
    myDecoder.DecodeMessage();
    std::cout << "Using the key: "<< myDecoder.GetUserKey() << ". Decoded the following message: \n" << myDecoder.GetPlainTextMsg() << std::endl;



    return 0;



}