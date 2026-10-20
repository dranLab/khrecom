/* newlib libc/stdlib/atexit.c */
#define NULL 0
#define _ATEXIT_SIZE 32
typedef unsigned int size_t;

struct _atexit {
    struct _atexit *_next;
    int _ind;
    void (*_fns[_ATEXIT_SIZE])(void);
};

struct _reent {
    char _pad[0x148];
    struct _atexit *_atexit;
    struct _atexit _atexit0;
};

extern struct _reent *_impure_ptr;
#define _REENT _impure_ptr
extern void *malloc(size_t);

int atexit(void (*fn)(void))
{
    register struct _atexit *p;

    if ((p = _REENT->_atexit) == NULL)
        _REENT->_atexit = p = &_REENT->_atexit0;
    if (p->_ind >= _ATEXIT_SIZE) {
        if ((p = (struct _atexit *)malloc(sizeof *p)) == NULL)
            return -1;
        *(int *)&p->_ind = 0;
        *(struct _atexit **)&p->_next = _REENT->_atexit; /* not a struct-field store: _REENT reloads, like the original */
        _REENT->_atexit = p;
    }
    p->_fns[p->_ind++] = fn;
    return 0;
}
