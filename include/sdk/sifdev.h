/* sifdev.h (Sony EE SDK): the declarations the library units use. */
#ifndef SDK_SIFDEV_H
#define SDK_SIFDEV_H

struct sce_stat {
    unsigned int st_mode;
    unsigned int st_attr;
    unsigned int st_size;
    unsigned char st_ctime[8];
    unsigned char st_atime[8];
    unsigned char st_mtime[8];
    unsigned int st_hisize;
    unsigned int st_private[6];
};

struct sce_dirent {
    struct sce_stat d_stat;
    char d_name[256];
    void *d_private;
};

#endif
