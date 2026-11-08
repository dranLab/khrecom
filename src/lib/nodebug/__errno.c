/* newlib libc/reent/errno.c and SCE's 128-bit string/memory functions (newlib memcpy.c, memmove.c, ...) */
#include "include_asm.h"

typedef unsigned int size_t;
typedef int u_long128 __attribute__((mode(TI)));

struct _reent {
    int _errno;
};
extern struct _reent *_impure_ptr;
#define _REENT _impure_ptr

/* Nonzero if either X or Y is not aligned on a quadword boundary.  */
#define UNALIGNED(X, Y) (((long)X | (long)Y) & (sizeof(u_long128) - 1))
/* How many bytes are copied each iteration of the 2X unrolled loop.  */
#define BIGBLOCKSIZE (sizeof(u_long128) << 1)
/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof(long))
/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN) ((LEN) < BIGBLOCKSIZE)

int *__errno(void)
{
    return &_REENT->_errno;
}

/* Parked: memcpy 11 of 43 lines, memmove 31 of 68 (2026-11-08): registers only, one cause in both: the original
   keeps the first parameter in t0, gives the quadword loop lq v1/sltiu a0/lq v0 (ours v0/v1/v0) and copies the byte
   loop's -1 from the entry test's register (li v0,-1; beq; move a0,v0; ours li a3,-1). gcc 2.95.3 can't produce either
   from this RTL: local-alloc's 3 qtys give the first lq v0 (its fake lifetime ends before the second lq is born, sq1 ->
   lq2 is a memory dependence), and no pass copies a cheap CONST_INT from another register (cse_set_around_loop and
   reload_cse need a costlier source); no other game function has that copy: maybe edited compiler output like the MMI
   neighbours. memmove: newlib's int len = length, declared first (38 -> 31); tried: quadword loop as while/do/for,
   statement order, temporaries, long/u_long128 pointers, byte loop forms, len int/size_t/long/int parameter, all 120
   declaration orders, dst0 references, every installed compiler and -f flag (diagnosis only), decomp_permuter 15 min
   each (memcpy 85k iterations): no gain */
#if 0
void *memcpy(void *dst0, const void *src0, size_t len0)
{
    char *dst = dst0;
    const char *src = src0;
    u_long128 *aligned_dst;
    const u_long128 *aligned_src;
    int len = len0;

    /* If the size is small, or either SRC or DST is unaligned,
       then punt into the byte copy loop.  This should be rare.  */
    if (!TOO_SMALL(len) && !UNALIGNED(src, dst)) {
        aligned_dst = (u_long128 *)dst;
        aligned_src = (u_long128 *)src;

        /* Copy 2X quadwords at a time if possible.  */
        while (len >= BIGBLOCKSIZE) {
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            len -= BIGBLOCKSIZE;
        }

        /* Copy one long word at a time if possible.  */
        while (len >= LITTLEBLOCKSIZE) {
            *(long *)aligned_dst = *(long *)aligned_src;
            aligned_dst = (u_long128 *)((long *)aligned_dst + 1);
            aligned_src = (u_long128 *)((long *)aligned_src + 1);
            len -= LITTLEBLOCKSIZE;
        }

        /* Pick up any residual with a byte copier.  */
        dst = (char *)aligned_dst;
        src = (char *)aligned_src;
    }

    while (len--)
        *dst++ = *src++;

    return dst0;
}

void *memmove(void *dst_void, const void *src_void, size_t length)
{
    int len = length;
    char *dst = dst_void;
    const char *src = src_void;
    u_long128 *aligned_dst;
    const u_long128 *aligned_src;

    if (src < dst && dst < src + len) {
        /* Destructive overlap...have to copy backwards */
        src += len;
        dst += len;
        while (len--) {
            *--dst = *--src;
        }
    } else {
        /* Use optimizing algorithm for a non-destructive copy to closely
           match memcpy. If the size is small or either SRC or DST is unaligned,
           then punt into the byte copy loop.  This should be rare.  */
        if (!TOO_SMALL(len) && !UNALIGNED(src, dst)) {
            aligned_dst = (u_long128 *)dst;
            aligned_src = (u_long128 *)src;

            /* Copy 2X quadwords at a time if possible.  */
            while (len >= BIGBLOCKSIZE) {
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                len -= BIGBLOCKSIZE;
            }

            /* Copy one long word at a time if possible.  */
            while (len >= LITTLEBLOCKSIZE) {
                *(long *)aligned_dst = *(long *)aligned_src;
                aligned_dst = (u_long128 *)((long *)aligned_dst + 1);
                aligned_src = (u_long128 *)((long *)aligned_src + 1);
                len -= LITTLEBLOCKSIZE;
            }

            /* Pick up any residual with a byte copier.  */
            dst = (char *)aligned_dst;
            src = (char *)aligned_src;
        }

        while (len--) {
            *dst++ = *src++;
        }
    }

    return dst_void;
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", memcpy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", memmove);
/* Hand-written: MMI string routines (128-bit loads and compares) */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", memset);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcat);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strchr);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcmp);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcpy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strlen);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strncmp);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strncpy);
