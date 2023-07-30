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
It should be noted that you can pass the arguments without quoting `""` but this will truncate the input at the first space.\
If you want to use space delimitated messages you must wrap them like below:\
`myEncoder -k "my secret key" -m "my secret message"`

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
It should be noted that you can pass the arguments without quoting `""` but this will truncate the input at the first space.\
If you want to use space delimitated messages you must wrap them like below:\
`myDecoder -k "my secret key"`

### Example Output:
#### Failure:
```
Decoder Error: missing the key, cannot decode ciphertext.
```
or
```
Decoder Error: missing the ciphertext, key has nothing to decode.
```

#### Success:
```
Using the key: my secret key. Decoded the following message: my secret message
```

## Tools and Libraries
 - C++
 - CMake
 - Crypto++
 - VSCode
 - LLVM (Clang)
 - Google C++ Style Guide
