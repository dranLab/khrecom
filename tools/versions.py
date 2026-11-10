"""The game versions of this repo (config/versions.yaml) and the one a tool works on by default.

config/versions.yaml lists the versions configure.py builds and tools/check.sh checks, the reference first (the
US translation beta). Tools without an explicit --version use KH_VERSION from the environment, else the first.
"""

import os
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent


def versions() -> list[str]:
    path = ROOT / "config" / "versions.yaml"
    listed = yaml.safe_load(path.read_text(encoding="utf-8")) or []
    for v in listed:
        if not (ROOT / "config" / v / f"{v}.yaml").exists():
            sys.exit(f"error: {path.relative_to(ROOT)} lists {v}, but config/{v}/{v}.yaml doesn't exist")
    return listed


def default_version(requested: str | None = None) -> str:
    """The version a tool works on: the requested one, KH_VERSION, or the reference (the first listed)."""
    version = requested or os.environ.get("KH_VERSION") or versions()[0]
    if not (ROOT / "config" / version / f"{version}.yaml").exists():
        sys.exit(f"error: unknown version {version!r}; configured: {versions()}")
    return version
