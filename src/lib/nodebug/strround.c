/* newlib libc/stdio/vfprintf.c (floating point), SN Systems version: SN's float formatting (strround, strrev,
   itoa, fftoa) and digit grouping (add_separators) */
#include "stdio_local.h"
#include "lib/src/newlib/libm/fdlibm.h"

extern double sn_floor(double);
extern double sn_fmod(double, double);
extern size_t strlen(const char *);
extern void *memcpy(void *, const void *, size_t);
extern void *alloca(size_t);
extern char *strcpy(char *, const char *);

/* round the digit string at its n-th digit; 0 if the carry runs out of the first digit */
int strround(char *s, int n)
{
    if (n > 0) {
        n--;
        if (s[n] >= '5') {
            for (;;) {
                s[n] = '0';
                if (--n <= 0)
                    break;
                if (s[n] != '9')
                    break;
            }
            if (s[n] == '9')
                return 0;
            s[n]++;
        }
    }
    return 1;
}

char *strrev(char *s)
{
    int len = 0;
    int i;
    char c;

    while (s[len])
        len++;
    len--;
    for (i = 0; i < len; i++, len--) {
        c = s[i];
        s[i] = s[len];
        s[len] = c;
    }
    return s;
}

/* function-local static of the original, in the nodebug data block */
extern char lower[] __asm__("lower.9"); /* "0123456789abcdefghijklmnopqrstuvwxyz" */

char *itoa(int value, char *s, int base)
{
    unsigned int u = value;
    int neg = value < 0;
    int i;

    if (neg && base == 10)
        u = -value;
    i = 0;
    do {
        s[i++] = lower[u % base];
        u /= base;
    } while (u);
    if (neg)
        s[i++] = '-';
    s[i] = 0;
    return strrev(s);
}

static char fbuf[168] __attribute__((aligned(8)));

/* x as a string of fmt 'e', 'E' or 'f' with prec digits after the point; strip removes trailing zeros;
   the sign goes to *signp (or the string) */
char *fftoa(double x, int prec, char fmt, int strip, char *signp)
{
    unsigned int hx, lx, e, mant, sign;
    char *p;
    int n, d, exp, len, ndig, i, pos;
    double ip, eps;

    /* MATCH: fdlibm's EXTRACT_WORDS; its do { } while (0) keeps the parameter copies ahead of the word split
       (sched1), which frees a0-a2 for it and makes fmt lose its register to len + 1 */
    EXTRACT_WORDS(hx, lx, x);
    mant = hx & 0xfffff;
    e = hx >> 20;
    sign = e & 0x800;
    e &= 0x7ff;
    if (e == 0x7ff) {
        if (mant == 0 && lx == 0) {
            if (sign)
                memcpy(fbuf, "-Inf", 5);
            else
                memcpy(fbuf, "Inf", 4);
        } else
            memcpy(fbuf, "NaN", 4);
        return fbuf;
    }

    ndig = prec;
    n = 0;
    if (x < 0) {
        if (signp) {
            *signp = '-';
            p = fbuf;
        } else {
            fbuf[0] = '-';
            p = fbuf + 1;
        }
        x = 0 - x;
    } else
        p = fbuf;

    if (x >= 1.0) {
        /* the integer digits */
        ip = sn_floor(x);
        x = x - ip;
        while (n < 163) {
            d = sn_fmod(ip, 10.0);
            p[n++] = d + '0';
            if ((ip = (ip - d) * 0.1) < 1.0)
                break;
        }
        /* MATCH: d counts up (s0, the digit's register) and i down; n is the shift loop's counter below */
        for (d = 0, i = n - 1; d < i; d++, i--) {
            char c = p[d];
            p[d] = p[i];
            p[i] = c;
        }
    }
    exp = n;
    if (exp == 0) {
        if (fmt != 'f') {
            if (x != 0) {
                for (;;) {
                    x = x * 10.0;
                    if (x >= 1.0 || exp < -1020)
                        break;
                    exp--;
                }
                exp--;
            }
            if (x >= 1.0) {
                ndig = prec + 1;
                x = x * 0.1;
            }
        } else {
            p[0] = '0';
            n = 1;
        }
    }

    if (exp < 0)
        len = ndig;
    else if (fmt == 'f')
        len = ndig + (exp > 0 ? exp : 1);
    else
        len = ndig + 1;

    /* the fraction digits; MATCH: goto exits, a break would let expand_end_loop rotate the loop */
    eps = 5.551115123125783e-17;
    while (1) {
        x = x * 10.0;
        d = x;
        x = x - d;
        eps = eps * 10.0;
        if (n >= len)
            goto done;
        if (x < eps)
            goto done;
        if (x > 1.0 - eps)
            goto done;
        p[n++] = d + '0';
    }
done:
    if (x >= 0.5)
        d++;
    p[n++] = d + '0';
    while (n <= len)
        p[n++] = '0';
    if (!strround(p, len + 1)) {
        p[0] = '1';
        len = len + 1;
        exp++;
    }

    if (prec != 0) {
        pos = 1;
        if (fmt == 'f' && exp > 0)
            pos = exp;
        for (n = len; pos < n; n--)
            p[n] = p[n - 1];
        p[pos] = '.';
    } else
        len--;

    if (strip) {
        while (len != 0 && p[len] == '0')
            len--;
        if (p[len] == '.')
            len--;
    }

    if (fmt != 'f') {
        p[++len] = fmt;
        if (exp >= 0) {
            if (exp != 0)
                exp--;
            p[++len] = '+';
            if (exp < 10)
                p[++len] = '0';
        } else {
            p[++len] = '-';
            if (exp >= -9)
                p[++len] = '0';
            exp = -exp;
        }
        itoa(exp, p + ++len, 10);
    } else
        p[len + 1] = 0;
    return fbuf;
}

static int _vfwrite(int fd, const char *buf, unsigned int n, int flush);

void _vfflush(FILE *fp)
{
    _vfwrite(fp->_file, 0, 0, 2);
}

#define VFWRITE_CUMULATIVE_WRITTEN "cumulative_written.21"
#define VFWRITE_PCH_POINTER "pch_pointer.22"
#include "vfprintf.h"

/* insert ',' between groups of 3 integer digits; the result goes to buf */
char *add_separators(char *buf, char *cp, char *end, int *size)
{
    int n = end - cp + 10;
    char *tmp = alloca(n);
    char *q = tmp + n - 1;
    int count = 0;
    struct {
        int state; /* 0: integer part, 1: after the point, 2: exponent */
        char *dot;
    } st;
    char *p;

    if (buf == NULL || cp == NULL || end == NULL)
        return NULL;
    st.state = 0;
    st.dot = end;
    for (p = cp; p <= end && st.state != 2; p++) {
        switch (*p) {
        case '.':
            st.dot = p;
            if (st.state == 0)
                st.state = 1;
            break;
        case 'e':
            st.state = 2;
            break;
        case 'E':
            st.state = 2;
            break;
        }
    }
    if (st.state == 2)
        return cp;
    for (p = end; cp < p; p--) {
        *q-- = *p;
        if (p <= st.dot) {
            /* MATCH: count++ in both arms (decomp-permuter): count and the constant 2 swap t0/t1 otherwise */
            if (count % 3 == 0 && count != 0) {
                *q-- = ',';
                n++;
                count++;
            } else {
                count++;
            }
        }
    }
    *q = *p;
    n -= 10;
    memcpy(buf, q, n);
    *size = n;
    return buf;
}
// SOURCE DATA: .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .rodata
