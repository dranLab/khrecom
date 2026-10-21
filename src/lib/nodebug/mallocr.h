/* newlib libc/stdlib/mallocr.c (dlmalloc 2.6.4): the parts the nodebug malloc units use */
#ifndef NODEBUG_MALLOCR_H
#define NODEBUG_MALLOCR_H

typedef unsigned int size_t;
#define INTERNAL_SIZE_T size_t
#define Void_t void

struct _reent;
extern void __malloc_lock(struct _reent *);
extern void __malloc_unlock(struct _reent *);
extern void *_sbrk_r(struct _reent *, int);

#define RARG struct _reent *reent_ptr,
#define RCALL reent_ptr,
#define MALLOC_LOCK __malloc_lock(reent_ptr)
#define MALLOC_UNLOCK __malloc_unlock(reent_ptr)
#define MORECORE(size) _sbrk_r(RCALL(size))
#define MORECORE_FAILURE -1
#define malloc_getpagesize (4096)

struct malloc_chunk {
    INTERNAL_SIZE_T prev_size; /* Size of previous chunk (if free). */
    INTERNAL_SIZE_T size; /* Size in bytes, including overhead. */
    struct malloc_chunk *fd; /* double links -- used only if free. */
    struct malloc_chunk *bk;
};
typedef struct malloc_chunk *mchunkptr;
typedef struct malloc_chunk *mbinptr;

struct mallinfo {
    int arena; /* total space allocated from system */
    int ordblks; /* number of non-inuse chunks */
    int smblks; /* unused -- always zero */
    int hblks; /* number of mmapped regions */
    int hblkhd; /* total space in mmapped regions */
    int usmblks; /* unused -- always zero */
    int fsmblks; /* unused -- always zero */
    int uordblks; /* total allocated space */
    int fordblks; /* total non-inuse space */
    int keepcost; /* top-most, releasable (via malloc_trim) space */
};

#define SIZE_SZ (sizeof(INTERNAL_SIZE_T))
#define MINSIZE (sizeof(struct malloc_chunk))
#define mem2chunk(mem) ((mchunkptr)((char *)(mem)-2 * SIZE_SZ))

#define PREV_INUSE 0x1
#define IS_MMAPPED 0x2
#define SIZE_BITS (PREV_INUSE | IS_MMAPPED)
#define chunksize(p) ((p)->size & ~(SIZE_BITS))
#define chunk_at_offset(p, s) ((mchunkptr)(((char *)(p)) + (s)))
#define inuse_bit_at_offset(p, s) (((mchunkptr)(((char *)(p)) + (s)))->size & PREV_INUSE)
#define set_head(p, s) ((p)->size = (s))
#define set_foot(p, s) (((mchunkptr)((char *)(p) + (s)))->prev_size = (s))
#define set_head_size(p, s) ((p)->size = (((p)->size & PREV_INUSE) | (s)))
#define set_inuse_bit_at_offset(p, s) (((mchunkptr)(((char *)(p)) + (s)))->size |= PREV_INUSE)
#define chunk2mem(p) ((Void_t *)((char *)(p) + 2 * SIZE_SZ))
#define MALLOC_ALIGNMENT 16
#define MALLOC_ALIGN_MASK (MALLOC_ALIGNMENT - 1)
#define request2size(req)                                                                              \
    (((unsigned long)((req) + (SIZE_SZ + MALLOC_ALIGN_MASK)) < (unsigned long)(MINSIZE + MALLOC_ALIGN_MASK)) \
         ? MINSIZE                                                                                     \
         : (((req) + (SIZE_SZ + MALLOC_ALIGN_MASK)) & ~(MALLOC_ALIGN_MASK)))
#define long_sub_size_t(x, y) (sizeof(long) > sizeof(INTERNAL_SIZE_T) && x < y ? -(long)(y - x) : (long)(x - y))

#define NAV 128
extern mbinptr __malloc_av_[NAV * 2 + 2];
#define av_ __malloc_av_
#define bin_at(i) ((mbinptr)((char *)&(av_[2 * (i) + 2]) - 2 * SIZE_SZ))
#define first(b) ((b)->fd)
#define last(b) ((b)->bk)
#define next_bin(b) ((mbinptr)((char *)(b) + 2 * sizeof(mbinptr)))
#define prev_bin(b) ((mbinptr)((char *)(b)-2 * sizeof(mbinptr)))
#define top (bin_at(0)->fd)
#define initial_top ((mchunkptr)(bin_at(0)))
#define last_remainder (bin_at(1))
#define binblocks_r (bin_at(0)->size)
#define binblocks binblocks_r

#define MAX_SMALLBIN 63
#define MAX_SMALLBIN_SIZE 512
#define SMALLBIN_WIDTH 8
#define BINBLOCKWIDTH 4
#define is_small_request(nb) (nb < MAX_SMALLBIN_SIZE - SMALLBIN_WIDTH)
#define smallbin_index(sz) (((unsigned long)(sz)) >> 3)
#define bin_index(sz)                                                                                  \
    (((((unsigned long)(sz)) >> 9) == 0)      ? (((unsigned long)(sz)) >> 3)                           \
     : ((((unsigned long)(sz)) >> 9) <= 4)    ? 56 + (((unsigned long)(sz)) >> 6)                      \
     : ((((unsigned long)(sz)) >> 9) <= 20)   ? 91 + (((unsigned long)(sz)) >> 9)                      \
     : ((((unsigned long)(sz)) >> 9) <= 84)   ? 110 + (((unsigned long)(sz)) >> 12)                    \
     : ((((unsigned long)(sz)) >> 9) <= 340)  ? 119 + (((unsigned long)(sz)) >> 15)                    \
     : ((((unsigned long)(sz)) >> 9) <= 1364) ? 124 + (((unsigned long)(sz)) >> 18)                    \
                                              : 126)
#define idx2binblock(ix) ((unsigned long)1 << (ix / BINBLOCKWIDTH))
#define mark_binblock(ii) (binblocks_r = (INTERNAL_SIZE_T)((unsigned long)binblocks_r | idx2binblock(ii)))

#define frontlink(P, S, IDX, BK, FD)                                                                   \
    {                                                                                                  \
        if (S < MAX_SMALLBIN_SIZE) {                                                                   \
            IDX = smallbin_index(S);                                                                   \
            mark_binblock(IDX);                                                                        \
            BK = bin_at(IDX);                                                                          \
            FD = BK->fd;                                                                               \
            P->bk = BK;                                                                                \
            P->fd = FD;                                                                                \
            FD->bk = BK->fd = P;                                                                       \
        } else {                                                                                       \
            IDX = bin_index(S);                                                                        \
            BK = bin_at(IDX);                                                                          \
            FD = BK->fd;                                                                               \
            if (FD == BK)                                                                              \
                mark_binblock(IDX);                                                                    \
            else {                                                                                     \
                while (FD != BK && S < chunksize(FD))                                                  \
                    FD = FD->fd;                                                                       \
                BK = FD->bk;                                                                           \
            }                                                                                          \
            P->bk = BK;                                                                                \
            P->fd = FD;                                                                                \
            FD->bk = BK->fd = P;                                                                       \
        }                                                                                              \
    }

#define unlink(P, BK, FD)                                                                              \
    {                                                                                                  \
        BK = P->bk;                                                                                    \
        FD = P->fd;                                                                                    \
        FD->bk = BK;                                                                                   \
        BK->fd = FD;                                                                                   \
    }

#define clear_last_remainder (last_remainder->fd = last_remainder->bk = last_remainder)

#define link_last_remainder(P)                                                                         \
    {                                                                                                  \
        last_remainder->fd = last_remainder->bk = P;                                                   \
        P->fd = P->bk = last_remainder;                                                                \
    }

extern unsigned long __malloc_trim_threshold;
extern unsigned long __malloc_top_pad;
extern char *__malloc_sbrk_base;
extern struct mallinfo __malloc_current_mallinfo;
extern unsigned long __malloc_max_sbrked_mem;
extern unsigned long __malloc_max_total_mem;
#define max_sbrked_mem __malloc_max_sbrked_mem
#define max_total_mem __malloc_max_total_mem
#define mmapped_mem 0
#define trim_threshold __malloc_trim_threshold
#define top_pad __malloc_top_pad
#define sbrk_base __malloc_sbrk_base
#define current_mallinfo __malloc_current_mallinfo
#define sbrked_mem (current_mallinfo.arena)

#define check_inuse_chunk(P)
#define check_chunk(P)

void _free_r(RARG Void_t *mem);
int _malloc_trim_r(RARG size_t pad);
#define fREe _free_r
#define malloc_trim _malloc_trim_r

#endif
