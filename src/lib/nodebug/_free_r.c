/* newlib libc/stdlib/mallocr.c (dlmalloc 2.6.4): fREe and malloc_trim (DEFINE_FREE / DEFINE_MALLOC_TRIM) */
#include "mallocr.h"

void _free_r(RARG Void_t *mem)
{
    mchunkptr p; /* chunk corresponding to mem */
    INTERNAL_SIZE_T hd; /* its head field */
    INTERNAL_SIZE_T sz; /* its size */
    int idx; /* its bin index */
    mchunkptr next; /* next contiguous chunk */
    INTERNAL_SIZE_T nextsz; /* its size */
    INTERNAL_SIZE_T prevsz; /* size of previous contiguous chunk */
    mchunkptr bck; /* misc temp for linking */
    mchunkptr fwd; /* misc temp for linking */
    int islr; /* track whether merging with last_remainder */

    if (mem == 0) /* free(0) has no effect */
        return;

    MALLOC_LOCK;

    p = mem2chunk(mem);
    hd = p->size;

    check_inuse_chunk(p);

    sz = hd & ~PREV_INUSE;
    next = chunk_at_offset(p, sz);
    nextsz = chunksize(next);

    if (next == top) /* merge with top */
    {
        sz += nextsz;

        if (!(hd & PREV_INUSE)) /* consolidate backward */
        {
            prevsz = p->prev_size;
            p = chunk_at_offset(p, -prevsz);
            sz += prevsz;
            unlink(p, bck, fwd);
        }

        /* not struct-field stores: trim_threshold loads after them, like the original */
        *(INTERNAL_SIZE_T *)&p->size = sz | PREV_INUSE;
        *(mchunkptr *)&top = p;
        if ((unsigned long)(sz) >= (unsigned long)trim_threshold)
            malloc_trim(RCALL top_pad);
        MALLOC_UNLOCK;
        return;
    }

    set_head(next, nextsz); /* clear inuse bit */

    islr = 0;

    if (!(hd & PREV_INUSE)) /* consolidate backward */
    {
        prevsz = p->prev_size;
        p = chunk_at_offset(p, -prevsz);
        sz += prevsz;

        if (p->fd == last_remainder) /* keep as last_remainder */
            islr = 1;
        else
            unlink(p, bck, fwd);
    }

    if (!(inuse_bit_at_offset(next, nextsz))) /* consolidate forward */
    {
        sz += nextsz;

        if (!islr && next->fd == last_remainder) /* re-insert last_remainder */
        {
            islr = 1;
            link_last_remainder(p);
        } else
            unlink(next, bck, fwd);
    }

    set_head(p, sz | PREV_INUSE);
    set_foot(p, sz);
    if (!islr)
        frontlink(p, sz, idx, bck, fwd);

    MALLOC_UNLOCK;
}

int malloc_trim(RARG size_t pad)
{
    long top_size; /* Amount of top-most memory */
    long extra; /* Amount to release */
    char *current_brk; /* address returned by pre-check sbrk call */
    char *new_brk; /* address returned by negative sbrk call */

    unsigned long pagesz = malloc_getpagesize;

    MALLOC_LOCK;

    top_size = chunksize(top);
    extra = ((top_size - pad - MINSIZE + (pagesz - 1)) / pagesz - 1) * pagesz;

    if (extra < (long)pagesz) /* Not enough memory to release */
    {
        MALLOC_UNLOCK;
        return 0;
    }

    else {
        /* Test to make sure no one else called sbrk */
        current_brk = (char *)(MORECORE(0));
        if (current_brk != (char *)(top) + top_size) {
            MALLOC_UNLOCK;
            return 0; /* Apparently we don't own memory; must fail */
        }

        else {
            new_brk = (char *)(MORECORE(-extra));

            if (new_brk == (char *)(MORECORE_FAILURE)) /* sbrk failed? */
            {
                /* Try to figure out what we have */
                current_brk = (char *)(MORECORE(0));
                top_size = current_brk - (char *)top;
                if (top_size >= (long)MINSIZE) /* if not, we are very very dead! */
                {
                    sbrked_mem = current_brk - sbrk_base;
                    set_head(top, top_size | PREV_INUSE);
                }
                check_chunk(top);
                MALLOC_UNLOCK;
                return 0;
            }

            else {
                /* Success. Adjust top accordingly. */
                set_head(top, (top_size - extra) | PREV_INUSE);
                sbrked_mem -= extra;
                check_chunk(top);
                MALLOC_UNLOCK;
                return 1;
            }
        }
    }
}
