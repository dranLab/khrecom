#include "pppAlloc.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "include_asm.h"

void pppDumpHeap(pppMemMng *mng) {}

void pppCreateHeap(pppMemMng *mng, long128 *addr, u_word size) {
    pppEMCB *mcb;

    mng->effect_heap_top = (pppEMCB *)addr;
    mng->last_free_heap = NULL;
    mng->effect_heap_bottom = (pppEMCB *)((u_char *)addr + size - sizeof(pppEMCB));

    mcb = (pppEMCB *)addr;
    mcb->before = NULL;
    mcb->next = mng->effect_heap_bottom;
    mcb->owner = 0;

    mcb = mng->effect_heap_bottom;
    mcb->before = mng->effect_heap_top;
    mcb->owner = 1;
    mcb->next = NULL;

    mng->allocSize = 0;
}

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/pppAlloc", pppAlloc);

/* Differs: the original keeps the found block's old next (from the size check) in a register through the
 * split, which needs a third pointer variable; ref lists only mcb and newMcb. The wrap-around block layout
 * and the last_free_heap init (explicit if/else) differ too. */
#if 0
void *pppAlloc(pppMemMng *mng, u_word size) {
    pppEMCB *mcb;
    pppEMCB *newMcb;

#line 89
    if (mng == NULL) AssertApp(__FILE__, __LINE__, "mng", 0);

    size = ((size + 15) & ~15) + sizeof(pppEMCB);
    if (mng->last_free_heap == NULL) {
        mng->last_free_heap = mng->effect_heap_top;
    }
    mcb = mng->last_free_heap->next;

    for (;;) {
        mcb = mcb->before;
        if (mcb == NULL) {
            if (mng->last_free_heap == NULL) {
                TRACE("!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppAlloc error size=%d !!!!!!!!!!!!!!!!!!!!!!!\n", size);
                if (ppvErrorFunc != NULL) {
                    ppvErrorFunc();
                }
                return NULL;
            }
            mcb = mng->effect_heap_bottom;
            mng->last_free_heap = NULL;
            continue;
        }
        if (mcb->owner != 0) {
            continue;
        }
        if ((int)((u_char *)mcb->next - (u_char *)mcb) < (int)size) {
            continue;
        }
        break;
    }

    mng->allocSize += size;
    newMcb = (pppEMCB *)((u_char *)mcb->next - size);
    if (newMcb == mcb || (u_int)((u_char *)newMcb - (u_char *)mcb) < sizeof(pppEMCB)) {
        mng->last_free_heap = NULL;
    } else {
        mng->last_free_heap = mcb;
        newMcb->before = mcb;
        newMcb->next = mcb->next;
        mcb->next->before = newMcb;
        mcb->next = newMcb;
        mcb = newMcb;
    }
    mcb->owner = 1;
    return mcb + 1;
}

#endif

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/pppAlloc", pppFree);

/* Same instructions as the original except that two temporaries (mcb->before and next) get swapped
 * registers (v1/a1); about 60 equivalent spellings of the merge blocks were tried. */
#if 0
void pppFree(pppMemMng *mng, void *ptr) {
    pppEMCB *mcb;
    pppEMCB *next;

#line 179
    if (mng == NULL) AssertApp(__FILE__, __LINE__, "mng", 0);

    if (ptr == NULL) {
        TRACE("!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppFree NULL! !!!!!!!!!!!!!!!!!!!!!!!\n");
        return;
    }

    mcb = (pppEMCB *)ptr - 1;
    mcb->owner = 0;
    mng->last_free_heap = mcb;
    mng->allocSize -= (u_char *)mcb->next - (u_char *)mcb;

    if (mcb->before != NULL && mcb->before->owner == 0) {
        mcb = mcb->before;
        mcb->next = ((pppEMCB *)ptr - 1)->next;
        ((pppEMCB *)ptr - 1)->next->before = mcb;
    }
    next = mcb->next;
    if (next->owner == 0) {
        mcb->next = next->next;
        next->next->before = mcb;
    }
}
#endif
