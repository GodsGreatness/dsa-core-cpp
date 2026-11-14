#!/usr/bin/env bash
# DSA Core C++17 - Automated Test Suite Runner (Linux / macOS)
set -e

echo "================================================="
echo "       DSA Core C++17 - Unit Test Runner         "
echo "================================================="

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
cd "$ROOT_DIR"

echo "[Build] Compiling and running all test suites with make..."
make test

echo -e "\n>>> All unit test suites executed successfully! <<<"
