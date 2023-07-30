#include "decoder.h"

#include <cstdlib>      // For temporary directory handling
#include <fstream>      // output file streams
#include <files.h>      // Making files

#include <eccrypto.h>   // CryptoPP::DL_GroupParameters_EC
#include <osrng.h>      // CryptoPP::AutoSeededRandomPool
#include <hex.h>        // CryptoPP::HexEncoder
#include <pwdbased.h>   // CryptoPP::PKCS5_PBKDF2_HMAC
#include <asn.h>        // CryptoPP::ASN1::secp256r1
#include <oids.h>       // CryptoPP::ASN1

/****************************************************************************
 * Constructors
 ***************************************************************************/
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

/****************************************************************************
 * Setters
 ***************************************************************************/

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


/****************************************************************************
 * Getters
 ***************************************************************************/
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


// Function to read keys from the temporary directory
bool ReadKeysFromTemporaryDirectory(std::string& privateKeyHex, std::string& publicKeyHex) {
    // Get the temporary directory path
    std::string tempDir;
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
        return false;
    }

    // Combine the temporary directory with file names
    std::string privateKeyPath = tempDir + "/private_key.txt";
    std::string publicKeyPath = tempDir + "/public_key.txt";

    // Read keys from files in the temporary directory
    std::ifstream privateKeyFile(privateKeyPath);
    if (!privateKeyFile) {
        std::cerr << "Error: Unable to open private key file for reading." << std::endl;
        return false;
    }
    privateKeyFile >> privateKeyHex;
    privateKeyFile.close();

    // TODO DEBUG REMOVE THIS
    std::cout << "Loaded private key hex: " << privateKeyHex << std::endl;

    std::ifstream publicKeyFile(publicKeyPath);
    if (!publicKeyFile) {
        std::cerr << "Error: Unable to open public key file for reading." << std::endl;
        return false;
    }
    publicKeyFile >> publicKeyHex;
    publicKeyFile.close();

    // Delete the temporary files
    std::remove(privateKeyPath.c_str());
    std::remove(publicKeyPath.c_str());

    return true;
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
        std::cout << "Decoding the provided message." << std::endl;
        std::cout << "Using passkey : " << userKey << std::endl;
        std::cout << "For ciphertext: " << cipherText
                  << std::endl;

    // We need the keys generated earlier so grab them
    std::string privateKeyHex, publicKeyHex;
    if (!ReadKeysFromTemporaryDirectory(privateKeyHex, publicKeyHex)) {
        std::cerr << "Error: Unable to read keys from temporary directory." << std::endl;
        return;
    }

    // Create a CryptoPP::StringSource to decode the hex-encoded private key
    CryptoPP::HexDecoder privateKeyDecoder;
    privateKeyDecoder.Put((const CryptoPP::byte*)privateKeyHex.data(), privateKeyHex.size());
    privateKeyDecoder.MessageEnd();

    privateKey.Load(privateKeyDecoder);

    // Create a CryptoPP::StringSource to decode the hex-encoded public key
    CryptoPP::HexDecoder publicKeyDecoder;
    publicKeyDecoder.Put((const CryptoPP::byte*)publicKeyHex.data(), publicKeyHex.size());
    publicKeyDecoder.MessageEnd();

    publicKey.Load(publicKeyDecoder);

    plainTextMsg = ECC_DecryptMessage();

    }
}