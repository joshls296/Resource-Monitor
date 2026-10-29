# build.ps1 - Automated build script for Windows C++ projects
param (
    [string]$BuildType = "Release"
)

$ErrorActionPreference = "Stop"

$SourceDir = $PSScriptRoot
$BuildDir = Join-Path $SourceDir "build"
$CMakeCommand = Get-Command cmake -ErrorAction SilentlyContinue
$CMakeExecutable = if ($CMakeCommand) { $CMakeCommand.Source }

if (-not $CMakeCommand) {
    $CMakeCandidates = @(
        (Join-Path $env:ProgramFiles "CMake\bin\cmake.exe")
    )

    foreach ($PathEntry in @(
        [Environment]::GetEnvironmentVariable("Path", "User"),
        [Environment]::GetEnvironmentVariable("Path", "Machine")
    )) {
        foreach ($Directory in ($PathEntry -split ";")) {
            if ($Directory -match "CMake") {
                $CMakeCandidates += Join-Path $Directory "cmake.exe"
            }
        }
    }

    $WinGetPackageRoot = Join-Path $env:LOCALAPPDATA "Microsoft\WinGet\Packages"
    if (Test-Path $WinGetPackageRoot) {
        foreach ($Package in (Get-ChildItem $WinGetPackageRoot -Directory -Filter "Kitware.CMake*")) {
            foreach ($VersionDirectory in (Get-ChildItem $Package.FullName -Directory)) {
                $CMakeCandidates += Join-Path $VersionDirectory.FullName "bin\cmake.exe"
            }
        }
    }

    $CMakePath = $CMakeCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if (-not $CMakePath) {
        throw "CMake was not found. Install CMake from https://cmake.org/download/ or add its bin directory to PATH."
    }

    $CMakeExecutable = $CMakePath
}

Write-Host "Creating build directory..." -ForegroundColor Cyan
if (-not (Test-Path -Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Write-Host "Configuring CMake..." -ForegroundColor Cyan
& $CMakeExecutable -S $SourceDir -B $BuildDir
if ($LASTEXITCODE -ne 0) {
    throw "CMake configuration failed with exit code $LASTEXITCODE."
}

Write-Host "Compiling project ($BuildType)..." -ForegroundColor Cyan
& $CMakeExecutable --build $BuildDir --config $BuildType
if ($LASTEXITCODE -ne 0) {
    throw "Build failed with exit code $LASTEXITCODE."
}

$Executable = Join-Path $BuildDir "$BuildType\WinResourceMonitor.exe"
if (-not (Test-Path -Path $Executable)) {
    $Executable = Join-Path $BuildDir "WinResourceMonitor.exe"
}
if (-not (Test-Path -Path $Executable)) {
    throw "Build completed, but the executable was not found in $BuildDir."
}

Write-Host "Build complete! Executable: $Executable" -ForegroundColor Green