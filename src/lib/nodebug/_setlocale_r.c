/* newlib libc/locale/locale.c (MB_CAPABLE, newlib 1.9) and SCE's memchr */
#include "include_asm.h"

#define NULL 0
#define LC_ALL 0
#define LC_CTYPE 2
typedef unsigned int size_t;

struct _reent {
    char _pad[0x30];
    int _current_category;
    const char *_current_locale;
};
struct lconv;

extern struct _reent *_impure_ptr;
#define _REENT _impure_ptr
extern int __mb_cur_max;
extern int strcmp(const char *, const char *);
extern char *strcpy(char *, const char *);
extern size_t strlen(const char *);


/* stands in for the original's static struct lconv (its .data is in the nodebug data block):
   its strings come first in .rodata */
static inline const char *__lconv_strings(int i)
{
    return i ? "" : ".";
}

/* statics of the original, in the nodebug data block */
extern struct lconv lconv __asm__("lconv");
extern char lc_ctype[] __asm__("lc_ctype.3");
extern char last_lc_ctype[] __asm__("last_lc_ctype.4");

char *_setlocale_r(struct _reent *p, int category, const char *locale)
{
    if (locale) {
        if (category != LC_CTYPE) {
            if (strcmp(locale, "C") && strcmp(locale, ""))
                return NULL;
            if (category == LC_ALL) {
                strcpy(last_lc_ctype, lc_ctype);
                strcpy(lc_ctype, locale);
                __mb_cur_max = 1;
            }
        } else {
            if (strcmp(locale, "C") && strcmp(locale, "") && strcmp(locale, "C") &&
                strcmp(locale, "C-JIS") && strcmp(locale, "C-EUCJP") && strcmp(locale, "C-SJIS"))
                return NULL;

            strcpy(last_lc_ctype, lc_ctype);
            strcpy(lc_ctype, locale);

            if (!strcmp(locale, "C-JIS"))
                __mb_cur_max = 8;
            else if (strlen(locale) > 1)
                __mb_cur_max = 2;
            else
                __mb_cur_max = 1;
        }
        p->_current_category = category;
        p->_current_locale = locale;

        if (category == LC_CTYPE)
            return last_lc_ctype;
    } else {
        if (category == LC_CTYPE)
            return lc_ctype;
    }

    return "C";
}

struct lconv *_localeconv_r(struct _reent *data)
{
    return (struct lconv *)&lconv;
}

char *setlocale(int category, const char *locale)
{
    return _setlocale_r(_REENT, category, locale);
}

struct lconv *localeconv(void)
{
    return _localeconv_r(_REENT);
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/nodebug/_setlocale_r", memchr);
// SOURCE DATA: .rodata
