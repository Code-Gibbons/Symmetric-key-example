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
 - MingGW
 - VSCode
 - LLVM (Clang)
 - Google C++ Style Guide

 ## Building
 1. To build you will need CMake installed. Presently this has only been tested on windows
   - In theory it's portable to other platforms but I don't have an home linux setup and I haven't worked on the shell script since the start
 2. Clone the project ideally somewhere you have read write execute permissions.
 3. Ensure you have powershell unsigned script execution enabled and run one of the two build commands
  - `./cmake-build.ps1 encode`
  - `./cmake-build.ps1 decode`\
  Presently there are two separate programs being built but the CMakeList.txt will handle either as long as the argument is passed in. For that we can use powershell script to pass in the input to CMake.
4. At this point you should be able to run the program like the prompt above!

## Known Issues
 - Due to windows file handles if you have any of the temp files open during either encode or decode run you may get undefined behavior typically resulting in a failure.
   - This has been observed as occasional behavior with the OS sometime not sure if it's also related to starting up processes to closely together.
 - This isn't as well understood but it seems under some circumstances you can provide a key that is too long for the decoder to handle correctly.\
  This has been fixed by increasing the size of the derivation key used and hasn't been observed again.\
  As a result of this the encode is slightly longer than expected but no runtime above 20s has been observed yet.

## Potential issues
 - This is presently very windows centric, if a build script were ported from powershell to bash in theory this should also work as the directory stuff should be handled, but this hasn't been tested.\
   Additionally I haven't built in another OS so it's hard to say if Crypto++ or it's cmake build function the exact same