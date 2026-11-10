/* Checks for the matching link only (configure.py adds it there; the shifted links of shift.yaml leave it out).
 * They hold because the image is the original's: the SHA-1 already implies them, these say what broke. */
ASSERT(_end == 0x006A0EF8, "_end (end of .bss) is not the original 0x006A0EF8");
