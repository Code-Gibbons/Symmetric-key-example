# CMake build system generation for symmetric key example project

# Lowest cmake version I feel am familar with using 3 in general should be fine do not allow < 3.x
cmake_minimum_required(VERSION 3.17)

project(
    symmetric-key-project
    VERSION 0.1
    LANGUAGES CXX
)

# Only do this for the main project not any subprojects
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Let's ensure -std=c++xx instead of -std=g++xx
    # A toolchain file can be added later for cross platform builds for now target windows native
    set(CMAKE_CXX_EXTENSIONS ON)
    set(CMAKE_CXX_COMPILER_ID GNU)

    # Consider some small doxygen packages been wanting to learn how to work with those for awhile
    # Find the package and create a docs folder, commented out now for basic impl and testing first
    # find_package(Doxygen)
    # if(Doxygen_FOUND )
    # add_subdirectory(docs)
    # endif()
endif()

# Rather than making a CMakeList for the submodule let's fetch it and make things easier
message("Attempting to fetch Crypto++ Library")

include(FetchContent)

# Go figure I pick a library the doesn't support CMake this repo seems to attempt to fix that issue let's try
FetchContent_Declare(
    cryptopp-cmake
    GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git
    GIT_TAG f857b775bcb4ff24e4993d85cce811587f8b0616 # CMake 8.8 release
)

FetchContent_MakeAvailable(cryptopp-cmake)
FetchContent_GetProperties(cryptopp-cmake)

# message("All Libraries are fetched and avaliable linking to the executable.")
add_executable(${PROJECT_NAME}
    src/Encoder/main.cpp
    src/Encoder/encoder.cpp
)

# target_link_directories(${PROJECT_NAME} PRIVATE ${cryptopp_BINARY_DIR})
# target_include_directories(${PROJECT_NAME} PRIVATE ${cryptopp_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE cryptopp)
