# iso/

Put your own dump of the game disc here (for example `iso/khrecom.iso`).
Everything in this folder except this README is gitignored. **Never commit game data.**

`tools/extract_iso.py` writes the boot ELF (for example `iso/SLUS_217.99`) and
`SYSTEM.CNF` next to the image.

Several discs may sit here (the beta `khrecom.iso`, the retail `khrecom_retail.iso`): `tools/build_iso.py` picks
the one whose boot ELF is the version's original ELF. Both boot `SLUS_217.99`, so extract the retail one with
`tools/extract_iso.py iso/khrecom_retail.iso --name SLUS_217.99_retail` (also writes
`SLUS_217.99_retail.SYSTEM.CNF`); the script never overwrites a file with other contents.
