/* Doug Lea's malloc 2.8.x, mspaces only, with a comment/return-address debug tag in every chunk header. */
#include "System/dlmalloc.h"
#include "include_asm.h"

extern int* __errno(void);
#define MALLOC_FAILURE_ACTION (*__errno() = 12) /* ENOMEM */

#define MAX_SIZE_T (~(size_t)0)
#define SIZE_T_SIZE (sizeof(size_t))
#define SIZE_T_BITSIZE (sizeof(size_t) << 3)
#define SIZE_T_ONE ((size_t)1)

#define MALLOC_ALIGNMENT ((size_t)16U)
#define CHUNK_ALIGN_MASK (MALLOC_ALIGNMENT - SIZE_T_ONE)
#define is_aligned(A) (((size_t)((A)) & (CHUNK_ALIGN_MASK)) == 0)
#define align_offset(A)                                                                                                \
    ((((size_t)(A)&CHUNK_ALIGN_MASK) == 0) ? 0 : ((MALLOC_ALIGNMENT - ((size_t)(A)&CHUNK_ALIGN_MASK)) & CHUNK_ALIGN_MASK))

#define DEFAULT_GRANULARITY ((size_t)64U * (size_t)1024U)
#define DEFAULT_TRIM_THRESHOLD ((size_t)2U * (size_t)1024U * (size_t)1024U)
#define DEFAULT_MMAP_THRESHOLD MAX_SIZE_T

#define M_TRIM_THRESHOLD (-1)
#define M_GRANULARITY (-2)
#define M_MMAP_THRESHOLD (-3)

#define HAVE_MMAP 0
#define HAVE_MORECORE 0
#define MFAIL ((void*)(MAX_SIZE_T))
#define CMFAIL ((char*)(MFAIL))
#define CALL_MUNMAP(a, s) (-1)
#define CALL_MREMAP(addr, osz, nsz, mv) MFAIL
#define CALL_MORECORE(S) MFAIL
#define HALF_MAX_SIZE_T (MAX_SIZE_T / 2U)

#define IS_MMAPPED_BIT (0U)
#define USE_NONCONTIGUOUS_BIT (4U)
#define EXTERN_BIT (8U)

struct malloc_chunk {
    size_t prev_foot;
    size_t head;
    char* comment;
    unsigned int ra;
    struct malloc_chunk* fd;
    struct malloc_chunk* bk;
};

typedef struct malloc_chunk mchunk;
typedef struct malloc_chunk* mchunkptr;
typedef struct malloc_chunk* sbinptr;
typedef unsigned int bindex_t;
typedef unsigned int binmap_t;
typedef unsigned int flag_t;

#define MCHUNK_SIZE (sizeof(mchunk))
#define CHUNK_OVERHEAD (SIZE_T_SIZE * 3)
#define MIN_CHUNK_SIZE ((MCHUNK_SIZE + CHUNK_ALIGN_MASK) & ~CHUNK_ALIGN_MASK)

#define chunk2mem(p) ((void*)((char*)(p) + 16))
#define mem2chunk(mem) ((mchunkptr)((char*)(mem)-16))
#define align_as_chunk(A) (mchunkptr)((A) + align_offset(chunk2mem(A)))

#define MAX_REQUEST ((-MIN_CHUNK_SIZE) << 2)
#define MIN_REQUEST (MIN_CHUNK_SIZE - CHUNK_OVERHEAD - SIZE_T_ONE)

#define pad_request(req) (((req) + CHUNK_OVERHEAD + CHUNK_ALIGN_MASK) & ~CHUNK_ALIGN_MASK)
#define request2size(req) (((req) < MIN_REQUEST) ? MIN_CHUNK_SIZE : pad_request(req))

#define PINUSE_BIT (SIZE_T_ONE)
#define CINUSE_BIT (SIZE_T_ONE << 1)
#define INUSE_BITS (PINUSE_BIT | CINUSE_BIT)
#define FENCEPOST_HEAD (INUSE_BITS | SIZE_T_SIZE)

#define cinuse(p) ((p)->head & CINUSE_BIT)
#define pinuse(p) ((p)->head & PINUSE_BIT)
#define chunksize(p) ((p)->head & ~(INUSE_BITS))
#define clear_pinuse(p) ((p)->head &= ~PINUSE_BIT)

#define chunk_plus_offset(p, s) ((mchunkptr)(((char*)(p)) + (s)))
#define chunk_minus_offset(p, s) ((mchunkptr)(((char*)(p)) - (s)))
#define next_chunk(p) ((mchunkptr)(((char*)(p)) + ((p)->head & ~INUSE_BITS)))
#define prev_chunk(p) ((mchunkptr)(((char*)(p)) - ((p)->prev_foot)))
#define next_pinuse(p) ((next_chunk(p)->head) & PINUSE_BIT)

#define set_foot(p, s) (((mchunkptr)((char*)(p) + (s)))->prev_foot = (s))
#define set_size_and_pinuse_of_free_chunk(p, s) ((p)->head = (s | PINUSE_BIT), set_foot(p, s))
#define set_free_with_pinuse(p, s, n) (clear_pinuse(n), set_size_and_pinuse_of_free_chunk(p, s))

#define set_inuse(M, p, s)                                                                                             \
    ((p)->head = (((p)->head & PINUSE_BIT) | s | CINUSE_BIT),                                                          \
     ((mchunkptr)(((char*)(p)) + (s)))->head |= PINUSE_BIT)
#define set_inuse_and_pinuse(M, p, s)                                                                                  \
    ((p)->head = (s | PINUSE_BIT | CINUSE_BIT), ((mchunkptr)(((char*)(p)) + (s)))->head |= PINUSE_BIT)
#define set_size_and_pinuse_of_inuse_chunk(M, p, s) ((p)->head = (s | PINUSE_BIT | CINUSE_BIT))

#define clear_tag(p) ((p)->comment = 0, (p)->ra = 0)

struct malloc_tree_chunk {
    size_t prev_foot;
    size_t head;
    char* comment;
    unsigned int ra;
    struct malloc_tree_chunk* fd;
    struct malloc_tree_chunk* bk;
    struct malloc_tree_chunk* child[2];
    struct malloc_tree_chunk* parent;
    bindex_t index;
};

typedef struct malloc_tree_chunk tchunk;
typedef struct malloc_tree_chunk* tchunkptr;
typedef struct malloc_tree_chunk* tbinptr;

#define leftmost_child(t) ((t)->child[0] != 0 ? (t)->child[0] : (t)->child[1])

struct malloc_segment {
    char* base;
    size_t size;
    struct malloc_segment* next;
    flag_t sflags;
};

typedef struct malloc_segment msegment;
typedef struct malloc_segment* msegmentptr;

#define NSMALLBINS (32U)
#define NTREEBINS (32U)
#define SMALLBIN_SHIFT (4U)
#define TREEBIN_SHIFT (8U)
#define MIN_LARGE_SIZE (SIZE_T_ONE << TREEBIN_SHIFT)
#define MAX_SMALL_SIZE (MIN_LARGE_SIZE - SIZE_T_ONE)
#define MAX_SMALL_REQUEST (MAX_SMALL_SIZE - CHUNK_ALIGN_MASK - CHUNK_OVERHEAD)

struct malloc_state {
    binmap_t smallmap;
    binmap_t treemap;
    size_t dvsize;
    size_t topsize;
    char* least_addr;
    mchunkptr dv;
    mchunkptr top;
    size_t trim_check;
    size_t magic;
    mchunkptr smallbins[(NSMALLBINS + 2) * 2];
    tbinptr treebins[NTREEBINS];
    size_t footprint;
    size_t max_footprint;
    flag_t mflags;
    msegment seg;
};

typedef struct malloc_state* mstate;

struct malloc_params {
    size_t magic;
    size_t page_size;
    size_t granularity;
    size_t mmap_threshold;
    size_t trim_threshold;
    flag_t default_mflags;
};

static struct malloc_params mparams;
static struct malloc_state _gm_;

#define is_initialized(M) ((M)->top != 0)
#define is_mmapped_segment(S) ((S)->sflags & IS_MMAPPED_BIT)
#define is_extern_segment(S) ((S)->sflags & EXTERN_BIT)
#define granularity_align(S) (((S) + (mparams.granularity)) & ~(mparams.granularity - SIZE_T_ONE))
#define MMAP_FOOT_PAD (4 * SIZE_T_SIZE)
#define disable_contiguous(M) ((M)->mflags |= USE_NONCONTIGUOUS_BIT)

#define TOP_FOOT_SIZE (align_offset(chunk2mem(0)) + pad_request(sizeof(struct malloc_segment)) + MIN_CHUNK_SIZE)

#define should_trim(M, s) ((s) > (M)->trim_check)

#define is_small(s) (((s) >> SMALLBIN_SHIFT) < NSMALLBINS)
#define small_index(s) ((s) >> SMALLBIN_SHIFT)
#define small_index2size(i) ((i) << SMALLBIN_SHIFT)

#define smallbin_at(M, i) ((sbinptr)((char*)&((M)->smallbins[(i) << 1])))
#define treebin_at(M, i) (&((M)->treebins[i]))

#define compute_tree_index(S, I)                                                                                       \
    {                                                                                                                  \
        size_t X = S >> TREEBIN_SHIFT;                                                                                 \
        if (X == 0)                                                                                                    \
            I = 0;                                                                                                     \
        else if (X > 0xFFFF)                                                                                           \
            I = NTREEBINS - 1;                                                                                         \
        else {                                                                                                         \
            unsigned int Y = (unsigned int)X;                                                                          \
            unsigned int N = ((Y - 0x100) >> 16) & 8;                                                                  \
            unsigned int K = (((Y <<= N) - 0x1000) >> 16) & 4;                                                         \
            N += K;                                                                                                    \
            N += K = (((Y <<= K) - 0x4000) >> 16) & 2;                                                                 \
            K = 14 - N + ((Y <<= K) >> 15);                                                                            \
            I = (K << 1) + ((S >> (K + (TREEBIN_SHIFT - 1)) & 1));                                                     \
        }                                                                                                              \
    }

#define leftshift_for_tree_index(i)                                                                                    \
    ((i == NTREEBINS - 1) ? 0 : ((SIZE_T_BITSIZE - SIZE_T_ONE) - (((i) >> 1) + TREEBIN_SHIFT - 2)))

#define idx2bit(i) ((binmap_t)(1) << (i))

#define mark_smallmap(M, i) ((M)->smallmap |= idx2bit(i))
#define clear_smallmap(M, i) ((M)->smallmap &= ~idx2bit(i))
#define smallmap_is_marked(M, i) ((M)->smallmap & idx2bit(i))

#define mark_treemap(M, i) ((M)->treemap |= idx2bit(i))
#define clear_treemap(M, i) ((M)->treemap &= ~idx2bit(i))
#define treemap_is_marked(M, i) ((M)->treemap & idx2bit(i))

#define compute_bit2idx(X, I)                                                                                          \
    {                                                                                                                  \
        unsigned int Y = X - 1;                                                                                        \
        unsigned int K = Y >> (16 - 4) & 16;                                                                           \
        unsigned int N = K;                                                                                            \
        Y >>= K;                                                                                                       \
        N += K = Y >> (8 - 3) & 8;                                                                                     \
        Y >>= K;                                                                                                       \
        N += K = Y >> (4 - 2) & 4;                                                                                     \
        Y >>= K;                                                                                                       \
        N += K = Y >> (2 - 1) & 2;                                                                                     \
        Y >>= K;                                                                                                       \
        N += K = Y >> (1 - 0) & 1;                                                                                     \
        Y >>= K;                                                                                                       \
        I = (bindex_t)(N + Y);                                                                                         \
    }

#define least_bit(x) ((x) & -(x))
#define left_bits(x) ((x << 1) | -(x << 1))

/* ----------------------------------- bin maintenance ----------------------------------- */

#define insert_small_chunk(M, P, S)                                                                                    \
    {                                                                                                                  \
        bindex_t I = small_index(S);                                                                                   \
        mchunkptr B = smallbin_at(M, I);                                                                               \
        mchunkptr F = B;                                                                                               \
        if (!smallmap_is_marked(M, I))                                                                                 \
            mark_smallmap(M, I);                                                                                       \
        else                                                                                                           \
            F = B->fd;                                                                                                 \
        B->fd = P;                                                                                                     \
        F->bk = P;                                                                                                     \
        P->fd = F;                                                                                                     \
        P->bk = B;                                                                                                     \
    }

#define unlink_small_chunk(M, P, S)                                                                                    \
    {                                                                                                                  \
        mchunkptr F = P->fd;                                                                                           \
        mchunkptr B = P->bk;                                                                                           \
        if (F == B)                                                                                                    \
            clear_smallmap(M, small_index(S));                                                                         \
        else {                                                                                                         \
            F->bk = B;                                                                                                 \
            B->fd = F;                                                                                                 \
        }                                                                                                              \
    }

#define unlink_first_small_chunk(M, B, P, I)                                                                           \
    {                                                                                                                  \
        mchunkptr F = P->fd;                                                                                           \
        if (B == F)                                                                                                    \
            clear_smallmap(M, I);                                                                                      \
        else {                                                                                                         \
            B->fd = F;                                                                                                 \
            F->bk = B;                                                                                                 \
        }                                                                                                              \
    }

#define replace_dv(M, P, S)                                                                                            \
    {                                                                                                                  \
        size_t DVS = M->dvsize;                                                                                        \
        if (DVS != 0) {                                                                                                \
            mchunkptr DV = M->dv;                                                                                      \
            insert_small_chunk(M, DV, DVS);                                                                            \
        }                                                                                                              \
        M->dvsize = S;                                                                                                 \
        M->dv = P;                                                                                                     \
    }

#define insert_large_chunk(M, X, S)                                                                                    \
    {                                                                                                                  \
        tbinptr* H;                                                                                                    \
        bindex_t I;                                                                                                    \
        compute_tree_index(S, I);                                                                                      \
        H = treebin_at(M, I);                                                                                          \
        X->index = I;                                                                                                  \
        X->child[0] = X->child[1] = 0;                                                                                 \
        if (!treemap_is_marked(M, I)) {                                                                                \
            mark_treemap(M, I);                                                                                        \
            *H = X;                                                                                                    \
            X->parent = (tchunkptr)H;                                                                                  \
            X->fd = X->bk = X;                                                                                         \
        } else {                                                                                                       \
            tchunkptr T = *H;                                                                                          \
            size_t K = S << leftshift_for_tree_index(I);                                                               \
            while (1) {                                                                                               \
                if (chunksize(T) != S) {                                                                               \
                    tchunkptr* C = &(T->child[(K >> (SIZE_T_BITSIZE - SIZE_T_ONE)) & 1]);                              \
                    K <<= 1;                                                                                           \
                    if (*C != 0)                                                                                       \
                        T = *C;                                                                                        \
                    else {                                                                                             \
                        *C = X;                                                                                        \
                        X->parent = T;                                                                                 \
                        X->fd = X->bk = X;                                                                             \
                        break;                                                                                         \
                    }                                                                                                  \
                } else {                                                                                               \
                    tchunkptr F = T->fd;                                                                               \
                    T->fd = F->bk = X;                                                                                 \
                    X->fd = F;                                                                                         \
                    X->bk = T;                                                                                         \
                    X->parent = 0;                                                                                     \
                    break;                                                                                             \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    }

#define unlink_large_chunk(M, X)                                                                                       \
    {                                                                                                                  \
        tchunkptr XP = X->parent;                                                                                      \
        tchunkptr R;                                                                                                   \
        if (X->bk != X) {                                                                                              \
            tchunkptr F = X->fd;                                                                                       \
            R = X->bk;                                                                                                 \
            F->bk = R;                                                                                                 \
            R->fd = F;                                                                                                 \
        } else {                                                                                                       \
            tchunkptr* RP;                                                                                             \
            if (((R = *(RP = &(X->child[1]))) != 0) || ((R = *(RP = &(X->child[0]))) != 0)) {                          \
                tchunkptr* CP;                                                                                         \
                while ((*(CP = &(R->child[1])) != 0) || (*(CP = &(R->child[0])) != 0)) {                               \
                    R = *(RP = CP);                                                                                    \
                }                                                                                                      \
                *RP = 0;                                                                                               \
            }                                                                                                          \
        }                                                                                                              \
        if (XP != 0) {                                                                                                 \
            tbinptr* H = treebin_at(M, X->index);                                                                      \
            if (X == *H) {                                                                                             \
                if ((*H = R) == 0)                                                                                     \
                    clear_treemap(M, X->index);                                                                        \
            } else {                                                                                                   \
                if (XP->child[0] == X)                                                                                 \
                    XP->child[0] = R;                                                                                  \
                else                                                                                                   \
                    XP->child[1] = R;                                                                                  \
            }                                                                                                          \
            if (R != 0) {                                                                                              \
                tchunkptr C0, C1;                                                                                      \
                R->parent = XP;                                                                                        \
                if ((C0 = X->child[0]) != 0) {                                                                         \
                    R->child[0] = C0;                                                                                  \
                    C0->parent = R;                                                                                    \
                }                                                                                                      \
                if ((C1 = X->child[1]) != 0) {                                                                         \
                    R->child[1] = C1;                                                                                  \
                    C1->parent = R;                                                                                    \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    }

#define insert_chunk(M, P, S)                                                                                          \
    if (is_small(S))                                                                                                   \
        insert_small_chunk(M, P, S) else {                                                                             \
            insert_large_chunk(M, ((tchunkptr)(P)), S);                                                                \
        }

#define unlink_chunk(M, P, S)                                                                                          \
    if (is_small(S))                                                                                                   \
        unlink_small_chunk(M, P, S) else {                                                                             \
            unlink_large_chunk(M, ((tchunkptr)(P)));                                                                   \
        }

/* ----------------------------------- functions ----------------------------------- */

static msegmentptr segment_holding(mstate m, char* addr) {
    msegmentptr sp = &m->seg;
    for (;;) {
        if (addr >= sp->base && addr < sp->base + sp->size)
            return sp;
        if ((sp = sp->next) == 0)
            return 0;
    }
}

static int has_segment_link(mstate m, msegmentptr ss) {
    msegmentptr sp = &m->seg;
    for (;;) {
        if ((char*)sp >= ss->base && (char*)sp < ss->base + ss->size)
            return 1;
        if ((sp = sp->next) == 0)
            return 0;
    }
}

static void init_mparams(void) {
    if (mparams.page_size == 0) {
        mparams.magic = 0x58585858U;
        mparams.mmap_threshold = DEFAULT_MMAP_THRESHOLD;
        mparams.trim_threshold = DEFAULT_TRIM_THRESHOLD;
        mparams.default_mflags = USE_NONCONTIGUOUS_BIT;
        mparams.page_size = 4096;
        mparams.granularity = DEFAULT_GRANULARITY;
    }
}

static int change_mparam(int param_number, int value) {
    size_t val = (size_t)value;
    init_mparams();
    switch (param_number) {
    case M_TRIM_THRESHOLD:
        mparams.trim_threshold = val;
        return 1;
    case M_GRANULARITY:
        if (val >= mparams.page_size && ((val & (val - 1)) == 0)) {
            mparams.granularity = val;
            return 1;
        } else
            return 0;
    case M_MMAP_THRESHOLD:
        mparams.mmap_threshold = val;
        return 1;
    default:
        return 0;
    }
}

static void internal_malloc_stats(mstate m) {
}

static void* mmap_alloc(mstate m, size_t nb) {
    return 0;
}

static mchunkptr mmap_resize(mstate m, mchunkptr oldp, size_t nb) {
    size_t oldsize = chunksize(oldp);
    if (is_small(nb))
        return 0;
    if (oldsize >= nb + SIZE_T_SIZE && (oldsize - nb) <= (mparams.granularity << 1))
        return oldp;
    else {
        size_t offset = oldp->prev_foot & ~IS_MMAPPED_BIT;
        size_t oldmmsize = oldsize + offset + MMAP_FOOT_PAD;
        size_t newmmsize = granularity_align(nb + 6 * SIZE_T_SIZE + CHUNK_ALIGN_MASK);
        char* cp = (char*)CALL_MREMAP((char*)oldp - offset, oldmmsize, newmmsize, 1);
        if (cp != CMFAIL) {
            mchunkptr newp = (mchunkptr)(cp + offset);
            size_t psize = newmmsize - offset - MMAP_FOOT_PAD;
            newp->head = (psize | CINUSE_BIT);
            chunk_plus_offset(newp, psize)->head = FENCEPOST_HEAD;
            chunk_plus_offset(newp, psize + SIZE_T_SIZE)->head = 0;
            if (cp < m->least_addr)
                m->least_addr = cp;
            if ((m->footprint += newmmsize - oldmmsize) > m->max_footprint)
                m->max_footprint = m->footprint;
            return newp;
        }
    }
    return 0;
}

static void init_top(mstate m, mchunkptr p, size_t psize) {
    size_t offset = align_offset(chunk2mem(p));
    p = (mchunkptr)((char*)p + offset);
    psize -= offset;

    m->top = p;
    m->topsize = psize;
    p->head = psize | PINUSE_BIT;
    chunk_plus_offset(p, psize)->head = TOP_FOOT_SIZE;
    m->trim_check = mparams.trim_threshold;
}

static void init_bins(mstate m) {
    bindex_t i;
    for (i = 0; i < NSMALLBINS; ++i) {
        sbinptr bin = smallbin_at(m, i);
        bin->fd = bin->bk = bin;
    }
}

static void* prepend_alloc(mstate m, char* newbase, char* oldbase, size_t nb) {
    mchunkptr p = align_as_chunk(newbase);
    mchunkptr oldfirst = align_as_chunk(oldbase);
    size_t psize = (char*)oldfirst - (char*)p;
    mchunkptr q = chunk_plus_offset(p, nb);
    size_t qsize = psize - nb;
    set_size_and_pinuse_of_inuse_chunk(m, p, nb);

    if (oldfirst == m->top) {
        m->topsize += qsize;
        m->top = q;
        q->head = m->topsize | PINUSE_BIT;
    } else if (oldfirst == m->dv) {
        size_t dsize = m->dvsize += qsize;
        m->dv = q;
        set_size_and_pinuse_of_free_chunk(q, dsize);
    } else {
        if (!cinuse(oldfirst)) {
            size_t nsize = chunksize(oldfirst);
            unlink_chunk(m, oldfirst, nsize);
            oldfirst = chunk_plus_offset(oldfirst, nsize);
            qsize += nsize;
        }
        set_free_with_pinuse(q, qsize, oldfirst);
        insert_chunk(m, q, qsize);
    }

    return chunk2mem(p);
}

static void add_segment(mstate m, char* tbase, size_t tsize, flag_t mmapped) {
    char* old_top = (char*)m->top;
    msegmentptr oldsp = segment_holding(m, old_top);
    char* old_end = oldsp->base + oldsp->size;
    char* rawsp = old_end - (pad_request(sizeof(struct malloc_segment)) + 4 * SIZE_T_SIZE + CHUNK_ALIGN_MASK);
    size_t offset = align_offset(chunk2mem(rawsp));
    char* asp = rawsp + offset;
    char* csp = (asp < (old_top + MIN_CHUNK_SIZE)) ? old_top : asp;
    msegmentptr ss = (msegmentptr)(chunk2mem(csp));
    mchunkptr p = chunk_plus_offset(csp, pad_request(sizeof(struct malloc_segment)));

    init_top(m, (mchunkptr)tbase, tsize - TOP_FOOT_SIZE);

    set_size_and_pinuse_of_inuse_chunk(m, (mchunkptr)csp, pad_request(sizeof(struct malloc_segment)));
    ss->base = tbase;
    ss->size = tsize;
    ss->sflags = mmapped;
    ss->next = m->seg.next;
    m->seg.next = ss;

    for (;;) {
        mchunkptr nextp = chunk_plus_offset(p, SIZE_T_SIZE);
        p->head = FENCEPOST_HEAD;
        if ((char*)(&(nextp->head)) < old_end)
            p = nextp;
        else
            break;
    }

    if (csp != old_top) {
        mchunkptr p = (mchunkptr)old_top;
        size_t psize = csp - old_top;
        mchunkptr tn = chunk_plus_offset(p, psize);
        set_free_with_pinuse(p, psize, tn);
        insert_chunk(m, p, psize);
    }
}

static void* sys_alloc(mstate m, size_t nb) {
    init_mparams();
    MALLOC_FAILURE_ACTION;
    return 0;
}

static int sys_trim(mstate m, size_t pad) {
    size_t released = 0;
    if (pad < MAX_REQUEST && is_initialized(m)) {
        pad += TOP_FOOT_SIZE;
        if (m->topsize > pad) {
            size_t unit = mparams.granularity;
            msegment pseg;
            size_t extra = ((m->topsize - pad + (unit - SIZE_T_ONE)) / unit - SIZE_T_ONE) * unit;
            msegmentptr sp = segment_holding(m, (char*)m->top);
            asm volatile("nop"); /* the original has a nop here */

            if (!is_extern_segment(sp)) {
                if (is_mmapped_segment(sp)) {
                    if (HAVE_MMAP && sp->size >= extra && !has_segment_link(m, sp)) {
                        size_t newsize = sp->size - extra;
                        if ((CALL_MREMAP(sp->base, sp->size, newsize, 0) != MFAIL) ||
                            (CALL_MUNMAP(sp->base + newsize, extra) == 0)) {
                            released = extra;
                        }
                    }
                } else if (HAVE_MORECORE) {
                    if (extra >= HALF_MAX_SIZE_T)
                        extra = (HALF_MAX_SIZE_T) + SIZE_T_ONE - unit;
                    {
                        char* old_br = (char*)(CALL_MORECORE(0));
                        if (old_br == sp->base + sp->size) {
                            char* rel_br = (char*)(CALL_MORECORE(-extra));
                            char* new_br = (char*)(CALL_MORECORE(0));
                            if (rel_br != CMFAIL && new_br < old_br)
                                released = old_br - new_br;
                        }
                    }
                }
            }

            if (released != 0) {
                sp->size -= released;
                m->footprint -= released;
                init_top(m, m->top, m->topsize - released);
            }
        }
        if (released == 0)
            m->trim_check = MAX_SIZE_T;
    }
    return (released != 0) ? 1 : 0;
}

static void* tmalloc_large(mstate m, size_t nb) {
    tchunkptr v = 0;
    size_t rsize = -nb;
    tchunkptr t;
    bindex_t idx;
    compute_tree_index(nb, idx);

    if ((t = *treebin_at(m, idx)) != 0) {
        size_t sizebits = nb << leftshift_for_tree_index(idx);
        tchunkptr rst = 0;
        for (;;) {
            tchunkptr rt;
            size_t trem = chunksize(t) - nb;
            if (trem < rsize) {
                v = t;
                if ((rsize = trem) == 0)
                    break;
            }
            rt = t->child[1];
            t = t->child[(sizebits >> (SIZE_T_BITSIZE - SIZE_T_ONE)) & 1];
            if (rt != 0 && rt != t)
                rst = rt;
            if (t == 0) {
                t = rst;
                break;
            }
            sizebits <<= 1;
        }
    }

    if (t == 0 && v == 0) {
        binmap_t leftbits = left_bits(idx2bit(idx)) & m->treemap;
        if (leftbits != 0) {
            bindex_t i;
            binmap_t leastbit = least_bit(leftbits);
            compute_bit2idx(leastbit, i);
            t = *treebin_at(m, i);
        }
    }

    while (t != 0) {
        size_t trem = chunksize(t) - nb;
        if (trem < rsize) {
            rsize = trem;
            v = t;
        }
        t = leftmost_child(t);
    }

    if (v != 0 && rsize < (size_t)(m->dvsize - nb)) {
        mchunkptr r = chunk_plus_offset(v, nb);
        unlink_large_chunk(m, v);
        if (rsize < MIN_CHUNK_SIZE)
            set_inuse_and_pinuse(m, v, (rsize + nb));
        else {
            set_size_and_pinuse_of_inuse_chunk(m, v, nb);
            set_size_and_pinuse_of_free_chunk(r, rsize);
            insert_chunk(m, r, rsize);
        }
        clear_tag(v);
        return chunk2mem(v);
    }
    return 0;
}

static void* tmalloc_small(mstate m, size_t nb) {
    tchunkptr t, v;
    size_t rsize;
    bindex_t i;
    binmap_t leastbit = least_bit(m->treemap);
    compute_bit2idx(leastbit, i);

    v = t = *treebin_at(m, i);
    rsize = chunksize(t) - nb;

    while ((t = leftmost_child(t)) != 0) {
        size_t trem = chunksize(t) - nb;
        if (trem < rsize) {
            rsize = trem;
            v = t;
        }
    }

    {
        mchunkptr r = chunk_plus_offset(v, nb);
        unlink_large_chunk(m, v);
        if (rsize < MIN_CHUNK_SIZE)
            set_inuse_and_pinuse(m, v, (rsize + nb));
        else {
            set_size_and_pinuse_of_inuse_chunk(m, v, nb);
            set_size_and_pinuse_of_free_chunk(r, rsize);
            replace_dv(m, r, rsize);
        }
        clear_tag(v);
        return chunk2mem(v);
    }
}

static void* internal_realloc(mstate m, void* oldmem, size_t bytes) {
    if (bytes >= MAX_REQUEST) {
        MALLOC_FAILURE_ACTION;
        return 0;
    }
    {
        mchunkptr oldp = mem2chunk(oldmem);
        size_t oldsize = chunksize(oldp);
        mchunkptr next = chunk_plus_offset(oldp, oldsize);
        mchunkptr newp = 0;
        void* extra = 0;

        size_t nb = request2size(bytes);
        if (oldsize >= nb) {
            size_t rsize = oldsize - nb;
            newp = oldp;
            if (rsize >= MIN_CHUNK_SIZE) {
                mchunkptr remainder = chunk_plus_offset(newp, nb);
                set_inuse(m, newp, nb);
                set_inuse_and_pinuse(m, remainder, rsize);
                extra = chunk2mem(remainder);
            }
        } else if (next == m->top && oldsize + m->topsize > nb) {
            size_t newtopsize = oldsize + m->topsize - nb;
            mchunkptr newtop = chunk_plus_offset(oldp, nb);
            set_inuse(m, oldp, nb);
            newtop->head = newtopsize | PINUSE_BIT;
            m->top = newtop;
            m->topsize = newtopsize;
            newp = oldp;
        }

        if (newp != 0) {
            if (extra != 0) {
                mspace_free(m, extra);
            }
            clear_tag(newp);
            return chunk2mem(newp);
        } else {
            void* newmem = mspace_malloc(m, bytes);
            if (newmem != 0) {
                size_t oc = oldsize - CHUNK_OVERHEAD;
                memcpy(newmem, oldmem, (oc < bytes) ? oc : bytes);
                mspace_free(m, oldmem);
            }
            return newmem;
        }
    }
}

static void* internal_memalign(mstate m, size_t alignment, size_t bytes) {
    if (alignment <= MALLOC_ALIGNMENT)
        return mspace_malloc(m, bytes);
    if (alignment < MIN_CHUNK_SIZE)
        alignment = MIN_CHUNK_SIZE;
    if ((alignment & (alignment - SIZE_T_ONE)) != 0) {
        size_t a = MALLOC_ALIGNMENT << 1;
        while (a < alignment)
            a <<= 1;
        alignment = a;
    }

    if (bytes >= MAX_REQUEST - alignment) {
        MALLOC_FAILURE_ACTION;
    } else {
        size_t nb = request2size(bytes);
        char* mem = (char*)mspace_malloc(m, nb + alignment + MIN_CHUNK_SIZE - CHUNK_OVERHEAD);
        if (mem != 0) {
            void* leader = 0;
            void* trailer = 0;
            mchunkptr p = mem2chunk(mem);

            if ((((size_t)(mem)) % alignment) != 0) {
                char* brk = (char*)mem2chunk((size_t)(((size_t)(mem + alignment - SIZE_T_ONE)) & -alignment));
                char* pos = ((size_t)(brk - (char*)(p)) >= MIN_CHUNK_SIZE) ? brk : brk + alignment;
                size_t leadsize = pos - (char*)(p);
                size_t newsize = chunksize(p) - leadsize;

                set_inuse(m, (mchunkptr)pos, newsize);
                set_inuse(m, p, leadsize);
                leader = mem;
                p = (mchunkptr)pos;
            }

            {
                size_t size = chunksize(p);
                if (size > nb + MIN_CHUNK_SIZE) {
                    size_t remainder_size = size - nb;
                    mchunkptr remainder = chunk_plus_offset(p, nb);
                    set_inuse(m, p, nb);
                    set_inuse_and_pinuse(m, remainder, remainder_size);
                    trailer = chunk2mem(remainder);
                }
            }

            if (leader != 0) {
                mspace_free(m, leader);
            }
            if (trailer != 0) {
                mspace_free(m, trailer);
            }
            return chunk2mem(p);
        }
    }
    return 0;
}

static void** ialloc(mstate m, size_t n_elements, size_t* sizes, int opts, void* chunks[]) {
    size_t element_size;
    size_t contents_size;
    size_t array_size;
    void* mem;
    mchunkptr p;
    size_t remainder_size;
    void** marray;
    mchunkptr array_chunk;
    size_t size;
    size_t i;

    if (chunks != 0) {
        if (n_elements == 0)
            return chunks;
        marray = chunks;
        array_size = 0;
    } else {
        if (n_elements == 0)
            return (void**)mspace_malloc(m, 0);
        marray = 0;
        array_size = request2size(n_elements * (sizeof(void*)));
    }

    if (opts & 0x1) {
        element_size = request2size(*sizes);
        contents_size = n_elements * element_size;
    } else {
        element_size = 0;
        contents_size = 0;
        for (i = 0; i != n_elements; ++i)
            contents_size += request2size(sizes[i]);
    }

    size = contents_size + array_size;

    mem = mspace_malloc(m, size - CHUNK_OVERHEAD);
    if (mem == 0)
        return 0;

    p = mem2chunk(mem);
    remainder_size = chunksize(p);

    if (opts & 0x2) {
        memset((size_t*)mem, 0, remainder_size - SIZE_T_SIZE - array_size);
    }

    if (marray == 0) {
        size_t array_chunk_size;
        array_chunk = chunk_plus_offset(p, contents_size);
        array_chunk_size = remainder_size - contents_size;
        marray = (void**)(chunk2mem(array_chunk));
        set_size_and_pinuse_of_inuse_chunk(m, array_chunk, array_chunk_size);
        remainder_size = contents_size;
    }

    for (i = 0;; ++i) {
        marray[i] = chunk2mem(p);
        if (i != n_elements - 1) {
            if (element_size != 0)
                size = element_size;
            else
                size = request2size(sizes[i]);
            remainder_size -= size;
            set_size_and_pinuse_of_inuse_chunk(m, p, size);
            p = chunk_plus_offset(p, size);
        } else {
            set_size_and_pinuse_of_inuse_chunk(m, p, remainder_size);
            break;
        }
    }

    return marray;
}

static mstate init_user_mstate(char* tbase, size_t tsize) {
    mchunkptr mn;
    mchunkptr msp = align_as_chunk(tbase);
    mstate m = (mstate)(chunk2mem(msp));
    memset(m, 0, pad_request(sizeof(struct malloc_state)));
    msp->head = (pad_request(sizeof(struct malloc_state)) | PINUSE_BIT | CINUSE_BIT);
    m->seg.base = m->least_addr = tbase;
    m->seg.size = m->footprint = m->max_footprint = tsize;
    m->magic = mparams.magic;
    m->mflags = mparams.default_mflags;
    disable_contiguous(m);
    init_bins(m);
    mn = next_chunk(mem2chunk(m));
    init_top(m, mn, (size_t)((tbase + tsize) - (char*)mn) - TOP_FOOT_SIZE);
    return m;
}

mspace create_mspace(size_t capacity, int locked) {
    init_mparams();
    return 0;
}

mspace create_mspace_with_base(void* base, size_t capacity, int locked) {
    mstate m = 0;
    size_t msize = pad_request(sizeof(struct malloc_state));
    init_mparams();

    if (capacity > msize + TOP_FOOT_SIZE && capacity < (size_t) - (msize + TOP_FOOT_SIZE + mparams.page_size)) {
        m = init_user_mstate((char*)base, capacity);
    }
    return (mspace)m;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/dlmalloc", destroy_mspace);

#if 0
/* Not matching. The original is: lw seg.next; beqz; lw next (A); bnezl L, lw (entry test + body); L: bnezl L, lw.
 * That is "if (sp) { sp = sp->next; while (sp) sp = sp->next; }" with the loop exit test duplicated, but jump2
 * cross-jumps the peeled "lw; beqz" into the loop (the insn before both tests is the same lw), giving an
 * unrotated loop (40 bytes). An asm statement between them (volatile or "=r"/"0" pass-through) stops the merge
 * but then reorg leaves the second test's delay slot unfilled (64 bytes). The do-while below (one loop test,
 * 48 bytes), the real dlmalloc loop over &ms->seg (tests &seg, addiu), for/while forms and a volatile load
 * for the peeled step were tried too. */
size_t destroy_mspace(mspace msp) {
    mstate ms = (mstate)msp;
    msegmentptr sp = ms->seg.next;
    msegmentptr next;
    if (sp != 0) {
        do {
            next = sp->next;
            sp = next;
        } while (sp != 0);
    }
    return 0;
}
#endif

void* mspace_malloc(mspace msp, size_t bytes) {
    mstate ms = (mstate)msp;
    void* mem;
    size_t nb;
    if (bytes <= MAX_SMALL_REQUEST) {
        bindex_t idx;
        binmap_t smallbits;
        nb = (bytes < MIN_REQUEST) ? MIN_CHUNK_SIZE : pad_request(bytes);
        idx = small_index(nb);
        smallbits = ms->smallmap >> idx;

        if ((smallbits & 0x3U) != 0) {
            mchunkptr b, p;
            idx += ~smallbits & 1;
            b = smallbin_at(ms, idx);
            p = b->fd;
            unlink_first_small_chunk(ms, b, p, idx);
            set_inuse_and_pinuse(ms, p, small_index2size(idx));
            clear_tag(p);
            mem = chunk2mem(p);
            goto postaction;
        } else if (nb > ms->dvsize) {
            if (smallbits != 0) {
                mchunkptr b, p, r;
                size_t rsize;
                bindex_t i;
                binmap_t leftbits = (smallbits << idx) & left_bits(idx2bit(idx));
                binmap_t leastbit = least_bit(leftbits);
                compute_bit2idx(leastbit, i);
                b = smallbin_at(ms, i);
                p = b->fd;
                unlink_first_small_chunk(ms, b, p, i);
                rsize = small_index2size(i) - nb;
                set_size_and_pinuse_of_inuse_chunk(ms, p, nb);
                r = chunk_plus_offset(p, nb);
                set_size_and_pinuse_of_free_chunk(r, rsize);
                replace_dv(ms, r, rsize);
                clear_tag(p);
                mem = chunk2mem(p);
                goto postaction;
            } else if (ms->treemap != 0 && (mem = tmalloc_small(ms, nb)) != 0) {
                goto postaction;
            }
        }
    } else if (bytes >= MAX_REQUEST)
        nb = MAX_SIZE_T;
    else {
        nb = pad_request(bytes);
        if (ms->treemap != 0 && (mem = tmalloc_large(ms, nb)) != 0) {
            goto postaction;
        }
    }

    if (nb <= ms->dvsize) {
        size_t rsize = ms->dvsize - nb;
        mchunkptr p = ms->dv;
        if (rsize >= MIN_CHUNK_SIZE) {
            mchunkptr r = ms->dv = chunk_plus_offset(p, nb);
            ms->dvsize = rsize;
            set_size_and_pinuse_of_free_chunk(r, rsize);
            set_size_and_pinuse_of_inuse_chunk(ms, p, nb);
        } else {
            size_t dvs = ms->dvsize;
            ms->dvsize = 0;
            ms->dv = 0;
            set_inuse_and_pinuse(ms, p, dvs);
        }
        clear_tag(p);
        mem = chunk2mem(p);
        goto postaction;
    } else if (nb < ms->topsize) {
        size_t rsize = ms->topsize -= nb;
        mchunkptr p = ms->top;
        mchunkptr r = ms->top = chunk_plus_offset(p, nb);
        r->head = rsize | PINUSE_BIT;
        set_size_and_pinuse_of_inuse_chunk(ms, p, nb);
        clear_tag(p);
        mem = chunk2mem(p);
        goto postaction;
    }

    mem = 0;

postaction:
    return mem;
}

void mspace_free(mspace msp, void* mem) {
    if (mem != 0) {
        mchunkptr p = mem2chunk(mem);
        mstate fm = (mstate)msp;
        size_t psize = chunksize(p);
        mchunkptr next = chunk_plus_offset(p, psize);
        if (!pinuse(p)) {
            size_t prevsize = p->prev_foot;
            mchunkptr prev = chunk_minus_offset(p, prevsize);
            psize += prevsize;
            p = prev;
            if (p != fm->dv) {
                unlink_chunk(fm, p, prevsize);
            } else if ((next->head & INUSE_BITS) == INUSE_BITS) {
                fm->dvsize = psize;
                set_free_with_pinuse(p, psize, next);
                return;
            }
        }

        if (!cinuse(next)) {
            if (next == fm->top) {
                size_t tsize = fm->topsize += psize;
                fm->top = p;
                p->head = tsize | PINUSE_BIT;
                if (p == fm->dv) {
                    fm->dv = 0;
                    fm->dvsize = 0;
                }
                if (should_trim(fm, tsize))
                    sys_trim(fm, 0);
                return;
            } else if (next == fm->dv) {
                size_t dsize = fm->dvsize += psize;
                fm->dv = p;
                set_size_and_pinuse_of_free_chunk(p, dsize);
                return;
            } else {
                size_t nsize = chunksize(next);
                psize += nsize;
                unlink_chunk(fm, next, nsize);
                set_size_and_pinuse_of_free_chunk(p, psize);
                if (p == fm->dv) {
                    fm->dvsize = psize;
                    return;
                }
            }
        } else
            set_free_with_pinuse(p, psize, next);
        insert_chunk(fm, p, psize);
    }
}

void* mspace_calloc(mspace msp, size_t n_elements, size_t elem_size) {
    void* mem;
    size_t req = 0;
    if (n_elements != 0) {
        req = n_elements * elem_size;
        if (((n_elements | elem_size) & ~(size_t)0xffff) && (req / n_elements != elem_size))
            req = MAX_SIZE_T;
    }
    mem = mspace_malloc(msp, req);
    if (mem != 0)
        memset(mem, 0, req);
    return mem;
}

void* mspace_realloc(mspace msp, void* oldmem, size_t bytes) {
    if (oldmem == 0)
        return mspace_malloc(msp, bytes);
    return internal_realloc((mstate)msp, oldmem, bytes);
}

void* mspace_memalign(mspace msp, size_t alignment, size_t bytes) {
    return internal_memalign((mstate)msp, alignment, bytes);
}

void** mspace_independent_calloc(mspace msp, size_t n_elements, size_t elem_size, void* chunks[]) {
    size_t sz = elem_size;
    return ialloc((mstate)msp, n_elements, &sz, 3, chunks);
}

void** mspace_independent_comalloc(mspace msp, size_t n_elements, size_t sizes[], void* chunks[]) {
    return ialloc((mstate)msp, n_elements, sizes, 0, chunks);
}

int mspace_trim(mspace msp, size_t pad) {
    return sys_trim((mstate)msp, pad);
}

void mspace_malloc_stats(mspace msp) {
    internal_malloc_stats((mstate)msp);
}

size_t mspace_footprint(mspace msp) {
    return ((mstate)msp)->footprint;
}

int mspace_mallopt(int param_number, int value) {
    return change_mparam(param_number, value);
}

void* mspace_malloc_d(mspace msp, size_t bytes, char* msg, unsigned int ra) {
    void* p = mspace_malloc(msp, bytes);
    if (p != 0) {
        mchunkptr pp = mem2chunk(p);
        pp->comment = msg;
        pp->ra = ra;
    }
    return p;
}

void* mspace_realloc_d(mspace msp, void* mem, size_t newsize, char* msg, unsigned int ra) {
    void* p = mspace_realloc(msp, mem, newsize);
    if (p != 0) {
        mchunkptr pp = mem2chunk(p);
        pp->comment = msg;
        pp->ra = ra;
    }
    return p;
}

void* mspace_calloc_d(mspace msp, size_t n_elements, size_t elem_size, char* msg, unsigned int ra) {
    void* p = mspace_calloc(msp, n_elements, elem_size);
    if (p != 0) {
        mchunkptr pp = mem2chunk(p);
        pp->comment = msg;
        pp->ra = ra;
    }
    return p;
}

void* mspace_memalign_d(mspace msp, size_t alignment, size_t bytes, char* msg, unsigned int ra) {
    void* p = mspace_memalign(msp, alignment, bytes);
    if (p != 0) {
        mchunkptr pp = mem2chunk(p);
        pp->comment = msg;
        pp->ra = ra;
    }
    return p;
}

void* mspace_get_next_chunk(mspace msp, void* chunk) {
    mchunkptr p = (mchunkptr)chunk;
    size_t size = chunksize(p);
    p = chunk_plus_offset(p, size);
    if ((char*)p < (char*)((mstate)msp)->top)
        return p;
    return 0;
}

void* mspace_get_top_chunk(mspace msp) {
    return mspace_get_next_chunk(msp, ((mstate)msp)->least_addr);
}

size_t mspace_get_chunk_size(void* chunk) {
    return chunksize((mchunkptr)chunk);
}

int mspace_chunk_inuse(void* chunk) {
    return cinuse((mchunkptr)chunk);
}

int mspace_chunk_is_small(size_t size) {
    return is_small(size);
}

void* mspace_chunk2mem(void* chunk) {
    return chunk2mem(chunk);
}

void* mspace_mem2chunk(void* chunk) {
    return mem2chunk(chunk);
}

void* mspace_get_rest_heap(mspace msp) {
    return ((mstate)msp)->top;
}

char* mspace_get_chunk_comment(void* chunk) {
    return ((mchunkptr)chunk)->comment;
}

unsigned int mspace_get_chunk_ra(void* chunk) {
    return ((mchunkptr)chunk)->ra;
}

int mspace_check_inuse_mem(mspace msp, void* mem) {
    mchunkptr p = mem2chunk(mem);
    if (!((is_aligned(chunk2mem(p))) || (p->head == FENCEPOST_HEAD)))
        return 0;
    if (!cinuse(p))
        return 0;
    if (!next_pinuse(p))
        return 0;
    return pinuse(p) || next_chunk(prev_chunk(p)) == p;
}

size_t mspace_get_rest_heap_size(mspace msp) {
    return ((mstate)msp)->topsize;
}

size_t mspace_get_chunk_size_from_mem(void* mem) {
    return chunksize(mem2chunk(mem));
}

/* Words in other units' data equal these addresses (splat made them references). */
asm(".globl D_00504D20\nD_00504D20 = 0x00504D20\n.globl D_00504D4D\nD_00504D4D = 0x00504D4D");
// SOURCE DATA: .bss
