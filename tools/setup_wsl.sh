#!/usr/bin/env bash
# One-time setup of the build environment on Ubuntu/Debian (incl. WSL).
# Run from the repo root:  bash tools/setup_wsl.sh
set -euo pipefail

cd "$(dirname "$0")/.."

echo "== System packages (needs sudo) =="
# The PS2 compilers from decomp.me are 32-bit x86 Linux binaries -> need the i386 libc.
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install -y \
    binutils-mips-linux-gnu \
    ninja-build \
    python3-venv python3-pip \
    libc6:i386 \
    git curl xz-utils

echo "== Python virtualenv (.venv) =="
python3 -m venv .venv
.venv/bin/pip install --upgrade pip
.venv/bin/pip install -r requirements.txt

echo
echo "Done. Activate the environment with:  source .venv/bin/activate"
