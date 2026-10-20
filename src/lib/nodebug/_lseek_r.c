/* newlib libc/reent/lseekr.c */
typedef long off_t;
typedef int _ssize_t;
typedef unsigned int size_t;
struct stat;
struct _reent {
    int _errno;
};
extern int errno;
extern off_t lseek(int fd, off_t pos, int whence);

off_t _lseek_r(struct _reent *ptr, int fd, off_t pos, int whence)
{
    off_t ret;

    errno = 0;
    if ((ret = lseek(fd, pos, whence)) == -1 && errno != 0)
        ptr->_errno = errno;
    return ret;
}
