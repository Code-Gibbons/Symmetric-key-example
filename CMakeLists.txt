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

# We could use submodules or try fetch content looks neat but play with it later stick with knowns for now
# include(FetchContent)
# FetchContent_Declare(
# cryptopp
# GIT_REPOSITORY https://github.com/weidai11/cryptopp.git
# GIT_TAG        4d0cad5401d1a2c998b314bc89288c9620d3021d # 8.8 release
# )

# Cleanup naming later for right now NAME.exe is fine
# These are seperate applications so we need to keep them in different CMakeLists
# A top level list is still possible just basically need this is a global project and then
# have a subdirectory(src/Decoder) and subdirectory(src/Encoder) to start lets make the encoder
add_executable(PROJECT_NAME
    src/Encoder/main.cpp
    src/Encoder/encoder.cpp)

# target_link_libraries(PROJECT_NAME.exe PRIVATE cryptopp-shared)