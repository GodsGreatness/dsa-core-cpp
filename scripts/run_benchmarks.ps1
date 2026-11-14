# DSA Core C++17 - Empirical Benchmark Runner (Windows PowerShell)
$ErrorActionPreference = "Stop"
Write-Host "=================================================" -ForegroundColor Cyan
Write-Host "     DSA Core C++17 - Empirical Benchmarks       " -ForegroundColor Cyan
Write-Host "=================================================" -ForegroundColor Cyan

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$rootDir = Split-Path -Parent $scriptDir
Set-Location $rootDir

Write-Host "[Build] Compiling and executing benchmarks..." -ForegroundColor Yellow
make benchmark

Write-Host "`n>>> All benchmarks completed successfully! <<<" -ForegroundColor Green
