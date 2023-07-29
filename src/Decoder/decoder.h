#ifndef DECODER_H
#define DECODER_H

#include <string>       // std namespace string not C style string
#include <eccrypto.h>   // Public and Private key


// Look at this we could deff do some inheritance if I have time I'd like to make both of these inherit from some class
namespace decoder {
class Decoder {
   private:
    std::string userKey;
    std::string plainTextMsg;
    std::string cipherText;
    CryptoPP::ECIES<CryptoPP::ECP>::PrivateKey privateKey;
    CryptoPP::ECIES<CryptoPP::ECP>::PublicKey publicKey;

    void GenerateECCKeysFromUserKey(void);
    std::string ECC_DecryptMessage(void);


   public:
    // Constructors
    Decoder();
    Decoder(std::string userKey, std::string cipherText);

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

    void DecodeMessage(void);
};
}  // namespace encoder

#endif  // ENCODER_H