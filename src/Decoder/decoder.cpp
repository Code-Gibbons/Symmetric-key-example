#include "decoder.h"

#include <readline/readline.h>
#include <iostream>

#include <eccrypto.h>
#include <osrng.h>
#include <hex.h>
#include <pwdbased.h>
#include <files.h>
#include <asn.h>
#include <oids.h>

//******************************************************************************
// Private instance variables
//******************************************************************************
const int unsigned SIZE_BYTES_DERIVED_KEY   = 32;
const unsigned int NUM_PBKDF2_ITERATIONS    = 10000;

//******************************************************************************
// Constructors
//******************************************************************************
decoder::Decoder::Decoder() {
    decoder::Decoder::userKey = "";
    decoder::Decoder::plainTextMsg = "";
    decoder::Decoder::cipherText = "";
    decoder::Decoder::publicKey;
    decoder::Decoder::privateKey;
}

decoder::Decoder::Decoder(std::string userKey, std::string cipherText) {
    decoder::Decoder::userKey = userKey;
    decoder::Decoder::plainTextMsg = "";
    decoder::Decoder::cipherText = "";
    decoder::Decoder::publicKey;
    decoder::Decoder::privateKey;
}

//******************************************************************************
// Setters
//******************************************************************************

void decoder::Decoder::SetPlainTextMsg(std::string userMsg) {
    decoder::Decoder::plainTextMsg = userMsg;
}

void decoder::Decoder::SetUserKey(std::string userKey) {
    decoder::Decoder::userKey = userKey;
}

void decoder::Decoder::SetCipherText(std::string cipherText) {
    decoder::Decoder::cipherText = cipherText;
}

void decoder::Decoder::SetPublicKey(CryptoPP::ECIES<CryptoPP::ECP>::PublicKey publicKey){
    decoder::Decoder::publicKey = publicKey;
}

void decoder::Decoder::SetPrivateKey(CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey privateKey){
    decoder::Decoder::privateKey = privateKey;
}


//******************************************************************************
// Getters
//******************************************************************************
std::string decoder::Decoder::GetUserKey() { return decoder::Decoder::userKey; }

std::string decoder::Decoder::GetPlainTextMsg() {
    return decoder::Decoder::plainTextMsg;
}
std::string decoder::Decoder::GetCipherText() {
    return decoder::Decoder::cipherText;
}

CryptoPP::ECIES<CryptoPP::ECP>::PublicKey decoder::Decoder::GetPublicKey(void) {
    return publicKey;
}

CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey decoder::Decoder::GetPrivateKey(void){
    return privateKey;
}