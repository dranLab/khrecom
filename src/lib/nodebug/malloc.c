/* newlib libc/stdlib/malloc.c (with the malloc lock calls) */
typedef unsigned int size_t;
struct _reent;
extern struct _reent *_impure_ptr;
#define _REENT _impure_ptr
extern void __malloc_lock(struct _reent *);
extern void __malloc_unlock(struct _reent *);
extern void *_malloc_r(struct _reent *, size_t);
extern void _free_r(struct _reent *, void *);

void *malloc(size_t nbytes)
{
    void *p;

    __malloc_lock(_REENT);
    p = _malloc_r(_REENT, nbytes);
    __malloc_unlock(_REENT);
    return p;
}

void free(void *aptr)
{
    __malloc_lock(_REENT);
    _free_r(_REENT, aptr);
    __malloc_unlock(_REENT);
}
