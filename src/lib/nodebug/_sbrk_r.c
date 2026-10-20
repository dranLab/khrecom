/* newlib libc/reent/sbrkr.c */
typedef long off_t;
typedef int _ssize_t;
typedef unsigned int size_t;
struct stat;
struct _reent {
    int _errno;
};
extern int errno;
extern void * sbrk(size_t incr);

void * _sbrk_r(struct _reent *ptr, size_t incr)
{
    void * ret;

    errno = 0;
    if ((ret = sbrk(incr)) == (void *)-1 && errno != 0)
        ptr->_errno = errno;
    return ret;
}
