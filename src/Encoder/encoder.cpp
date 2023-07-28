#include "encoder.h"

#include <readline/readline.h>

#include <iostream>

//******************************************************************************
// Constructors
//******************************************************************************
encoder::Encoder::Encoder() {
    encoder::Encoder::userKey = "";
    encoder::Encoder::plainTextMsg = "";
    encoder::Encoder::cipherText = "";
}

encoder::Encoder::Encoder(std::string userKey, std::string userMsg) {
    encoder::Encoder::userKey = userKey;
    encoder::Encoder::plainTextMsg = userMsg;
    encoder::Encoder::cipherText = "";  // Empty cipher text until processed
}

//******************************************************************************
// Setters
//******************************************************************************

void encoder::Encoder::SetPlainTextMsg(std::string userMsg) {
    encoder::Encoder::plainTextMsg = userMsg;
}

void encoder::Encoder::SetUserKey(std::string userKey) {
    encoder::Encoder::userKey = userKey;
}

void encoder::Encoder::SetCipherText(std::string cipherText) {
    encoder::Encoder::cipherText = cipherText;
}

//******************************************************************************
// Getters
//******************************************************************************
std::string encoder::Encoder::GetUserKey() { return encoder::Encoder::userKey; }

std::string encoder::Encoder::GetPlainTextMsg() {
    return encoder::Encoder::plainTextMsg;
}

std::string encoder::Encoder::GetCipherText() {
    return encoder::Encoder::cipherText;
}

//******************************************************************************
// Logic
//******************************************************************************
void encoder::Encoder::EncodeMessage() {
    if (userKey.empty()) {
        std::cout << "Error encoder missing a key, cannot encode message."
                  << std::endl;
        return;
    }

    if (plainTextMsg.empty()) {
        std::cout
            << "Error encoder missing a message, key has nothing to encode."
            << std::endl;
        return;
    } else {
        std::cout << "Encoding would be performed here!" << std::endl;
        std::cout << "Using key: " << userKey << std::endl;
        std::cout << "For   message: " << plainTextMsg
                  << std::endl;
    }
}