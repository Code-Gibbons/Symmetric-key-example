# Symmetric Key Example Program

## Project Details
This project consists of two C++ CLI programs
 1. The encoder takes a user provided passkey and creates an encrypted message from the provided key.
    - The encoder will pass the ciphertext to the decoder application via a method of my choosing.
 2. The decoder takes a user provided passkey and attempts to decode the ciphertext generated from the encoder program.
    - If successful the user should see back the original message they encrypted from the encoder
    - If not successful the user will receive an error notification.
## Encoder
### Command Line Arguments
```
-k <key> # The user provided key
-m <message> # The user provided message
```
### Example Call:
`myEncoder -k myKey -m myMessage`

### Example Output
```
Encoding complete a ciphertext has been generated from the provided passkey and message and is now accessible to the decoder.
```

## Decoder
### Command Line Arguments:
```
-k <key> # The user provided key
```
### Example Call:
`myDecoder -k myKey`

### Example Output:
#### Failure:
```
Decoder Error: The key provided doesn't match the key for the given ciphertext. Please verify input key.
```

#### Success:
```
The original message entered for the provided key was myMessage.
```

## Tools and Libraries
 - C++
 - CMake
 - Crypto++
 - Visual Studio Community
 - VSCode
