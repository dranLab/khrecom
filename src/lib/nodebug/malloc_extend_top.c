/* newlib libc/stdlib/mallocr.c (dlmalloc 2.6.4): malloc_extend_top and mALLOc (DEFINE_MALLOC) */
#include "mallocr.h"

#define mALLOc _malloc_r

/*
  Extend the top-most chunk by obtaining memory from system.
  Main interface to sbrk (but see also malloc_trim).
*/
static void malloc_extend_top(RARG INTERNAL_SIZE_T nb)
{
    char *brk; /* return value from sbrk */
    INTERNAL_SIZE_T front_misalign; /* unusable bytes at front of sbrked space */
    INTERNAL_SIZE_T correction; /* bytes for 2nd sbrk call */
    char *new_brk; /* return of 2nd sbrk call */
    INTERNAL_SIZE_T top_size; /* new size of top chunk */

    mchunkptr old_top = top; /* Record state of old top */
    INTERNAL_SIZE_T old_top_size = chunksize(old_top);
    char *old_end = (char *)(chunk_at_offset(old_top, old_top_size));

    /* Pad request with top_pad plus minimal overhead */

    INTERNAL_SIZE_T sbrk_size = nb + top_pad + MINSIZE;
    unsigned long pagesz = malloc_getpagesize;

    /* If not the first time through, round to preserve page boundary */
    /* Otherwise, we need to correct to a page size below anyway. */
    /* (We also correct below if an intervening foreign sbrk call.) */

    if (sbrk_base != (char *)(-1))
        sbrk_size = (sbrk_size + (pagesz - 1)) & ~(pagesz - 1);

    brk = (char *)(MORECORE(sbrk_size));

    /* Fail if sbrk failed or if a foreign sbrk call killed our space */
    if (brk == (char *)(MORECORE_FAILURE) || (brk < old_end && old_top != initial_top))
        return;

    sbrked_mem += sbrk_size;

    if (brk == old_end) /* can just add bytes to current top */
    {
        top_size = sbrk_size + old_top_size;
        set_head(top, top_size | PREV_INUSE);
    } else {
        if (sbrk_base == (char *)(-1)) /* First time through. Record base */
            sbrk_base = brk;
        else /* Someone else called sbrk().  Count those bytes as sbrked_mem. */
            sbrked_mem += brk - (char *)old_end;

        /* Guarantee alignment of first new chunk made from this space */
        front_misalign = (unsigned long)chunk2mem(brk) & MALLOC_ALIGN_MASK;
        if (front_misalign > 0) {
            correction = (MALLOC_ALIGNMENT)-front_misalign;
            brk += correction;
        } else
            correction = 0;

        /* Guarantee the next brk will be at a page boundary */
        correction += pagesz - ((unsigned long)(brk + sbrk_size) & (pagesz - 1));

        /* Allocate correction */
        new_brk = (char *)(MORECORE(correction));
        if (new_brk == (char *)(MORECORE_FAILURE))
            return;

        sbrked_mem += correction;

        top = (mchunkptr)brk;
        top_size = new_brk - brk + correction;
        set_head(top, top_size | PREV_INUSE);

        if (old_top != initial_top) {

            /* There must have been an intervening foreign sbrk call. */
            /* A double fencepost is necessary to prevent consolidation */

            /* If not enough space to do this, then user did something very wrong */
            if (old_top_size < MINSIZE) {
                set_head(top, PREV_INUSE); /* will force null return from malloc */
                return;
            }

            /* Also keep size a multiple of MALLOC_ALIGNMENT */
            old_top_size = (old_top_size - 3 * SIZE_SZ) & ~MALLOC_ALIGN_MASK;
            set_head_size(old_top, old_top_size);
            chunk_at_offset(old_top, old_top_size)->size = SIZE_SZ | PREV_INUSE;
            chunk_at_offset(old_top, old_top_size + SIZE_SZ)->size = SIZE_SZ | PREV_INUSE;
            /* If possible, release the rest. */
            if (old_top_size >= MINSIZE)
                fREe(RCALL chunk2mem(old_top));
        }
    }

    if ((unsigned long)sbrked_mem > (unsigned long)max_sbrked_mem)
        max_sbrked_mem = sbrked_mem;
    if ((unsigned long)(mmapped_mem + sbrked_mem) > (unsigned long)max_total_mem)
        max_total_mem = mmapped_mem + sbrked_mem;
}

Void_t *mALLOc(RARG size_t bytes)
{
    mchunkptr victim; /* inspected/selected chunk */
    INTERNAL_SIZE_T victim_size; /* its size */
    int idx; /* index for bin traversal */
    mbinptr bin; /* associated bin */
    mchunkptr remainder; /* remainder from a split */
    long remainder_size; /* its size */
    int remainder_index; /* its bin index */
    unsigned long block; /* block traverser bit */
    int startidx; /* first bin of a traversed block */
    mchunkptr fwd; /* misc temp for linking */
    mchunkptr bck; /* misc temp for linking */
    mbinptr q; /* misc temp */

    INTERNAL_SIZE_T nb = request2size(bytes); /* padded request size; */

    MALLOC_LOCK;

    /* Check for exact match in a bin */

    if (is_small_request(nb)) /* Faster version for small requests */
    {
        idx = smallbin_index(nb);

        /* No traversal or size check necessary for small bins.  */

        q = bin_at(idx);
        victim = last(q);

        if (victim != q) {
            victim_size = chunksize(victim);
            unlink(victim, bck, fwd);
            set_inuse_bit_at_offset(victim, victim_size);
            MALLOC_UNLOCK;
            return chunk2mem(victim);
        }

        idx += 2; /* Set for bin scan below. We've already scanned 2 bins. */

    } else {
        idx = bin_index(nb);
        bin = bin_at(idx);

        for (victim = last(bin); victim != bin; victim = victim->bk) {
            victim_size = chunksize(victim);
            remainder_size = long_sub_size_t(victim_size, nb);

            if (remainder_size >= (long)MINSIZE) /* too big */
            {
                --idx; /* adjust to rescan below after checking last remainder */
                break;
            }

            else if (remainder_size >= 0) /* exact fit */
            {
                unlink(victim, bck, fwd);
                set_inuse_bit_at_offset(victim, victim_size);
                MALLOC_UNLOCK;
                return chunk2mem(victim);
            }
        }

        ++idx;
    }

    /* Try to use the last split-off remainder */

    if ((victim = last_remainder->fd) != last_remainder) {
        victim_size = chunksize(victim);
        remainder_size = long_sub_size_t(victim_size, nb);

        if (remainder_size >= (long)MINSIZE) /* re-split */
        {
            remainder = chunk_at_offset(victim, nb);
            set_head(victim, nb | PREV_INUSE);
            link_last_remainder(remainder);
            set_head(remainder, remainder_size | PREV_INUSE);
            set_foot(remainder, remainder_size);
            MALLOC_UNLOCK;
            return chunk2mem(victim);
        }

        clear_last_remainder;

        if (remainder_size >= 0) /* exhaust */
        {
            set_inuse_bit_at_offset(victim, victim_size);
            MALLOC_UNLOCK;
            return chunk2mem(victim);
        }

        /* Else place in bin */

        frontlink(victim, victim_size, remainder_index, bck, fwd);
    }

    /*
       If there are any possibly nonempty big-enough blocks,
       search for best fitting chunk by scanning bins in blockwidth units.
    */

    if ((block = idx2binblock(idx)) <= binblocks) {

        /* Get to the first marked block */

        if ((block & binblocks) == 0) {
            /* force to an even block boundary */
            idx = (idx & ~(BINBLOCKWIDTH - 1)) + BINBLOCKWIDTH;
            block <<= 1;
            while ((block & binblocks) == 0) {
                idx += BINBLOCKWIDTH;
                block <<= 1;
            }
        }

        /* For each possibly nonempty block ... */
        for (;;) {
            startidx = idx; /* (track incomplete blocks) */
            q = bin = bin_at(idx);

            /* For each bin in this block ... */
            do {
                /* Find and use first big enough chunk ... */

                for (victim = last(bin); victim != bin; victim = victim->bk) {
                    victim_size = chunksize(victim);
                    remainder_size = long_sub_size_t(victim_size, nb);

                    if (remainder_size >= (long)MINSIZE) /* split */
                    {
                        remainder = chunk_at_offset(victim, nb);
                        set_head(victim, nb | PREV_INUSE);
                        unlink(victim, bck, fwd);
                        link_last_remainder(remainder);
                        set_head(remainder, remainder_size | PREV_INUSE);
                        set_foot(remainder, remainder_size);
                        MALLOC_UNLOCK;
                        return chunk2mem(victim);
                    }

                    else if (remainder_size >= 0) /* take */
                    {
                        set_inuse_bit_at_offset(victim, victim_size);
                        unlink(victim, bck, fwd);
                        MALLOC_UNLOCK;
                        return chunk2mem(victim);
                    }
                }

                bin = next_bin(bin);

                if (idx < MAX_SMALLBIN) {
                    bin = next_bin(bin);
                    ++idx;
                }
            } while ((++idx & (BINBLOCKWIDTH - 1)) != 0);

            /* Clear out the block bit. */

            do /* Possibly backtrack to try to clear a partial block */
            {
                if ((startidx & (BINBLOCKWIDTH - 1)) == 0) {
                    binblocks &= ~block;
                    break;
                }
                --startidx;
                q = prev_bin(q);
            } while (first(q) == q);

            /* Get to the next possibly nonempty block */

            if ((block <<= 1) <= binblocks && (block != 0)) {
                while ((block & binblocks) == 0) {
                    idx += BINBLOCKWIDTH;
                    block <<= 1;
                }
            } else
                break;
        }
    }

    /* Try to use top chunk */

    /* Require that there be a remainder, ensuring top always exists  */
    remainder_size = long_sub_size_t(chunksize(top), nb);
    if (chunksize(top) < nb || remainder_size < (long)MINSIZE) {
        malloc_extend_top(RCALL nb);
        remainder_size = long_sub_size_t(chunksize(top), nb);
        if (chunksize(top) < nb || remainder_size < (long)MINSIZE) {
            MALLOC_UNLOCK;
            return 0; /* propagate failure */
        }
    }

    victim = top;
    set_head(victim, nb | PREV_INUSE);
    top = chunk_at_offset(victim, nb);
    set_head(top, remainder_size | PREV_INUSE);
    MALLOC_UNLOCK;
    return chunk2mem(victim);
}
