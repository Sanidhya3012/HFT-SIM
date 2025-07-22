# PowerShell build script for hft-simulator
New-Item -ItemType Directory -Force -Path ../build
Set-Location ../build
cmake ..
cmake --build . 