/* newlib libc/stdlib/rand.c */
struct _reent;
extern struct _reent *_impure_ptr;
#define _REENT _impure_ptr
#define RAND_MAX 0x7fffffff

struct _reent {
    char _pad[0x58];
    unsigned int _next;
};

void srand(unsigned int seed)
{
    _REENT->_next = seed;
}

int rand(void)
{
    return ((_REENT->_next = _REENT->_next * 1103515245 + 12345) & RAND_MAX);
}
