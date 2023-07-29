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

//******************************************************************************
// Logic
//******************************************************************************

// Generate ECC keyset from user key
void decoder::Decoder::GenerateECCKeysFromUserKey()
{
    CryptoPP::AutoSeededRandomPool prng;
    // According to API below call is a managed with zeroization
    CryptoPP::SecByteBlock derivedKey(SIZE_BYTES_DERIVED_KEY);

    CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> ecc;
    // Attention should be given that this is CryptoPP::byte not std::byte
    ecc.DeriveKey(derivedKey.data(), derivedKey.size(), 0,
    reinterpret_cast<const CryptoPP::byte*>(userKey.data()),
    userKey.size(), nullptr, 0, NUM_PBKDF2_ITERATIONS);

    prng.IncorporateEntropy(derivedKey.data(), derivedKey.size());

    // This setups params and initializes a key using curve secp256r1
    // Not sure of advantages or weakness of this curve but it seems popular when researching
    // so I feel it's sufficiently hardened.
    CryptoPP::DL_GroupParameters_EC<CryptoPP::ECP> params;
    params.Initialize(CryptoPP::ASN1::secp256r1());
    privateKey.Initialize(prng, params);
    privateKey.MakePublicKey(publicKey);
}

// Decrypt the mesasge using the key
std::string decoder::Decoder::ECC_DecryptMessage(void) {
    std::string decryptedText;

    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::ECIES<CryptoPP::ECP>::Decryptor decryptor(privateKey);
    CryptoPP::StringSource(cipherText, true,
                           new CryptoPP::PK_DecryptorFilter(prng, decryptor,
                                                           new CryptoPP::StringSink(decryptedText)));

    return decryptedText;
}


void decoder::Decoder::DecodeMessage() {
    if (userKey.empty()) {
        std::cout << "Decoder Error: missing the key, cannot decode ciphertext."
                  << std::endl;
        return;
    }

    if (cipherText.empty()) {
        std::cout
            << "Decoder Error: missing the ciphertext, key has nothing to decode."
            << std::endl;
        return;
    }
    else {
        std::cout << "Encoding the provided message." << std::endl;
        std::cout << "Using passkey : " << userKey << std::endl;
        std::cout << "For ciphertext: " << cipherText
                  << std::endl;

    // Generate ECC keys from the user key
    CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey privateKey;
    CryptoPP::ECIES<CryptoPP::ECP>::PublicKey publicKey;
    GenerateECCKeysFromUserKey();

    plainTextMsg = ECC_DecryptMessage();

    }
}