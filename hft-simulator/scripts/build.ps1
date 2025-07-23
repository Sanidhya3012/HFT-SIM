# PowerShell build script for hft-simulator
$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Split-Path -Parent $scriptPath
$buildPath = Join-Path $projectRoot "build"

# Create and enter build directory
New-Item -ItemType Directory -Force -Path $buildPath
Set-Location $buildPath

# Run CMake
cmake $projectRoot
cmake --build . 