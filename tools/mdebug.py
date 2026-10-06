"""Minimal reader for the ECOFF symbolic debug info (.mdebug) in PS2 ELFs.

Only what the project's tools need: the file descriptors (one per compilation
unit, in link order) with their source name, procedures and local symbols.
Layouts follow binutils' include/coff/sym.h (32-bit, little-endian).
"""

import struct
from dataclasses import dataclass, field
from pathlib import Path

from elftools.elf.elffile import ELFFile

HDRR = struct.Struct("<hh23i")         # symbolic header, 0x60 bytes
HDRR_FIELDS = (
    "magic vstamp ilineMax cbLine cbLineOffset idnMax cbDnOffset ipdMax cbPdOffset isymMax cbSymOffset "
    "ioptMax cbOptOffset iauxMax cbAuxOffset issMax cbSsOffset issExtMax cbSsExtOffset ifdMax cbFdOffset "
    "crfd cbRfdOffset iextMax cbExtOffset"
).split()
FDR = struct.Struct("<Iiiiiiiiiihhiiiiiii")  # file descriptor, 0x48 bytes
PDR = struct.Struct("<Iiiiiiiiihhiii")  # procedure descriptor, 0x34 bytes
SYMR = struct.Struct("<iII")           # local symbol, 0x0C bytes
MAGIC = 0x7009

# SYMR.st (symbol type) / SYMR.sc (storage class) values used here
ST_GLOBAL, ST_STATIC, ST_PROC, ST_FILE, ST_STATICPROC = 1, 2, 6, 11, 14
SC_TEXT, SC_DATA, SC_BSS, SC_SDATA, SC_SBSS, SC_RDATA = 1, 2, 3, 13, 14, 15


@dataclass
class Symbol:
    name: str
    value: int
    st: int
    sc: int
    index: int


@dataclass
class SourceFile:
    index: int
    name: str
    adr: int
    procs: list[int] = field(default_factory=list)  # procedure addresses
    symbols: list[Symbol] = field(default_factory=list)


def _cstr(data: bytes, offset: int) -> str:
    return data[offset:data.index(b"\0", offset)].decode("latin-1")


def read_mdebug(elf_path: Path) -> list[SourceFile]:
    data = elf_path.read_bytes()
    with elf_path.open("rb") as f:
        section = ELFFile(f).get_section_by_name(".mdebug")
        if section is None:
            raise ValueError(f"{elf_path} has no .mdebug section")
        base = section["sh_offset"]

    hdr = dict(zip(HDRR_FIELDS, HDRR.unpack_from(data, base)))
    if hdr["magic"] != MAGIC:
        raise ValueError(f"bad .mdebug magic {hdr['magic']:#x}")
    # The offsets are file offsets. The SN linker leaves issBase at 0 and makes rss/iss global string offsets,
    # so adding issBase (as the format intends) still works.
    cb_pd_offset, cb_sym_offset = hdr["cbPdOffset"], hdr["cbSymOffset"]
    cb_ss_offset, ifd_max, cb_fd_offset = hdr["cbSsOffset"], hdr["ifdMax"], hdr["cbFdOffset"]

    files = []
    for i in range(ifd_max):
        (adr, rss, iss_base, _cb_ss, isym_base, csym, _iline_base, _cline, _iopt_base, _copt,
         ipd_first, cpd, *_rest) = FDR.unpack_from(data, cb_fd_offset + i * FDR.size)
        strings = cb_ss_offset + iss_base
        sf = SourceFile(i, _cstr(data, strings + rss) if rss >= 0 else "", adr)
        for p in range(ipd_first, ipd_first + cpd):
            sf.procs.append(PDR.unpack_from(data, cb_pd_offset + p * PDR.size)[0])
        for s in range(isym_base, isym_base + csym):
            iss, value, bits = SYMR.unpack_from(data, cb_sym_offset + s * SYMR.size)
            sf.symbols.append(Symbol(_cstr(data, strings + iss), value, bits & 0x3F, (bits >> 6) & 0x1F, bits >> 12))
        files.append(sf)
    return files
