/* printf for the EE kernel library: scePrintf (DECI2 tty), kprintf (SIO serial), sceSnprintf/sceVsnprintf. */
#include "include_asm.h"
#include "sdk/stdarg.h"
#include "sdk/eekernel.h"

int kputs(char *s);

typedef int (*PutcFunc)(void *ctx, unsigned int *size, int c);

static int count = 0;
static char linebuf[0x80];

static int kputchar(int c)
{
    while (*(volatile unsigned int *)0x1000F130 & 0x8000) {
    }
    *(volatile unsigned char *)0x1000F180 = c;
    return c;
}

/* Collect a line and send it to the host tty (kputs) at a newline or when full. */
static void deci2Putchar(int c)
{
    if (count >= 0x7E) {
        count = 0;
        linebuf[0x7F] = 0;
        kputs(linebuf);
    }
    if (c == '\n') {
        linebuf[count] = c;
        linebuf[count + 1] = 0;
        count = 0;
        kputs(linebuf);
        return;
    }
    linebuf[count++] = c;
}

static void serialPutchar(int c)
{
    if (c == '\n') {
        kputchar('\r');
        kputchar('\n');
        return;
    }
    kputchar(c);
}

/* The integer value of the double whose bits are x (9999 when too large); printfloat passes it d * 1e6. */
int ftoi(unsigned long x)
{
    long e = (x << 1) >> 53;

    e -= 0x433;
    if (e < -0x35) {
        return 0;
    }
    if (e >= 13) {
        return 9999;
    }
    x = (x << 12) >> 12;
    x |= 1UL << 52;
    if (e < 0) {
        x >>= (e = -e) - 2;
        if ((x & 3) == 3) {
            x = (x >> 2) + 1;
        } else {
            x >>= 2;
        }
    } else {
        x <<= e;
    }
    return x;
}

static int _sceSnprintf_pf(PutcFunc func, void *ctx, unsigned int *size, const char *fmt, ...);

int printfloat(PutcFunc func, void *ctx, unsigned int *size, double d)
{
    int n = 0, e = 0, i;

    if (*size == 0) {
        return 0;
    }
    if (d < 0.0) {
        d = 0.0 - d;
        n = 1;
        func(ctx, size, '-');
        (*size)--;
    }
    if (d < 0.1) {
        while (d < 0.1) {
            d *= 10.0;
            e--;
        }
    } else if (d >= 1.0) {
        while (d >= 1.0) {
            d /= 10.0;
            e++;
        }
    }
    i = ftoi((unsigned long)(d * 1000000.0));
    n += _sceSnprintf_pf(func, ctx, size, "0.%d", i);
    if (e >= 0) {
        n += _sceSnprintf_pf(func, ctx, size, "e+%d", e);
    } else {
        n += _sceSnprintf_pf(func, ctx, size, "e%d", e);
    }
    return n;
}

#define F_MINUS 0x1
#define F_PLUS 0x2
#define F_ZERO 0x4
#define F_SPACE 0x8
#define F_ALT 0x10
#define F_PREC 0x20
#define F_LONG 0x40
#define F_SHORT 0x80
#define F_CHAR 0x100

/* Parked: 2 of 853 lines, same size (2026-11-07): only the nop between c.eq.s and bc1f in the %f case (gcc writes
   "#nop"; the original's assembler inserted it at the ".set noreorder" before bc1f). The 991111/991111-01 ee/bin/as
   (GNU as 2.9-ee-991111) gives exactly that nop and the whole function matches, but it never fills a delay slot
   (not even with -O), so ftoi's and kputchar's `jr ra` slots, which 2.10 fills, open up; 2.10/2.12 insert no
   c.cond nop under any -mcpu/-mipsN, 990721 also adds one after mtc1. No source form found; the data and the
   draft's literals are ready (its .rdata block below). tried: compare_compilers (all compilers, own and
   unit assemblers), assembler option sweep on a snippet, permuter (earlier) */
#if 0
int _sceSnprintf(PutcFunc func, void *ctx, unsigned int size, const char *fmt, va_list ap)
{
    char buf[32];
    char prefix[32];
    char *bufend = &buf[31];
    char *p, *start;
    const char *digits, *q;
    unsigned long uval;
    long val;
    float f;
    int flags, width, prec, plen, len, n, i;
    int count = 0;
    char c, padc;

    while (*fmt) {
        c = *fmt;
        flags = 0;
        width = 0;
        prec = 0;
        plen = 0;
        if (c != '%') {
            goto plain;
        }
    again:
        fmt++;
        switch (*fmt) {
        case '-':
            flags |= F_MINUS;
            goto again;
        case '+':
            flags |= F_PLUS;
            goto again;
        case '0':
            flags |= F_ZERO;
            goto again;
        case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            n = 0;
            do {
                i = n * 10 - '0';
                n = i + *fmt;
                fmt++;
            } while ((unsigned char)(*fmt - '0') < 10);
            width = n;
            if (*fmt == 0) {
                goto next;
            }
            fmt--;
            goto again;
        case '.':
            fmt++;
            flags |= F_PREC;
            if (*fmt == '*') {
                n = va_arg(ap, int);
                prec = n > -2 ? n : -1;
                goto again;
            }
            for (n = 0; (unsigned char)(*fmt - '0') < 10; fmt++) {
                i = n * 10 - '0';
                n = i + *fmt;
            }
            prec = n > -2 ? n : -1;
        number_end:
            if (*fmt == 0) {
                goto next;
            }
            fmt--;
            goto again;
        case ' ':
            flags |= F_SPACE;
            goto again;
        case '#':
            flags |= F_ALT;
            goto again;
        case 'l':
            flags |= F_LONG;
            goto again;
        case 'h':
            flags |= F_SHORT;
            goto again;
        case 'o':
            if (flags & F_LONG) {
                uval = va_arg(ap, unsigned long);
            } else if (flags & F_SHORT) {
                uval = va_arg(ap, unsigned short);
            } else {
                uval = va_arg(ap, unsigned int);
            }
            p = bufend;
            if (uval == 0) {
                if (!(flags & F_PREC) || prec != 0) {
                    *p-- = '0';
                }
            } else {
                do {
                    *p-- = (uval & 7) + '0';
                    uval >>= 3;
                } while (uval);
                if (flags & F_ALT) {
                    *p-- = '0';
                }
            }
            start = p + 1;
            len = bufend - p;
            goto output;
        case 'x':
            digits = "0123456789abcdef";
            if (flags & F_ALT) {
                prefix[0] = '0';
                prefix[1] = 'x';
                plen = 2;
            }
            goto hex;
        case 'X':
            digits = "0123456789ABCDEF";
            if (flags & F_ALT) {
                prefix[0] = '0';
                prefix[1] = 'X';
                plen = 2;
            }
        hex:
            if (flags & F_LONG) {
                uval = va_arg(ap, unsigned long);
            } else if (flags & F_SHORT) {
                uval = va_arg(ap, unsigned short);
            } else {
                uval = va_arg(ap, unsigned int);
            }
            p = bufend;
            if (uval == 0) {
                plen = 0;
                if (!(flags & F_PREC) || prec != 0) {
                    *p-- = '0';
                }
            } else {
                do {
                    *p-- = digits[uval & 0xF];
                    uval >>= 4;
                } while (uval);
            }
            start = p + 1;
            len = bufend - p;
            goto output;
        case 'd':
        case 'i':
            if (flags & F_LONG) {
                val = va_arg(ap, long);
            } else if (flags & F_SHORT) {
                val = va_arg(ap, short);
            } else {
                val = va_arg(ap, int);
            }
            p = bufend;
            if (val >= 0) {
                if (flags & F_PLUS) {
                    prefix[0] = '+';
                    plen = 1;
                } else if (flags & F_SPACE) {
                    prefix[0] = ' ';
                    plen = 1;
                }
            } else {
                prefix[0] = '-';
                plen = 1;
            }
            if (val == 0) {
                if (!(flags & F_PREC) || prec != 0) {
                    *p-- = '0';
                }
            } else {
                if (val < 0) {
                    val = -val;
                }
                while (val) {
                    *p-- = val % 10 + '0';
                    val /= 10;
                }
            }
            start = p + 1;
            len = bufend - p;
            goto output;
        case 'u':
            if (flags & F_LONG) {
                uval = va_arg(ap, unsigned long);
            } else if (flags & F_SHORT) {
                uval = va_arg(ap, unsigned short);
            } else {
                uval = va_arg(ap, unsigned int);
            }
            p = bufend;
            if (uval == 0) {
                if (!(flags & F_PREC) || prec != 0) {
                    *p-- = '0';
                }
            } else {
                do {
                    *p-- = uval % 10 + '0';
                    uval /= 10;
                } while (uval);
            }
            start = p + 1;
            len = bufend - p;
            goto output;
        case 'e':
        case 'f':
            f = va_arg(ap, double);
            if (f == 0.0f) {
                count += func(ctx, &size, '0');
            } else {
                count += printfloat(func, ctx, &size, f);
            }
            goto next;
        case 's':
            q = va_arg(ap, char *);
            if (!q) {
                func(ctx, &size, '(');
                func(ctx, &size, 'n');
                func(ctx, &size, 'u');
                func(ctx, &size, 'l');
                func(ctx, &size, 'l');
                func(ctx, &size, ')');
                goto next;
            }
            p = (char *)q;
            if (flags & F_PREC) {
                for (n = 0; n < prec && *p++; n++) {
                }
                len = n;
            } else {
                for (len = 0; *p; p++) {
                    len++;
                }
            }
            start = (char *)q;
            goto output;
        case 'c':
            uval = va_arg(ap, char);
            flags |= F_CHAR;
            prec = 1;
            p = bufend;
            *p-- = uval;
            start = p + 1;
            len = bufend - p;
            goto output;
        case 'p':
            uval = va_arg(ap, unsigned int);
            digits = "0123456789abcdef";
            p = bufend;
            if (uval == 0) {
                if (!(flags & F_PREC) || prec != 0) {
                    *p-- = '0';
                }
            } else {
                do {
                    *p-- = digits[uval & 0xF];
                    uval >>= 4;
                } while (uval);
            }
            prefix[0] = '0';
            prefix[1] = 'x';
            plen = 2;
            start = p + 1;
            len = bufend - p;
            goto output;
        default:
            if (*fmt == 0) {
                goto next;
            }
            flags |= F_CHAR;
            prec = 1;
            p = bufend;
            *p-- = *fmt;
            start = p + 1;
            len = bufend - p;
            goto output;
        }
    output:
        if (len < prec) {
            prec -= len;
        } else {
            prec = 0;
        }
        width = width - len - plen - prec;
        p = start + len;
        if (flags & F_CHAR) {
            padc = ' ';
            if ((flags & F_ZERO) && width > 0) {
                padc = '0';
            }
        } else {
            padc = ' ';
            if ((flags & (F_PREC | F_ZERO)) == F_ZERO && width > 0) {
                padc = '0';
            }
        }
        if (flags & F_MINUS) {
            for (n = 0; n < plen; n++) {
                count += func(ctx, &size, prefix[n]);
            }
            for (n = 0; n < prec; n++) {
                count += func(ctx, &size, '0');
            }
            while (start < p) {
                count += func(ctx, &size, *start++);
            }
            for (n = 0; n < width; n++) {
                count += func(ctx, &size, ' ');
            }
        } else if (flags & F_ZERO) {
            if (padc == '0') {
                for (n = 0; n < plen; n++) {
                    count += func(ctx, &size, prefix[n]);
                }
                for (n = 0; n < width; n++) {
                    count += func(ctx, &size, padc);
                }
            } else {
                for (n = 0; n < width; n++) {
                    count += func(ctx, &size, padc);
                }
                for (n = 0; n < plen; n++) {
                    count += func(ctx, &size, prefix[n]);
                }
            }
            for (n = 0; n < prec; n++) {
                count += func(ctx, &size, '0');
            }
            while (start < p) {
                count += func(ctx, &size, *start++);
            }
        } else {
            for (n = 0; n < width; n++) {
                count += func(ctx, &size, padc);
            }
            for (n = 0; n < plen; n++) {
                count += func(ctx, &size, prefix[n]);
            }
            for (n = 0; n < prec; n++) {
                count += func(ctx, &size, '0');
            }
            while (start < p) {
                count += func(ctx, &size, *start++);
            }
        }
        goto next;
    plain:
        if (c != 0) {
            count += func(ctx, &size, c);
        }
    next:
        if (*fmt) {
            fmt++;
        }
    }
    func(ctx, &size, 0);
    return count;
}
#endif
int _sceSnprintf(PutcFunc func, void *ctx, unsigned int size, const char *fmt, va_list ap);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/kprintf2", _sceSnprintf);
/* _sceSnprintf's strings and jump table (the function is still assembly). */
#ifdef KH_RETAIL /* the same table at retail's addresses (every case label 0x72E00 lower) */
asm(".rdata\n.align 3\nD_00436900:\n.asciz \"0123456789abcdef\"\n"
    ".align 3\nD_00436918:\n.asciz \"0123456789ABCDEF\"\n"
    ".align 3\njtbl_00436930:\n"
    ".word .L00312E34\n.word .L0031345C\n.word .L0031345C\n.word .L00312E3C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L00312D54\n.word .L0031345C\n.word .L00312D4C\n.word .L00312DAC\n"
    ".word .L0031345C\n.word .L00312D5C\n.word .L00312D64\n.word .L00312D64\n.word .L00312D64\n"
    ".word .L00312D64\n.word .L00312D64\n.word .L00312D64\n.word .L00312D64\n.word .L00312D64\n"
    ".word .L00312D64\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L00312F34\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L0031345C\n.word .L00313378\n.word .L00312FF4\n.word .L003131D0\n"
    ".word .L003131D0\n.word .L0031345C\n.word .L00312E4C\n.word .L00312FF4\n.word .L0031345C\n"
    ".word .L0031345C\n.word .L00312E44\n.word .L0031345C\n.word .L0031345C\n.word .L00312E54\n"
    ".word .L003133B0\n.word .L0031345C\n.word .L0031345C\n.word .L00313234\n.word .L0031345C\n"
    ".word .L0031310C\n.word .L0031345C\n.word .L0031345C\n.word .L00312F08\n"
    ".text\n");
#else
asm(".rdata\n.align 3\nD_004FECC0:\n.asciz \"0123456789abcdef\"\n"
    ".align 3\nD_004FECD8:\n.asciz \"0123456789ABCDEF\"\n"
    ".align 3\njtbl_004FECF0:\n"
    ".word .L00385C34\n.word .L0038625C\n.word .L0038625C\n.word .L00385C3C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L00385B54\n.word .L0038625C\n.word .L00385B4C\n.word .L00385BAC\n"
    ".word .L0038625C\n.word .L00385B5C\n.word .L00385B64\n.word .L00385B64\n.word .L00385B64\n"
    ".word .L00385B64\n.word .L00385B64\n.word .L00385B64\n.word .L00385B64\n.word .L00385B64\n"
    ".word .L00385B64\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L00385D34\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L0038625C\n.word .L00386178\n.word .L00385DF4\n.word .L00385FD0\n"
    ".word .L00385FD0\n.word .L0038625C\n.word .L00385C4C\n.word .L00385DF4\n.word .L0038625C\n"
    ".word .L0038625C\n.word .L00385C44\n.word .L0038625C\n.word .L0038625C\n.word .L00385C54\n"
    ".word .L003861B0\n.word .L0038625C\n.word .L0038625C\n.word .L00386034\n.word .L0038625C\n"
    ".word .L00385F0C\n.word .L0038625C\n.word .L0038625C\n.word .L00385D08\n"
    ".text\n");
#endif

/* Into a buffer: ctx is the write pointer; the last byte of the buffer gets the terminating 0. */
static int snprintf_char(char **pbuf, unsigned int *size, int c)
{
    if (*size == 0) {
        return 1;
    }
    if (c < 0x100) {
        if (*size == 1) {
            c = 0;
        }
        *(*pbuf)++ = c;
        (*size)--;
        return 1;
    }
    **pbuf = 0;
    return 0;
}

static int printf_char(void *ctx, unsigned int *size, int c)
{
    if (c) {
        deci2Putchar(c);
    }
    return 1;
}

static int kprintf_char(void *ctx, unsigned int *size, int c)
{
    if (c) {
        serialPutchar(c);
    }
    return 1;
}

static int _sceSnprintf_pf(PutcFunc func, void *ctx, unsigned int *size, const char *fmt, ...)
{
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = _sceSnprintf(func, ctx, *size, fmt, ap);
    *size -= n;
    return n;
}

int sceVsnprintf(char *buf, int size, const char *fmt, va_list ap)
{
    return _sceSnprintf((PutcFunc)snprintf_char, &buf, size, fmt, ap);
}

int sceSnprintf(char *buf, int size, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    return _sceSnprintf((PutcFunc)snprintf_char, &buf, size, fmt, ap);
}

void sceVprintf(const char *fmt, va_list ap)
{
    int dummy = 0;
    int eie;

    eie = DIntr();
    _sceSnprintf(printf_char, &dummy, -1, fmt, ap);
    if (eie) {
        EIntr();
    }
}

void scePrintf(const char *fmt, ...)
{
    va_list ap;
    int dummy = 0;
    int eie;

    eie = DIntr();
    va_start(ap, fmt);
    _sceSnprintf(printf_char, &dummy, -1, fmt, ap);
    if (eie) {
        EIntr();
    }
}

void kprintf(const char *fmt, ...)
{
    va_list ap;
    int dummy = 0;
    int eie;

    eie = DIntr();
    va_start(ap, fmt);
    _sceSnprintf(kprintf_char, &dummy, -1, fmt, ap);
    if (eie) {
        EIntr();
    }
}
// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data .rodata
