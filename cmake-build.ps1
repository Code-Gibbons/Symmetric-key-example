# Delete the dir if it exists don't want cmake caching stuff
$buildDir = "build"

Write-Host "Checking for old build."
if (Test-Path -Path $buildDir -PathType Container) {
    Remove-Item -Path $buildDir -Force -Recurse
    Write-Host "Deleted prior build run."
}

# Call CMake build with MinGW
$cmakeCommand = "cmake -S . -B build -G ""MinGW Makefiles"""
Write-Host "Generate makefiles for ming32-make."
Invoke-Expression $cmakeCommand

# Switch dirs
Write-Host "Switch to makefile dir."
Push-Location -Path $buildDir

# Call Makefile
$makeCommand = "mingw32-make"
Write-Host "Running makefile."
Invoke-Expression $makeCommand

# Go back up
Pop-Location