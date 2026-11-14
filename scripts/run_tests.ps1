# DSA Core C++17 - Automated Test Suite Runner (Windows PowerShell)
$ErrorActionPreference = "Stop"
Write-Host "=================================================" -ForegroundColor Cyan
Write-Host "       DSA Core C++17 - Unit Test Runner         " -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$rootDir = Split-Path -Parent $scriptDir
Set-Location $rootDir

Write-Host "[Build] Compiling all test suites with make..." -ForegroundColor Yellow
make test

Write-Host "`n>>> All unit test suites executed successfully! <<<" -ForegroundColor Green
