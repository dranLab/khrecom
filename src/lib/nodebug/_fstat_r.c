/* newlib libc/reent/fstatr.c */
typedef long off_t;
typedef int _ssize_t;
typedef unsigned int size_t;
struct stat;
struct _reent {
    int _errno;
};
extern int errno;
extern int fstat(int fd, struct stat *pstat);

int _fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
    int ret;

    errno = 0;
    if ((ret = fstat(fd, pstat)) == -1 && errno != 0)
        ptr->_errno = errno;
    return ret;
}
