/* newlib libc/reent/readr.c */
typedef long off_t;
typedef int _ssize_t;
typedef unsigned int size_t;
struct stat;
struct _reent {
    int _errno;
};
extern int errno;
extern _ssize_t read(int fd, void *buf, size_t cnt);

_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t cnt)
{
    _ssize_t ret;

    errno = 0;
    if ((ret = read(fd, buf, cnt)) == -1 && errno != 0)
        ptr->_errno = errno;
    return ret;
}
