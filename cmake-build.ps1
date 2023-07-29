# Get the script's directory and set it as the working directory
$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptPath

$target = $args[0]

# Set the build directory based on the target argument
$buildDir = if ($target -eq "decode") {
    "build_decode"
}
elseif ($target -eq "encode") {
    "build_encode"
}
else {
    Write-Host("No valid build target provided. Exiting build script. Please pass either encode or decode.")
    exit
}

# Common build directory for shared memory
$commonBuildDir = "shared_resources"
$sharedMemoryName = "CipherTextMemory"

Write-Host "Checking for old build."
if (Test-Path -Path $buildDir -PathType Container) {
    Remove-Item -Path $buildDir -Force -Recurse
    Write-Host "Deleted prior build run."
}

# Create the build directory if it does not exist
if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

Set-Location $scriptPath
Write-Host "Current directory: $(Get-Location)"


# Call CMake build with MinGW
$cmakeCommand = if ($target -eq "decode") {
    "cmake -S . -B $buildDir -G ""MinGW Makefiles"" -DTARGET_TYPE=decode"
}
else {
    "cmake -S . -B $buildDir -G ""MinGW Makefiles"" -DTARGET_TYPE=encode"
}

# $cmakeCommand = "cmake -S . -B $buildDir -G ""MinGW Makefiles"" -DTARGET_TYPE=encode"
Write-Host "Generate makefiles for ming32-make."
Invoke-Expression $cmakeCommand

# Switch dirs
Write-Host "Switch to makefile dir."
Push-Location -Path $buildDir
Write-Host($buildDir)

# Call Makefile
$makeCommand = "mingw32-make"
Write-Host "Running makefile."
Invoke-Expression $makeCommand

# Go back up
Pop-Location

# Set shared memory name in the environment variable to be accessed by both programs dunno if I like this but let's see
$env:SHARED_MEMORY_NAME = $sharedMemoryName