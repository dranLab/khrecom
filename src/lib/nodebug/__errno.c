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
   each (memcpy 85k iterations): no gain. 2026-11-09 (unchanged, 11/31): the quadword loop needs lq2 issued by sched1
   right after sq1 (local-alloc's fake lifetime then keeps lq1 off lq2's v0, sltiu gets a0, dst0 moves to t0), but the
   sq1 -> lq2 memory dependence costs 2 cycles, so sched1 puts dst += 16 between, as the original's final order does:
   sched1 and sched2 would need different dependence graphs. aligned_dst[0]/[1] + aligned_dst += 2 gives the t0/a0
   part (lq regs swapped, sq 16(a3): 9 lines but 176 bytes, a different shape). The -1 copy has the shape of
   cse_set_around_loop's copy into the REG_LOOP_TEST_P register (cse.c ~8350, inserted after the entry test's set),
   which needs an in-loop source costlier than a pseudo. Also tried: do {} while (0) barriers, explicit/shared/scoped
   temps, index and pointer-increment forms, an index loop, alias-set changes (strict aliasing is off), 10 compilers x
   30 flags for the copy: no gain. 2026-11-10 (unchanged, 11/31): every pass that can turn a constant into a register
   copy refuses a cheap CONST_INT (cse_insn and cse_set_around_loop by cost, reload_cse_simplify_set/_operands by
   rtx_cost <= reg), and our cse2 sees the entry test's -1 and the hoisted -1 in one extended block: the copy needs
   other RTL, not another order; early mentions (len/dst assigned after the declarations, aligned_dst/src
   initialised), 10 byte-loop forms (!= 0, > 0, for, index, --len >= 0, if/do, end pointer, != -1), size_t/long len,
   -Os/-O1/-O3/-mips3/-mgp32/-mlong32: no gain */
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
#ifndef KH_RETAIL /* memmove is not in the retail ELF (nothing calls it there) */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", memmove);
#endif
/* Parked: memset 12 of 47 lines, same size (2026-11-08): registers only, in the fill-pattern block: the original
   has andi t1 / move t0,t1 / dsll v1,t0,8 / or t0,v1,t1 / pcpyh v1,t0 (the masked byte in two registers, a copy this
   C never produces: cse merges every same-mode copy, ours is andi t0 / dsll v0 / or t0 / pcpyh t0,t0), its block
   locals all skip v0 (the n >= 32 test in t2) and the pcpyh/pcpyld results are v1/t0 (ours t0/v1); an empty
   asm("" : "+r") gives the move, but then m moves to t0 (16 lines). Everything else (both loops, size_t while (n--)
   with its lui/ori -1 twice, the short-loop nops) is this compiler's. tried: int/long/unsigned/unsigned char masked
   locals, u_long128 locals (every DI<->TI conversion adds a por), separate pcpyh input/output variables, c &= 0xff
   (then sb stores the masked a1), pcpyld with two operands, before the loop or in an if, decomp_permuter 15 min
   (no gain). 2026-11-09 (unchanged, 12): newlib's d = c & 0xff with buffer = d; buffer = (buffer << 8) | d (gives
   the original's or operand order), the newlib i < LBLOCKSIZE loop, d as int/unsigned/long, a separate pcpyh output
   variable (16 variants: 12-18); empty asm clobbers of $2/$3 at each point (14 variants: 12-18, 9 only with one
   inside the quadword loop, which moves the globals: an invented mechanism, not kept). 2026-11-10 (unchanged, 12):
   different-mode copies (int/unsigned/short/char d, (long)d with (unsigned long)d, (long)(int)d, buffer from c and
   from (unsigned char)c), d initialised at its declaration (newlib's unsigned int d = c & 0xff: 14, the andi moves to
   the entry block), buffer <<= 8; buffer |= d (19), a one-pass for loop (23), 18 flag sets: cse/combine fold every
   extension of the andi result into one register. The original's block uses neither v0 nor a0-a3 (local-alloc's
   first choices), as if its pseudos were global or v0 live across the block */
#if 0
void *memset(void *m, int c, size_t n)
{
    char *s = (char *)m;
    long buffer;
    u_long128 qbuffer;
    long *aligned_addr;

    /* newlib memset.c with SCE's blocks: TOO_SMALL(n) is n < sizeof(long), UNALIGNED(m) a quadword test */
    if (!(n < 8) && !((long)m & 15)) {
        aligned_addr = (long *)m;
        buffer = c & 0xff;
        buffer = (buffer << 8) | buffer;
        /* MATCH: MMI instruction (the low halfword copied through each doubleword) */
        asm("pcpyh %0, %1" : "=r"(buffer) : "r"(buffer));

        while (n >= 32) {
            /* MATCH: MMI instruction (the doubleword into both halves); loop-invariant, gcc hoists it */
            asm("pcpyld %0, %1, %1" : "=r"(qbuffer) : "r"(buffer));
            *(u_long128 *)aligned_addr = qbuffer;
            aligned_addr += 2;
            *(u_long128 *)aligned_addr = qbuffer;
            aligned_addr += 2;
            n -= 32;
        }

        while (n >= 8) {
            *aligned_addr++ = buffer;
            n -= 8;
        }
        s = (char *)aligned_addr;
    }

    while (n--)
        *s++ = (char)c;

    return m;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", memset);
#endif
/* Hand-written: MMI string routines (128-bit loads and compares) */
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcat);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strchr);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcmp);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strcpy);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strlen);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strncmp);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/__errno", strncpy);
