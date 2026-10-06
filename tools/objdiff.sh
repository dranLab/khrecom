#!/usr/bin/env bash
# Run objdiff-cli (downloaded to tools/objdiff/ on first use) from the repo root.
# objdiff.json is written by configure.py.
#
#   tools/objdiff.sh diff -u System/PlaySpeedCtrl   # interactive diff, rebuilds when sources change
#   tools/objdiff.sh report generate -o build/report.json
set -euo pipefail
cd "$(dirname "$0")/.."

version=v3.8.2
bin=tools/objdiff/objdiff-cli-$version
if [[ ! -x "$bin" ]]; then
    mkdir -p tools/objdiff
    echo "downloading objdiff-cli $version" >&2
    curl -fsSL -o "$bin" "https://github.com/encounter/objdiff/releases/download/$version/objdiff-cli-linux-x86_64"
    chmod +x "$bin"
fi
exec "$bin" "$@"
