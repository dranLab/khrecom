#!/usr/bin/env python3
"""Place the COMMON symbols like the original linker did (docs/port-step1-shiftable.md S3).

C's uninitialized globals (`FVECTOR CShadowModelMan::m_vCamPos;`, `u_int rpc_buf[8] __attribute__((aligned(64)));`)
are COMMON symbols: the compiler records their size and alignment, the linker places them. The original's linker
put them in one block at the end of .bss, in an order GNU ld cannot reproduce (it allocates COMMON in hash-table
order). So the build places them itself: one tiny object per symbol, in the original order (the manifest, from the
original's symbol addresses), each with the size and alignment that the objects' COMMON records give - the C
declarations decide, not the original's offsets. Linked in place of the block, the definitions take the COMMON
symbols' place.

Checks: every symbol of the manifest must be COMMON in some object; replaying the allocation from the block's
original start must give every symbol its original address (so the C types and alignments are the original's;
the matching build's SHA-1 and _end assert check the link as well).

--allow-missing (a version whose units aren't all compiled yet, e.g. the retail build from assembly): a symbol
that no object declares COMMON keeps splat's size (up to the next symbol) and the alignment of its original
address (at most 128).

Usage (from build.ninja): tools/commons.py --pieces <pieces.txt> --objects <objects.txt> --out-dir <dir>
  pieces.txt: `<object> <symbol> <original address> <splat's size>` in the original order; objects.txt: the other
  link objects.
"""

import argparse
import subprocess
import sys
from pathlib import Path

CROSS = "mips-linux-gnu-"
AS_FLAGS = ["-EL", "-march=r5900", "-mabi=eabi", "-no-pad-sections", "-G0"]


def common_records(objects: list[str]) -> dict[str, tuple[int, int, list[str]]]:
    """symbol -> (size, alignment, objects): the largest of each over all objects that declare it COMMON."""
    records: dict[str, tuple[int, int, list[str]]] = {}
    for obj in objects:
        out = subprocess.run([f"{CROSS}readelf", "-sW", obj], capture_output=True, text=True, check=True).stdout
        for line in out.splitlines():
            p = line.split()
            if len(p) >= 8 and p[6] == "COM":  # for COMMON, readelf's Value column is the alignment
                align, size, name = int(p[1], 16), int(p[2]), p[7]
                old = records.get(name, (0, 0, []))
                records[name] = (max(old[0], size), max(old[1], align), old[2] + [obj])
    return records


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--pieces", required=True)
    parser.add_argument("--objects", required=True)
    parser.add_argument("--out-dir", required=True)
    parser.add_argument("--allow-missing", action="store_true",
                        help="symbols no object declares COMMON keep splat's size and their address's alignment")
    args = parser.parse_args()

    pieces = [line.split() for line in Path(args.pieces).read_text(encoding="utf-8").splitlines() if line.strip()]
    objects = Path(args.objects).read_text(encoding="utf-8").split()
    records = common_records(objects)
    errors, plan = [], []
    addr = int(pieces[0][2], 16)  # the block's original start
    for obj, name, original, splat_size in pieces:
        original = int(original, 16)
        if name in records:
            size, align, owners = records[name]
        elif args.allow_missing:
            size, align, owners = int(splat_size, 16), min(128, original & -original), ["splat's assembly"]
        else:
            errors.append(f"{name}: not COMMON in any object (declare it in its unit's C)")
            continue
        addr = (addr + align - 1) & -align
        if addr != original:
            errors.append(f"{name}: placed at {addr:#x} (size {size:#x}, alignment {align} from "
                          f"{', '.join(Path(o).name for o in owners)}), the original has it at {original:#x}")
            addr = original  # report each difference once
        plan.append((obj, name, size, align))
        addr += size
    if errors:
        for e in errors:
            print(f"error: {e}", file=sys.stderr)
        sys.exit(f"error: the C declarations don't reproduce the original COMMON block ({len(errors)} differences)")

    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    for obj, name, size, align in plan:
        src = Path(obj).with_suffix(".s")
        src.write_text(f"# {name}: COMMON, size {size:#x}, alignment {align} (from the objects) - tools/commons.py\n"
                       f'.section .bss, "wa"\n.globl "{name}"\n.type "{name}", @object\n"{name}":\n'
                       f'.space {size:#x}\n.size "{name}", {size:#x}\n', encoding="utf-8")
        subprocess.run([f"{CROSS}as", *AS_FLAGS, "-o", obj, str(src)], check=True)
        # exactly the symbol's alignment (gas would give every section 16)
        subprocess.run([f"{CROSS}objcopy", "--set-section-alignment", f".bss={align}",
                        "--set-section-alignment", ".text=1", "--set-section-alignment", ".data=1", obj], check=True)
    print(f"{len(plan)} COMMON symbols placed at their original addresses")


if __name__ == "__main__":
    main()
