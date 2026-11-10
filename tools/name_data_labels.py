#!/usr/bin/env python3
"""Give a compiled object's data the original's label names (docs/port-step1-shiftable.md, S3).

A unit whose data comes from source emits the original's bytes, but only under the compiler's names: a string
literal is `$LC12`, a static `name.123`, a pooled constant nothing at all. INCLUDE_ASM'd code and splat's data
objects still use the original's address names for them (`D_004E26D8`), which the matching link resolves with
original_labels.ld's PROVIDEs, at the original address. A shifted link has no such pins.

configure.py writes, per compiled object, `<object>.labels`: every address-named label of the original that lies
in one of the object's data sections from source, as `section offset name` (offset = label address - the
section's original start). The object's section *is* that range of the original (the matching build's SHA-1
checks it), so the offset is exact; this adds each label as a global symbol at that offset of its section. It
moves with the section in a shifted link, like any symbol. Names the object already defines globally, and
offsets outside the section (e.g. COMMON data), are left out.

Valid only while the object's data equals the original's: true for every matching unit, checked by the SHA-1 on
every build. A non-matching port must give such data real names in C instead.

Usage (from the cc rule): tools/name_data_labels.py <object> <object>.labels
"""

import subprocess
import sys
from pathlib import Path

CROSS = "mips-linux-gnu-"


def main() -> None:
    obj, labels = Path(sys.argv[1]), Path(sys.argv[2])
    sizes = {}
    out = subprocess.run([f"{CROSS}readelf", "-SW", str(obj)], capture_output=True, text=True, check=True).stdout
    for line in out.splitlines():
        parts = line.replace("[ ", "[").split()
        if len(parts) > 6 and parts[0].startswith("[") and parts[1].startswith("."):
            sizes[parts[1]] = int(parts[5], 16)
    defined = set()
    out = subprocess.run([f"{CROSS}nm", str(obj)], capture_output=True, text=True, check=True).stdout
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 3 and (parts[1].isupper() or parts[1] in "vw") and parts[1] != "U":
            defined.add(parts[2])

    args = []
    for line in labels.read_text(encoding="utf-8").splitlines():
        section, offset, name = line.split()
        offset = int(offset, 16)
        if name in defined or section not in sizes or offset >= sizes[section]:
            continue
        args.append(f"--add-symbol={name}={section}:0x{offset:X},global,object")
    if not args:
        return
    response = obj.with_suffix(".o.labelargs")
    response.write_text("\n".join(args) + "\n", encoding="utf-8")
    subprocess.run([f"{CROSS}objcopy", f"@{response}", str(obj)], check=True)


if __name__ == "__main__":
    main()
