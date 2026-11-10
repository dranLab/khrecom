/* System/Graphic/Effect/libppp/draw/global_w.c: the draw library's matrices and scratchpad indices (a data-only
   unit). */
#include "global_w.h"

/* The unit's .bss, in this order (initialized into .bss so that gcc emits them in definition order, as in
   pppPart.c); spr_index and spr_rev_index are .sbss in the original, spr_a and spr_rev_a COMMON (the linker
   placed them in its COMMON block, configure.py split_common). */
sceVu0FMATRIX os_mat_lw __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX os_mat_wv __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX os_mat_vs __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX os_mat_ws __attribute__((section(".bss"))) = {0};
sceVu0FMATRIX os_mat_ls __attribute__((section(".bss"))) = {0};
word spr_index;
word spr_a[32];
sceVu0FMATRIX os_mat_unit[4] __attribute__((section(".bss"))) = {0};
word spr_rev_index;
word spr_rev_a[32];
// SOURCE DATA: .bss
