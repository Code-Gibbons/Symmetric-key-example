#include <iostream> //
#include <encoder.h>
using namespace encoder;

std::string GetUserKey()
{
    std::string inputKey;
    std::getline(std::cin, inputKey);
    return inputKey;
}

int main()
{
    std::string userInput = encoder::GetUserKey();
    std::cout << "User input was: " << userInput << std::endl;
}