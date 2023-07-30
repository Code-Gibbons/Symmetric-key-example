#include "encoder.h"

#include <iostream>     // cin, cout
#include <cstdlib>      // For temporary directory handling
#include <files.h>      // output file streams

#include <eccrypto.h>   // CryptoPP::DL_GroupParameters_EC
#include <osrng.h>      // CryptoPP::AutoSeededRandomPool
#include <hex.h>        // CryptoPP::HexEncoder
#include <pwdbased.h>   // CryptoPP::PKCS5_PBKDF2_HMAC
#include <asn.h>        // CryptoPP::ASN1::secp256r1
#include <oids.h>       // CryptoPP::ASN1
#include <filters.h>    // CryptoPP::StringSource and PK_EncryptorFilter


/****************************************************************************
 * Private Vars Both arbitrarily chosen values
 ***************************************************************************/
const int unsigned SIZE_BYTES_DERIVED_KEY   = 32;
const unsigned int NUM_PBKDF2_ITERATIONS    = 10000;

/****************************************************************************
 * Constructors
 ***************************************************************************/
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
    encoder::Encoder::cipherText = "";
    encoder::Encoder::publicKey;
    encoder::Encoder::privateKey;
}

/****************************************************************************
 * Setters
 ***************************************************************************/

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


/****************************************************************************
* Getters
 ***************************************************************************/
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

/****************************************************************************
 * Generate the key pair using generator PBKDF2 and curve secp256r1
 ***************************************************************************/
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

    // Init Crypto++ params and initializes a key set using curve
    CryptoPP::DL_GroupParameters_EC<CryptoPP::ECP> params;
    params.Initialize(CryptoPP::ASN1::secp256r1());
    privateKey.Initialize(prng, params);
    privateKey.MakePublicKey(publicKey);
}

/****************************************************************************
 * Encrypt the message using the public key we just generated
 * @param const const CryptoPP::ECIES<CryptoPP::ECP>::PublicKey& a reference
 * to the public key we use to encrypt the message
 * @return std::string the resultant ciphertext from the encode
 ***************************************************************************/

    std::string encoder::Encoder::ECC_EncryptMessage(const CryptoPP::ECIES<CryptoPP::ECP>::PublicKey& publicKey) {
    std::string cipherText;

    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::ECIES<CryptoPP::ECP>::Encryptor encryptor(publicKey);
    CryptoPP::StringSource(plainTextMsg, true, new CryptoPP::PK_EncryptorFilter(prng, encryptor,
    new CryptoPP::StringSink(cipherText)));

    return cipherText;
}

/****************************************************************************
 *  Store the generated ciphertext and public & private keys in temp dir
 * @param const std::string& reference to the private key hexstring
 * @param const std::string& reference to the public key hexstring
 ***************************************************************************/
void TempKeyStorage(const std::string& privateKeyHex, const std::string& publicKeyHex) {

    std::string tempDir;
// Ideally this let's us be platform independent
#ifdef _WIN32
    char* tempDirEnvVar = std::getenv("TEMP");
    if (tempDirEnvVar)
        tempDir = tempDirEnvVar;
#else
    // This part has yet to be verified
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

#ifdef DEBUG
    std::cout << "Keys dumped to temporary directory: " << tempDir << std::endl;
#endif
}


/****************************************************************************
 * Public accessible encode message which uses the object's fields and
 * creates a private & public key and then a ciphertext for the given
 * plaintext message and user keyphrase
 ***************************************************************************/
void encoder::Encoder::EncodeMessage() {
    if (userKey.empty()) {
        std::cerr << "Error: No key was provided unable to continue encode." << std::endl;
        return;
    }

    if (plainTextMsg.empty()) {
        std::cerr
            << "Error: No message was provided cannot encode empty message."
            << std::endl;
        return;
    }
    else {
        std::cout << "Encoding the provided message." << std::endl;
        std::cout << "Using passkey: " << userKey << std::endl;
        std::cout << "For   message: " << plainTextMsg
                  << std::endl;

    GenerateECCKeysFromUserKey();

    // Transform both keys into hex representation to store them in temp data
    std::string privateKeyHex;
    CryptoPP::HexEncoder hexPrivateEncoder(new CryptoPP::StringSink(privateKeyHex), false);
    privateKey.Save(hexPrivateEncoder);
    hexPrivateEncoder.MessageEnd();

    std::string publicKeyHex;
    CryptoPP::HexEncoder hexPublicEncoder(new CryptoPP::StringSink(publicKeyHex), false);
    publicKey.Save(hexPublicEncoder);
    hexPublicEncoder.MessageEnd();

    TempKeyStorage(privateKeyHex, publicKeyHex);

    cipherText = ECC_EncryptMessage(publicKey);

#ifdef DEBUG
    std::string hexCipherText;
    CryptoPP::StringSource(cipherText, true,
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(hexCipherText),
            false
        ));
    std::cout << "For the provided key generated the following ciphertext:\n" << hexCipherText << std::endl;
#endif
    }
}