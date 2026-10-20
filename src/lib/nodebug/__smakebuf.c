/* newlib libc/stdio/makebuf.c, SCE version: never allocates, every stream stays unbuffered */
#define __SNBF 0x0002
#define __SOPT 0x0400
#define __SNPT 0x0800
#define BUFSIZ 1024
#define S_IFMT 0170000
#define S_IFREG 0100000

struct __sbuf {
    unsigned char *_base;
    int _size;
};

typedef struct __sFILE {
    unsigned char *_p;
    int _r;
    int _w;
    short _flags;
    short _file;
    struct __sbuf _bf;
    int _lbfsize;
    void *_cookie;
    int (*_read)();
    int (*_write)();
    long (*_seek)();
    int (*_close)();
    struct __sbuf _ub;
    unsigned char *_up;
    int _ur;
    unsigned char _ubuf[3];
    unsigned char _nbuf[1];
    struct __sbuf _lb;
    int _blksize;
    int _offset;
    struct _reent *_data;
} FILE;

struct stat {
    short st_dev;
    unsigned short st_ino;
    int st_mode;
    char _pad[0x70 - 8];
};

extern int _fstat_r(struct _reent *, int, struct stat *);
extern long __sseek();

void __smakebuf(register FILE *fp)
{
    struct stat st;

    if (fp->_flags & __SNBF) {
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
        return;
    }
    if (fp->_file < 0 || _fstat_r(fp->_data, fp->_file, &st) < 0) {
        fp->_flags |= __SNPT;
    } else {
        if ((st.st_mode & S_IFMT) == S_IFREG && fp->_seek == __sseek) {
            fp->_flags |= __SOPT;
            fp->_blksize = BUFSIZ;
        } else
            fp->_flags |= __SNPT;
    }
    fp->_flags |= __SNBF;
    fp->_bf._base = fp->_p = fp->_nbuf;
    fp->_bf._size = 1;
}
