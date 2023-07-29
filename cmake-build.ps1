param (
    [string]$target = "encode"
)

# Set the build directory based on the target argument
$buildDir = if ($target -eq "decode") {
    "build_decode"
}
else {
    "build_encode"
}

# Create the build directory if it does not exist
if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Change to the build directory
Set-Location $buildDir

# Run the CMake command to configure and generate the build files
cmake .. -DCMAKE_BUILD_TYPE=Release -DTARGET_TYPE=$target

# Build the project
cmake --build .

# Change back to the original directory
Set-Location ..