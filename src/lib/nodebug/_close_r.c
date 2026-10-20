/* newlib libc/reent/closer.c */
typedef long off_t;
typedef int _ssize_t;
typedef unsigned int size_t;
struct stat;
struct _reent {
    int _errno;
};
extern int errno;
extern int close(int fd);

int _close_r(struct _reent *ptr, int fd)
{
    int ret;

    errno = 0;
    if ((ret = close(fd)) == -1 && errno != 0)
        ptr->_errno = errno;
    return ret;
}
