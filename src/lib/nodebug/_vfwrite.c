/* newlib libc/stdio/vfprintf.c as vfiprintf (INTEGER_ONLY), SN Systems version: inline string output,
   buffered file output through _vfwrite() */
#include "stdio_local.h"

typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef long quad_t;
typedef unsigned long u_quad_t;
typedef int wchar_t;

extern int __mb_cur_max;
extern int _mbtowc_r(struct _reent *, wchar_t *, const char *, size_t, int *);
extern void *memcpy(void *, const void *, size_t);
extern void *memchr(const void *, int, size_t);
extern size_t strlen(const char *);
extern int write(int, const void *, int);
extern char *add_separators(char *buf, char *cp, char *end, int *size);

/* function-local statics of the original, in the nodebug data block */
extern int cumulative_written __asm__("cumulative_written.4");
extern char *pch_pointer __asm__("pch_pointer.5");

static char buffer[128];

/* buffer file output, writing 128 bytes at a time; flush != 0 writes out the rest */
static int _vfwrite(int fd, const char *buf, u_int n, int flush)
{
    int i;

    if (flush == 1) {
        i = write(fd, buffer, cumulative_written);
        cumulative_written = 0;
        pch_pointer = buffer;
        return i;
    }
    for (i = 0; i < n; i++) {
        *pch_pointer = buf[i];
        pch_pointer++;
        if (++cumulative_written >= 128) {
            int r = write(fd, buffer, cumulative_written);
            pch_pointer = buffer;
            cumulative_written = 0;
            if (r == 0)
                return 0;
        }
    }
    return i;
}

int _vfiprintf_r(struct _reent *, FILE *, const char *, va_list);

int vfiprintf(FILE *fp, const char *fmt0, va_list ap)
{
    return _vfiprintf_r(fp->_data, fp, fmt0, ap);
}

/*
 * Macros for converting digits to letters and vice versa
 */
#define to_digit(c) ((c) - '0')
#define is_digit(c) ((unsigned)to_digit(c) <= 9)
#define to_char(n) ((n) + '0')

#define BUF 40
#define PADSIZE 16 /* pad chunk size */

/* the strings (FILE with __SSTR) are written directly, other files through _vfwrite() */
#define PRINT(ptr, len)                                                                                \
    {                                                                                                  \
        if (fp->_flags & __SSTR) {                                                                     \
            if (fp->_w > (len)) {                                                                      \
                memcpy(fp->_p, (ptr), (len));                                                          \
                fp->_p += (len);                                                                       \
                fp->_w -= (len);                                                                       \
            } else {                                                                                   \
                memcpy(fp->_p, (ptr), fp->_w);                                                         \
                fp->_p += fp->_w;                                                                      \
                fp->_w = 0;                                                                            \
            }                                                                                          \
        } else {                                                                                       \
            if (fp->_flags == 0 && fp->_file == 0)                                                     \
                fp->_file = 1;                                                                         \
            _vfwrite(fp->_file, (ptr), (len), 0);                                                      \
        }                                                                                              \
    }
#define PAD(howmany, with)                                                                             \
    {                                                                                                  \
        if ((n = (howmany)) > 0) {                                                                     \
            while (n > PADSIZE) {                                                                      \
                PRINT(with, PADSIZE);                                                                  \
                n -= PADSIZE;                                                                          \
            }                                                                                          \
            PRINT(with, n);                                                                            \
        }                                                                                              \
    }
#define FLUSH()                                                                                        \
    {                                                                                                  \
        if (!(fp->_flags & __SSTR))                                                                    \
            _vfwrite(fp->_file, 0, 0, 1);                                                              \
    }

/*
 * To extend shorts properly, we need both signed and unsigned
 * argument extraction methods.
 */
#define SARG()                                                                                         \
    (flags & QUADINT ? va_arg(ap, quad_t)                                                              \
                     : flags & LONGINT ? va_arg(ap, long)                                              \
                                       : flags & SHORTINT ? (long)(short)va_arg(ap, int) : (long)va_arg(ap, int))
#define UARG()                                                                                         \
    (flags & QUADINT ? va_arg(ap, u_quad_t)                                                            \
                     : flags & LONGINT ? va_arg(ap, u_long)                                            \
                                       : flags & SHORTINT ? (u_long)(u_short)va_arg(ap, int)           \
                                                          : (u_long)va_arg(ap, u_int))


/*
 * Flags used during conversion.
 */
#define ALT 0x001 /* alternate form */
#define HEXPREFIX 0x002 /* add 0x or 0X prefix */
#define LADJUST 0x004 /* left adjustment */
#define LONGDBL 0x008 /* long double; unimplemented */
#define LONGINT 0x010 /* long integer */
#define QUADINT 0x020 /* quad integer */
#define SHORTINT 0x040 /* short integer */
#define ZEROPAD 0x080 /* zero (as opposed to blank) pad */
#define FPT 0x100 /* Floating point number */
#define GROUPING 0x200 /* ' flag: digit groups (SN) */

extern char blanks[PADSIZE] __asm__("blanks.12");
extern char zeroes[PADSIZE] __asm__("zeroes.13");

int _vfiprintf_r(struct _reent *data, FILE *fp, const char *fmt0, va_list ap)
{
    register char *fmt; /* format string */
    register int ch; /* character from fmt */
    register int n, m; /* handy integers (short term usage) */
    register char *cp; /* handy char pointer (short term usage) */
    register int flags; /* flags as above */
    int ret; /* return value accumulator */
    int width; /* width from format (%8d), or 0 */
    int prec; /* precision from format (%.3d), or -1 */
    char sign; /* sign prefix (' ', '+', '-', or \0) */
    wchar_t wc;
    u_quad_t _uquad; /* integer arguments %[diouxX] */
    enum { OCT, DEC, HEX } base; /* base for [diouxX] conversion */
    int dprec; /* a copy of prec if [diouxX], 0 otherwise */
    int realsz; /* field size expanded by dprec */
    int size; /* size of converted field or string */
    char *xdigs; /* digits for [xX] conversion */
    char buf[BUF]; /* space for %c, %[diouxX], %[eEfgG] */
    char ox[2]; /* space for 0x hex-prefix */
    int state = 0; /* mbtowc calls from library must not change state */

    fmt = (char *)fmt0;
    ret = 0;

    /*
     * Scan the format for conversions (`%' character).
     */
    for (;;) {
        cp = fmt;
        while ((n = _mbtowc_r(_REENT, &wc, fmt, __mb_cur_max, &state)) > 0) {
            fmt += n;
            if (wc == '%') {
                fmt--;
                break;
            }
        }
        if ((m = fmt - cp) != 0) {
            PRINT(cp, m);
            ret += m;
        }
        if (n <= 0)
            goto done;
        if (fp->_w <= 0 && (fp->_flags & __SSTR))
            return ret;
        fmt++; /* skip over '%' */

        flags = 0;
        dprec = 0;
        width = 0;
        prec = -1;
        sign = '\0';

    rflag:
        ch = *fmt++;
    reswitch:
        switch (ch) {
        case ' ':
            /*
             * ``If the space and + flags both appear, the space
             * flag will be ignored.''
             *	-- ANSI X3J11
             */
            if (!sign)
                sign = ' ';
            goto rflag;
        case '#':
            flags |= ALT;
            goto rflag;
        case '\'':
            flags |= GROUPING;
            goto rflag;
        case '*':
            /*
             * ``A negative field width argument is taken as a
             * - flag followed by a positive field width.''
             *	-- ANSI X3J11
             * They don't exclude field widths read from args.
             */
            if ((width = va_arg(ap, int)) >= 0)
                goto rflag;
            width = -width;
            /* FALLTHROUGH */
        case '-':
            flags |= LADJUST;
            goto rflag;
        case '+':
            sign = '+';
            goto rflag;
        case '.':
            if ((ch = *fmt++) == '*') {
                n = va_arg(ap, int);
                prec = n < 0 ? -1 : n;
                goto rflag;
            }
            n = 0;
            while (is_digit(ch)) {
                n = 10 * n + to_digit(ch);
                ch = *fmt++;
            }
            prec = n < 0 ? -1 : n;
            goto reswitch;
        case '0':
            /*
             * ``Note that 0 is taken as a flag, not as the
             * beginning of a field width.''
             *	-- ANSI X3J11
             */
            flags |= ZEROPAD;
            goto rflag;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            n = 0;
            do {
                n = 10 * n + to_digit(ch);
                ch = *fmt++;
            } while (is_digit(ch));
            width = n;
            goto reswitch;
        case 'h':
            flags |= SHORTINT;
            goto rflag;
        case 'l':
            if (*fmt == 'l') {
                fmt++;
                flags |= QUADINT;
            } else {
                flags |= LONGINT;
            }
            goto rflag;
        case 'q':
            flags |= QUADINT;
            goto rflag;
        case 'c':
            *(cp = buf) = va_arg(ap, int);
            size = 1;
            sign = '\0';
            break;
        case 'D':
            flags |= LONGINT;
            /*FALLTHROUGH*/
        case 'd':
        case 'i':
            _uquad = SARG();
            if ((quad_t)_uquad < 0) {
                _uquad = -_uquad;
                sign = '-';
            }
            base = DEC;
            goto number;
        case 'n':
            if (flags & QUADINT)
                *va_arg(ap, quad_t *) = ret;
            else if (flags & LONGINT)
                *va_arg(ap, long *) = ret;
            else if (flags & SHORTINT)
                *va_arg(ap, short *) = ret;
            else
                *va_arg(ap, int *) = ret;
            continue; /* no output */
        case 'O':
            flags |= LONGINT;
            /*FALLTHROUGH*/
        case 'o':
            _uquad = UARG();
            base = OCT;
            goto nosign;
        case 'p':
            /*
             * ``The argument shall be a pointer to void.  The
             * value of the pointer is converted to a sequence
             * of printable characters, in an implementation-
             * defined manner.''
             *	-- ANSI X3J11
             */
            /* NOSTRICT */
            _uquad = (u_long)va_arg(ap, void *);
            base = HEX;
            xdigs = "0123456789abcdef";
            flags |= HEXPREFIX;
            ch = 'x';
            goto nosign;
        case 's':
            if ((cp = va_arg(ap, char *)) == NULL)
                cp = "(null)";
            if (prec >= 0) {
                /*
                 * can't use strlen; can only look for the
                 * NUL in the first `prec' characters, and
                 * strlen() will go further.
                 */
                char *p = memchr(cp, 0, prec);

                if (p != NULL) {
                    size = p - cp;
                    if (size > prec)
                        size = prec;
                } else
                    size = prec;
            } else
                size = strlen(cp);
            sign = '\0';
            break;
        case 'U':
            flags |= LONGINT;
            /*FALLTHROUGH*/
        case 'u':
            _uquad = UARG();
            base = DEC;
            goto nosign;
        case 'X':
            xdigs = "0123456789ABCDEF";
            goto hex;
        case 'x':
            xdigs = "0123456789abcdef";
        hex:
            _uquad = UARG();
            base = HEX;
            /* leading 0x/X only if non-zero */
            if (flags & ALT && _uquad != 0)
                flags |= HEXPREFIX;

            /* unsigned conversions */
        nosign:
            sign = '\0';
            /*
             * ``... diouXx conversions ... if a precision is
             * specified, the 0 flag will be ignored.''
             *	-- ANSI X3J11
             */
        number:
            if ((dprec = prec) >= 0)
                flags &= ~ZEROPAD;

            /*
             * ``The result of converting a zero value with an
             * explicit precision of zero is no characters.''
             *	-- ANSI X3J11
             */
            cp = buf + BUF;
            if (_uquad != 0 || prec != 0) {
                /*
                 * Unsigned mod is hard, and unsigned mod
                 * by a constant is easier than that by
                 * a variable; hence this switch.
                 */
                switch (base) {
                case OCT:
                    do {
                        *--cp = to_char(_uquad & 7);
                        _uquad >>= 3;
                    } while (_uquad);
                    /* handle octal leading 0 */
                    if (flags & ALT && *cp != '0')
                        *--cp = '0';
                    break;

                case DEC:
                    /* many numbers are 1 digit */
                    while (_uquad >= 10) {
                        *--cp = to_char(_uquad % 10);
                        _uquad /= 10;
                    }
                    *--cp = to_char(_uquad);
                    break;

                case HEX:
                    do {
                        *--cp = xdigs[_uquad & 15];
                        _uquad >>= 4;
                    } while (_uquad);
                    break;

                default:
                    cp = "bug in vfprintf: bad base";
                    size = strlen(cp);
                    goto skipsize;
                }
            }
            if (base == DEC && (flags & GROUPING))
                cp = add_separators(buf, cp, buf + BUF, &size);
            else
                size = buf + BUF - cp;
        skipsize:
            break;
        default: /* "%?" prints ?, unless ? is NUL */
            if (ch == '\0')
                goto done;
            /* pretend it was %c with argument ch */
            cp = buf;
            *cp = ch;
            size = 1;
            sign = '\0';
            break;
        }

        /*
         * All reasonable formats wind up here.  At this point,
         * `cp' points to a string which (if not flags&LADJUST)
         * should be padded out to `width' places.  If
         * flags&ZEROPAD, it should first be prefixed by any
         * sign or other prefix; otherwise, it should be blank
         * padded before the prefix is emitted.  After any
         * left-hand padding and prefixing, emit zeroes
         * required by a decimal [diouxX] precision, then print
         * the string proper, then emit zeroes required by any
         * leftover floating precision; finally, if LADJUST,
         * pad with blanks.
         */

        /*
         * compute actual size, so we know how much to pad.
         */
        realsz = dprec > size ? dprec : size;
        if (sign)
            realsz++;
        else if (flags & HEXPREFIX)
            realsz += 2;

        /* right-adjusting blank padding */
        if ((flags & (LADJUST | ZEROPAD)) == 0)
            PAD(width - realsz, blanks);

        /* prefix */
        if (sign) {
            PRINT(&sign, 1);
        } else if (flags & HEXPREFIX) {
            ox[0] = '0';
            ox[1] = ch;
            PRINT(ox, 2);
        }

        /* right-adjusting zero padding */
        if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD)
            PAD(width - realsz, zeroes);

        /* leading zeroes from decimal precision */
        PAD(dprec - size, zeroes);

        /* the string or number proper */
        PRINT(cp, size);

        /* left-adjusting padding (always blank) */
        if (flags & LADJUST)
            PAD(width - realsz, blanks);

        /* finally, adjust ret */
        ret += width > realsz ? width : realsz;
    }
done:
    FLUSH();
    return (ret);
    /* NOTREACHED */
}
// SOURCE DATA: .rodata .bss
