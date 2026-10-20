/* The parts of GCC 2.95's libgcc2.h/longlong.h the libgcc2 units use (MIPS EE: int 32 bits, long long 64). */
#ifndef LIBGCC2_H
#define LIBGCC2_H

typedef int SItype __attribute__((mode(SI)));
typedef unsigned int USItype __attribute__((mode(SI)));
typedef int DItype __attribute__((mode(DI)));
typedef unsigned int UDItype __attribute__((mode(DI)));
typedef float SFtype __attribute__((mode(SF)));
typedef float DFtype __attribute__((mode(DF)));

typedef int word_type __attribute__((mode(__word__)));

#define BITS_PER_UNIT 8
#define W_TYPE_SIZE 32
#define WORD_SIZE (sizeof(SItype) * BITS_PER_UNIT)
#define HIGH_HALFWORD_COEFF (((UDItype)1) << (WORD_SIZE / 2))
#define HIGH_WORD_COEFF (((UDItype)1) << WORD_SIZE)

struct DIstruct {
    SItype low, high;
};

typedef union {
    struct DIstruct s;
    DItype ll;
} DIunion;

#define umul_ppmm(w1, w0, u, v) \
    __asm__("multu %2,%3" : "=l"((USItype)(w0)), "=h"((USItype)(w1)) : "d"((USItype)(u)), "d"((USItype)(v)))

#define __umulsidi3(u, v)                         \
    ({                                            \
        DIunion __w;                              \
        umul_ppmm(__w.s.high, __w.s.low, u, v);   \
        __w.ll;                                   \
    })

#endif
