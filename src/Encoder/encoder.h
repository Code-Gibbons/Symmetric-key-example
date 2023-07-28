#ifndef ENCODER_H
#define ENCODER_H

#include <string>  // std namespace string not C style string

namespace encoder {
class Encoder {
   private:
    std::string userKey;
    std::string plainTextMsg;
    std::string cipherText;

   public:
    // Constructors
    Encoder();
    Encoder(std::string userKey, std::string userMsg);

    // Setters
    void SetUserKey(std::string userKey);
    void SetPlainTextMsg(std::string userMsg);
    void SetCipherText(std::string cipherMsg);

    // Getters
    std::string GetUserKey();
    std::string GetPlainTextMsg();
    std::string GetCipherText();
};
}  // namespace encoder

#endif  // ENCODER_H