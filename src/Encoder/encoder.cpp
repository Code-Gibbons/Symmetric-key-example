#include "encoder.h"

#include <readline/readline.h>
#include <iostream>
#include <cstdlib> // For temporary directory handling

#include <eccrypto.h>
#include <osrng.h>
#include <hex.h>
#include <pwdbased.h>
#include <files.h>
#include <asn.h>
#include <oids.h>
#include <filters.h> // CryptoPP::StringSource


//******************************************************************************
// Private instance variables
//******************************************************************************
const int unsigned SIZE_BYTES_DERIVED_KEY   = 32;
const unsigned int NUM_PBKDF2_ITERATIONS    = 10000;

//******************************************************************************
// Constructors
//******************************************************************************
encoder::Encoder::Encoder() {
    encoder::Encoder::userKey = "";
    encoder::Encoder::plainTextMsg = "";
    encoder::Encoder::cipherText = "";
    encoder::Encoder::publicKey;
    encoder::Encoder::privateKey;
}

encoder::Encoder::Encoder(std::string userKey, std::string userMsg) {
    encoder::Encoder::userKey = userKey;
    encoder::Encoder::plainTextMsg = userMsg;
    encoder::Encoder::cipherText = "";  // Empty cipher text until processed
    encoder::Encoder::publicKey;
    encoder::Encoder::privateKey;
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

void encoder::Encoder::SetPublicKey(CryptoPP::ECIES<CryptoPP::ECP>::PublicKey publicKey){
    encoder::Encoder::publicKey = publicKey;
}

void encoder::Encoder::SetPrivateKey(CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey privateKey){
    encoder::Encoder::privateKey = privateKey;
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

CryptoPP::ECIES<CryptoPP::ECP>::PublicKey encoder::Encoder::GetPublicKey(void) {
    return publicKey;
}

CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey encoder::Encoder::GetPrivateKey(void){
    return privateKey;
}


//******************************************************************************
// Logic
//******************************************************************************

// Function to generate ECC keys using ECC generator PBKDF2 which is a "simple"
// derivation function but still resistant to dictionary attacks
void encoder::Encoder::GenerateECCKeysFromUserKey()
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

// Encrypt the message using the public key we just generated
std::string encoder::Encoder::ECC_EncryptMessage(void) {
        std::string cipherText;

        CryptoPP::AutoSeededRandomPool prng;
        CryptoPP::ECIES<CryptoPP::ECP>::Encryptor encryptor(publicKey);
        CryptoPP::StringSource(plainTextMsg, true, new CryptoPP::PK_EncryptorFilter(prng, encryptor,
        new CryptoPP::StringSink(cipherText)));

        return cipherText;

}

// Function to dump keys to a temporary directory
void TempKeyStorage(const std::string& privateKeyHex, const std::string& publicKeyHex) {
    // Get the temporary directory path
    std::string tempDir;

// Mostly dev'd on windows until I get a linux vm setup but may as well try and make this portable
#ifdef _WIN32
    char* tempDirEnvVar = std::getenv("TEMP");
    if (tempDirEnvVar)
        tempDir = tempDirEnvVar;
#else
    char* tempDirEnvVar = std::getenv("TMPDIR");
    if (tempDirEnvVar)
        tempDir = tempDirEnvVar;
#endif

    if (tempDir.empty()) {
        std::cerr << "Error: Unable to get temporary directory path." << std::endl;
        return;
    }

    // Combine the temporary directory with file names
    std::string privateKeyPath = tempDir + "/private_key.txt";
    std::string publicKeyPath = tempDir + "/public_key.txt";

    // Write keys to files in the temporary directory
    std::ofstream privateKeyFile(privateKeyPath);
    if (!privateKeyFile) {
        std::cerr << "Error: Unable to open private key file for writing." << std::endl;
        return;
    }
    privateKeyFile << privateKeyHex;
    privateKeyFile.close();

    std::ofstream publicKeyFile(publicKeyPath);
    if (!publicKeyFile) {
        std::cerr << "Error: Unable to open public key file for writing." << std::endl;
        return;
    }
    publicKeyFile << publicKeyHex;
    publicKeyFile.close();

    std::cout << "Keys dumped to temporary directory: " << tempDir << std::endl;
}


// Public accessible encode message which takes object fields and encodes a ciphertext msg with the generated key
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
    }
    else {
        std::cout << "Encoding the provided message." << std::endl;
        std::cout << "Using passkey: " << userKey << std::endl;
        std::cout << "For   message: " << plainTextMsg
                  << std::endl;

    // Generate ECC keys from the user key
    CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey privateKey;
    CryptoPP::ECIES<CryptoPP::ECP>::PublicKey publicKey;
    GenerateECCKeysFromUserKey();

    // Output the private key
    std::string privateKeyHex;
    CryptoPP::HexEncoder hexPrivateEncoder(new CryptoPP::StringSink(privateKeyHex), false);
    privateKey.Save(hexPrivateEncoder);
    hexPrivateEncoder.MessageEnd();


    // Output the public key
    std::string publicKeyHex;
    CryptoPP::HexEncoder hexPublicEncoder(new CryptoPP::StringSink(publicKeyHex), false);
    publicKey.Save(hexPublicEncoder);
    hexPublicEncoder.MessageEnd();

    TempKeyStorage(privateKeyHex, publicKeyHex);

    cipherText = ECC_EncryptMessage();
    std::string hexCipherText;
    CryptoPP::StringSource(cipherText, true,
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(hexCipherText),
            false // uppercase
        ));


    std::cout << "For the provided key generated the following ciphertext:\n" << hexCipherText << std::endl;

    }
}