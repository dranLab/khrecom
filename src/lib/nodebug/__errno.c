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

/* memcpy/memmove: only register allocation differs (the original moves dst0 out of a0 into t0,
   a0 then holds the loop temporaries); kept asm for now */
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
    char *dst = dst_void;
    const char *src = src_void;
    u_long128 *aligned_dst;
    const u_long128 *aligned_src;

    if (src < dst && dst < src + length) {
        /* Destructive overlap...have to copy backwards */
        src += length;
        dst += length;
        while (length--) {
            *--dst = *--src;
        }
    } else {
        /* Use optimizing algorithm for a non-destructive copy to closely
           match memcpy. If the size is small or either SRC or DST is unaligned,
           then punt into the byte copy loop.  This should be rare.  */
        if (!TOO_SMALL(length) && !UNALIGNED(src, dst)) {
            aligned_dst = (u_long128 *)dst;
            aligned_src = (u_long128 *)src;

            /* Copy 2X quadwords at a time if possible.  */
            while (length >= BIGBLOCKSIZE) {
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                length -= BIGBLOCKSIZE;
            }

            /* Copy one long word at a time if possible.  */
            while (length >= LITTLEBLOCKSIZE) {
                *(long *)aligned_dst = *(long *)aligned_src;
                aligned_dst = (u_long128 *)((long *)aligned_dst + 1);
                aligned_src = (u_long128 *)((long *)aligned_src + 1);
                length -= LITTLEBLOCKSIZE;
            }

            /* Pick up any residual with a byte copier.  */
            dst = (char *)aligned_dst;
            src = (char *)aligned_src;
        }

        while (length--) {
            *dst++ = *src++;
        }
    }

    return dst_void;
}
#endif
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", memcpy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", memmove);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", memset);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcat);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strchr);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcmp);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcpy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strlen);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strncmp);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strncpy);
