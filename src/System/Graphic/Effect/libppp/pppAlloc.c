#include "pppAlloc.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include "include_asm.h"

/* pppFree is still assembly; the strings it shares with pppAlloc (the file name, "mng") are defined here with their
 * original labels and pppAlloc's assert uses them too, since gcc would emit its own copies (the unit's .rodata comes
 * from source). Its own message follows pppAlloc's. Once pppFree matches: __FILE__, __LINE__, the literals. */
asm(".section .rodata\n.align 3\n.globl D_00468B00\nD_00468B00:\n"
    ".asciz \"c:/KHCOM/MainApp/System/Graphic/Effect/libppp/pppAlloc.c\"\n"
    ".align 3\n.globl D_00468B40\nD_00468B40:\n.asciz \"mng\"\n.previous");
extern char D_00468B00[];
extern char D_00468B40[];

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

    if (mng == NULL) AssertApp(D_00468B00, 89, D_00468B40, 0);

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

asm(".section .rodata\n.align 3\n.globl D_00468BA0\nD_00468BA0:\n"
    ".asciz \"!!!!!!!!!!!!!!!!!!!!!!!!! Effect pppFree NULL! !!!!!!!!!!!!!!!!!!!!!!!\\n\"\n.previous");

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Effect/libppp/pppAlloc", pppFree);

/* Same instructions as the original except that two temporaries (mcb->before and next) get swapped
 * registers (v1/a1); about 70 equivalent spellings of the merge blocks were tried (an explicit "before" variable
 * at function or block scope, "next" as the temporary, nested ifs, ptr-based accesses, register asm variables). */
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
// SOURCE DATA: .rodata
