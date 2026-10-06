#!/usr/bin/env bash
# Prepare a git worktree of this repo for building: link the large untracked parts from the main checkout
# (game files in iso/, compilers, wibo, ccc, objdiff, ref/, .venv) and run configure.py.
#
# Run it from inside the worktree, e.g. one a sub-agent got from Claude Code:
#   tools/setup_worktree.sh               # link + configure (~30 s)
#   tools/setup_worktree.sh --no-configure
set -euo pipefail
cd "$(git rev-parse --show-toplevel)"

here=$(pwd -P)
main=$(git worktree list --porcelain | awk '/^worktree / {print $2; exit}')
main=$(cd "$main" && pwd -P)
if [[ "$here" == "$main" ]]; then
    echo "this is the main checkout, nothing to link"
else
    link() {  # link <path>: symlink the main checkout's untracked file/dir at <path> into this worktree
        if [[ -e "$main/$1" && ! -e "$1" && ! -L "$1" ]]; then
            mkdir -p "$(dirname "$1")"
            ln -s "$main/$1" "$1"
            echo "linked $1"
        fi
    }
    for f in "$main"/iso/*; do
        [[ "$(basename "$f")" == "README.md" ]] || link "iso/$(basename "$f")"
    done
    for d in tools/compilers tools/wibo tools/ccc tools/objdiff ref .venv; do
        link "$d"
    done
fi

if [[ "${1:-}" != "--no-configure" ]]; then
    .venv/bin/python configure.py > build.configure.log 2>&1 || { tail -20 build.configure.log; exit 1; }
    tail -1 build.configure.log
fi
