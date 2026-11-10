/* System/Graphic/Effect/libppp/draw/draw_w.c: the draw library's work globals (a data-only unit). */
#include "draw_w.h"

typedef u_long128 u_qword; /* the debug info's 16-byte "long long unsigned int" */

word ot_beki[16] = {
    0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000,
};

/* COMMON in the original: the linker placed them in its COMMON block (configure.py split_common). */
u_qword ow_dtag_mdv1[32];
u_qword ow_dtag_mdv1n[32];
word ow_mpg_qsize[32];
u_qword ow_dtag_shapev[16];

/* The unit's other globals (os_gs2_register, os_gs2_register2, os_debug_draw, ogs_loadimage, ogs_storeimage,
   ow_gs_zbuf, ow_gs_zbuf2: its .bss) still come from the original's assembly: a 0x200-byte gap follows them that
   no symbol explains. The oa_dma* channel pointers are .sbss. */
// SOURCE DATA: .data
