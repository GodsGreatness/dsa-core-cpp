#!/usr/bin/env bash
# DSA Core C++17 - Empirical Benchmark Runner (Linux / macOS)
set -e

echo "================================================="
echo "     DSA Core C++17 - Empirical Benchmarks       "
echo "================================================="

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
cd "$ROOT_DIR"

echo "[Build] Compiling and executing benchmarks..."
make benchmark

echo -e "\n>>> All benchmarks completed successfully! <<<"
