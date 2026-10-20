/* newlib libc/reent/openr.c */
typedef long off_t;
typedef int _ssize_t;
typedef unsigned int size_t;
struct stat;
struct _reent {
    int _errno;
};
extern int errno;
extern int open(const char *file, int flags, int mode);

int _open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
    int ret;

    errno = 0;
    if ((ret = open(file, flags, mode)) == -1 && errno != 0)
        ptr->_errno = errno;
    return ret;
}
