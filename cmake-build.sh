#!/bin/bash

# Delete the dir if it exists; don't want CMake caching stuff
buildDir="build"

echo "Checking for old build."
if [ -d "$buildDir" ]; then
    rm -rf "$buildDir"
    echo "Deleted prior build run."
fi

# Call CMake build with MinGW
cmakeCommand="cmake -S . -B $buildDir -G 'MinGW Makefiles'"
echo "Generate makefiles for ming32-make."
eval "$cmakeCommand"

# Switch dirs
echo "Switch to makefile dir."
pushd "$buildDir"

# Call Makefile
makeCommand="mingw32-make"
echo "Running makefile."
eval "$makeCommand"

# Go back up
popd