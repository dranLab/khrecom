/* newlib libc/ctype/tolower.c */
extern const char _ctype_[];
#define _U 01
#define isupper(c) ((_ctype_+1)[(unsigned)(c)]&_U)

int _tolower(int c)
{
    return isupper(c) ? (c) - 'A' + 'a' : c;
}
