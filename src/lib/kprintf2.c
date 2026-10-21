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

/* The integer value of x * 1e6 (x: the bits of a double) for "0.%d". */
/* Parked: 21 lines differ (permuter score) */
#if 0
int ftoi(unsigned long x)
{
    long e = ((x << 1) >> 53) - 0x433;
    unsigned long m;

    if (e < -0x35) {
        return 0;
    }
    if (e >= 13) {
        return 9999;
    }
    m = ((x << 12) >> 12) | (1UL << 52);
    if (e < 0) {
        m >>= -e - 2;
        if ((m & 3) == 3) {
            m = (m >> 2) + 1;
        } else {
            m >>= 2;
        }
    } else {
        m <<= e;
    }
    return m;
}
#endif
int ftoi(unsigned long x);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/kprintf2", ftoi);

static int _sceSnprintf_pf(PutcFunc func, void *ctx, unsigned int *size, const char *fmt, ...);

/* Parked: 16 lines differ (permuter score) */
#if 0
int printfloat(PutcFunc func, void *ctx, unsigned int *size, double d)
{
    int n = 0, e = 0;

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
    n += _sceSnprintf_pf(func, ctx, size, "0.%d", ftoi((unsigned long)(d * 1000000.0)));
    if (e >= 0) {
        n += _sceSnprintf_pf(func, ctx, size, "e+%d", e);
    } else {
        n += _sceSnprintf_pf(func, ctx, size, "e%d", e);
    }
    return n;
}
#endif
int printfloat(PutcFunc func, void *ctx, unsigned int *size, double d);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/kprintf2", printfloat);

#define F_MINUS 0x1
#define F_PLUS 0x2
#define F_ZERO 0x4
#define F_SPACE 0x8
#define F_ALT 0x10
#define F_PREC 0x20
#define F_LONG 0x40
#define F_SHORT 0x80
#define F_CHAR 0x100

/* Parked: 4 lines differ (permuter score) */
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
// SOURCE DATA: .data .bss
