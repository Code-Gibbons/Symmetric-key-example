#ifndef ENCODER_H
#define ENCODER_H

#include <string>       // std namespace string not C style string
#include <eccrypto.h>   // Public and Private key
namespace encoder {
class Encoder {
   private:
    std::string userKey;
    std::string plainTextMsg;
    std::string cipherText;
    CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey privateKey;
    CryptoPP::ECIES<CryptoPP::ECP>::PublicKey publicKey;

    void GenerateECCKeysFromUserKey(void);
    std::string ECC_EncryptMessage(const CryptoPP::ECIES<CryptoPP::ECP>::PublicKey& publicKey);


   public:
    // Constructors
    Encoder();
    Encoder(std::string userKey, std::string userMsg);

    // Setters
    void SetUserKey(std::string userKey);
    void SetPlainTextMsg(std::string userMsg);
    void SetCipherText(std::string cipherMsg);
    void SetPublicKey(CryptoPP::ECIES<CryptoPP::ECP>::PublicKey publicKey);
    void SetPrivateKey(CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey privateKey);

    // Getters
    std::string GetUserKey(void);
    std::string GetPlainTextMsg(void);
    std::string GetCipherText(void);
    CryptoPP::ECIES<CryptoPP::ECP>::PublicKey GetPublicKey(void);
    CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey GetPrivateKey(void);

    void EncodeMessage(void);
};
}  // namespace encoder

#endif  // ENCODER_H