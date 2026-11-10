#include "pppAlloc.h"
#include "System/MainApp.h"
#include "System/Debug.h"










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

void *pppAlloc(pppMemMng *mng, u_word size) {
    pppEMCB *mcb;
    pppEMCB *newMcb;

    KH_DEBUG(if (mng == NULL) AssertApp(__FILE__, 89, "mng", 0);)

    size = ((size + 15) & ~15) + sizeof(pppEMCB);
    if (mng->last_free_heap == NULL) {
        goto init;
    }
    mcb = mng->last_free_heap->next;
    goto search;

wrap:
    mcb = mng->effect_heap_bottom;
    mng->last_free_heap = NULL;
    goto search;

init:
    mcb = mng->effect_heap_top->next;
    mng->last_free_heap = mng->effect_heap_top;

search:
    for (;;) {
        pppEMCB *next;

        if (mcb->before == NULL) {
            if (mng->last_free_heap != NULL) {
                goto wrap;
            }
            TRACE("!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppAlloc error size=%d !!!!!!!!!!!!!!!!!!!!!!!\n", size);
            if (ppvErrorFunc != NULL) {
                ppvErrorFunc();
            }
            return NULL;
        }
        mcb = mcb->before;
        if (mcb->owner != 0) {
            continue;
        }
        next = mcb->next;
        if ((int)((u_char *)next - (u_char *)mcb) < (int)size) {
            continue;
        }
        mng->allocSize += size;
        newMcb = (pppEMCB *)((u_char *)mcb->next - size);
        if (newMcb != mcb && (u_int)((u_char *)newMcb - (u_char *)mcb) >= sizeof(pppEMCB)) {
            mng->last_free_heap = mcb;
            newMcb->before = mcb;
            newMcb->next = next;
            next->before = newMcb;
            mcb->next = newMcb;
            mcb = newMcb;
        } else {
            mng->last_free_heap = NULL;
        }
        mcb->owner = 1;
        return mcb + 1;
    }
}

/* The original's debug info lists only mcb and next in registers; `before` is a plain local whose every use CSE
 * replaces by the temporary of the `mcb->before != NULL` test (its dead copy is deleted, so it never gets a register and
 * is omitted from the debug info). Reading mcb->before again after the `before->next` store would reload it. */
#line 175
void pppFree(pppMemMng *mng, void *ptr) {
    pppEMCB *mcb;
    pppEMCB *next;
    pppEMCB *before;
#line 179
    KH_DEBUG(if (mng == NULL) AssertApp(__FILE__, __LINE__, "mng", 0);)
    if (ptr == NULL) {
        TRACE("!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppFree NULL! !!!!!!!!!!!!!!!!!!!!!!!\n");
        return;
    }

    mcb = (pppEMCB *)ptr - 1;
    mcb->owner = 0;
    mng->last_free_heap = mcb;

    mng->allocSize -= (u_char *)mcb->next - (u_char *)mcb;

    if (mcb->before != NULL) {
        before = mcb->before;
        if (before->owner == 0) {
            before->next = mcb->next;
            next = mcb->next;
            next->before = before;
            mcb = before;
        }
    }

    next = mcb->next;
    if (next->owner == 0) {
        mcb->next = next->next;
        next = next->next;
        next->before = mcb;
    }
}
// SOURCE DATA: .rodata
