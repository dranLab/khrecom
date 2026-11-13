#include "khtypes.h"
#include "System/HeapMan.h"
#include "System/dlmalloc.h"
#include "System/Debug.h"
#include "System/FileIO.h"

struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};

extern "C" {
int CreateSema(SemaParam* param);
int SignalSema(int sid);
int WaitSema(int sid);
int sprintf(char* buf, const char* fmt, ...);
void* malloc(size_t size);
void free(void* buffer);
}

extern char _end[];

// Kept here: main.cpp and DebugProcInfo.cpp still declare their own CHeapMan.













struct _HEAPINFO_F {
    UINT _nSize;
    char _szFile[28];
};

namespace {
mspace s_msp = NULL;
size_t s_heap_size_all = 0;
size_t s_heap_used = 0;
}

namespace EERAM {
static const INT _HEAP_START = (((INT)_end - 1) / 64 + 1) * 64;
static const INT _HEAP_SIZE = 0x2000000 - 0x40000 - _HEAP_START;
}

namespace {
void* s_base;
int s_nSemaID;
}

extern "C" {

void* com_malloc(size_t bytes) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_malloc(s_msp, bytes);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    return p;
}

void* com_realloc(void* mem, size_t newsize) {
    void* p;
    size_t oldsize;

    WaitSema(s_nSemaID);
    oldsize = mem ? mspace_get_chunk_size_from_mem(mem) : 0;
    p = mspace_realloc(s_msp, mem, newsize);
    if (p) {
        s_heap_used = s_heap_used + mspace_get_chunk_size_from_mem(p) - oldsize;
    }
    SignalSema(s_nSemaID);
    return p;
}

void* com_calloc(size_t n_elements, size_t elem_size) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_calloc(s_msp, n_elements, elem_size);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    return p;
}

void* com_memalign(size_t alignment, size_t bytes) {
    void* p;

    WaitSema(s_nSemaID);
    p = mspace_memalign(s_msp, alignment, bytes);
    if (p) {
        s_heap_used += mspace_get_chunk_size_from_mem(p);
    }
    SignalSema(s_nSemaID);
    return p;
}

void com_free(void* mem) {
    if (mem) {
        WaitSema(s_nSemaID);
        KH_DEBUG(if (!mspace_check_inuse_mem(s_msp, mem)) {)
            KH_DEBUG(TRACE_COL(_TTY_REV_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!! \203\201\203\202\203\212\223\361\217d\212J\225\372\202\314\211\302\224\134\220\253\202\252\202\240\202\350\202\334\202\267 !!!!!!!!!!!!!!!!!!!!!!!!!!!\n");)
        KH_DEBUG(})
        size_t size = mspace_get_chunk_size_from_mem(mem);
        s_heap_used -= size;
        mspace_free(s_msp, mem);
        SignalSema(s_nSemaID);
    }
}

void* libc_malloc(size_t size) {
    return malloc(size);
}

void libc_free(void* buffer) {
    free(buffer);
}

KH_DEBUG(void* (com_malloc_d)(size_t bytes, char* msg, unsigned int ra) {)
    KH_DEBUG(void* p;)

    KH_DEBUG(WaitSema(s_nSemaID);)
    KH_DEBUG(p = mspace_malloc_d(s_msp, bytes, msg, ra);)
    KH_DEBUG(if (p) {)
        KH_DEBUG(s_heap_used += mspace_get_chunk_size_from_mem(p);)
    KH_DEBUG(})
    KH_DEBUG(SignalSema(s_nSemaID);)
    KH_DEBUG(if (!p) {)
        KH_DEBUG(TRACE_COL(_TTY_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!!malloc Faild Size:%08xbytes ra:%08x!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", bytes, ra);)
    KH_DEBUG(})
    KH_DEBUG(return p;)
KH_DEBUG(})

KH_DEBUG(void* (com_realloc_d)(void* mem, size_t newsize, char* msg, unsigned int ra) {)
    KH_DEBUG(void* p;)
    KH_DEBUG(size_t oldsize;)

    KH_DEBUG(WaitSema(s_nSemaID);)
    KH_DEBUG(oldsize = mem ? mspace_get_chunk_size_from_mem(mem) : 0;)
    KH_DEBUG(p = mspace_realloc_d(s_msp, mem, newsize, msg, ra);)
    KH_DEBUG(if (p) {)
        KH_DEBUG(s_heap_used = s_heap_used + mspace_get_chunk_size_from_mem(p) - oldsize;)
    KH_DEBUG(})
    KH_DEBUG(SignalSema(s_nSemaID);)
    KH_DEBUG(if (!p) {)
        KH_DEBUG(TRACE_COL(_TTY_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!!realloc Faild Size:%08xbytes ra:%08x!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", newsize, ra);)
    KH_DEBUG(})
    KH_DEBUG(return p;)
KH_DEBUG(})

KH_DEBUG(void* (com_calloc_d)(size_t n_elements, size_t elem_size, char* msg, unsigned int ra) {)
    KH_DEBUG(void* p;)

    KH_DEBUG(WaitSema(s_nSemaID);)
    KH_DEBUG(p = mspace_calloc_d(s_msp, n_elements, elem_size, msg, ra);)
    KH_DEBUG(if (p) {)
        KH_DEBUG(s_heap_used += mspace_get_chunk_size_from_mem(p);)
    KH_DEBUG(})
    KH_DEBUG(SignalSema(s_nSemaID);)
    KH_DEBUG(if (!p) {)
        KH_DEBUG(TRACE_COL(_TTY_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!!calloc Faild Size:%08xbytes ra:%08x!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", n_elements * elem_size, ra);)
    KH_DEBUG(})
    KH_DEBUG(return p;)
KH_DEBUG(})

KH_DEBUG(void* (com_memalign_d)(size_t alignment, size_t bytes, char* msg, unsigned int ra) {)
    KH_DEBUG(void* p;)

    KH_DEBUG(WaitSema(s_nSemaID);)
    KH_DEBUG(p = mspace_memalign_d(s_msp, alignment, bytes, msg, ra);)
    KH_DEBUG(if (p) {)
        KH_DEBUG(s_heap_used += mspace_get_chunk_size_from_mem(p);)
    KH_DEBUG(})
    KH_DEBUG(SignalSema(s_nSemaID);)
    KH_DEBUG(if (!p) {)
        KH_DEBUG(TRACE_COL(_TTY_RED, "!!!!!!!!!!!!!!!!!!!!!!!!!!!memalign Faild Size:%08xbytes ra:%08x!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", bytes, ra);)
    KH_DEBUG(})
    KH_DEBUG(return p;)
KH_DEBUG(})

KH_DEBUG(static char* s_pszInuse[2] = { "used", "free" };)
KH_DEBUG(static char* s_pszSize[2] = { "small ", "normal" };)

KH_DEBUG(static void _report_memory_leak(BOOL bLeak) {)
    KH_DEBUG(char szComment[64];)
    KH_DEBUG(int nASize;)
    KH_DEBUG(int nFSize;)
    KH_DEBUG(int nCount;)
    KH_DEBUG(void* pChunk;)
    KH_DEBUG(size_t size;)
    KH_DEBUG(char* pszComment;)
    KH_DEBUG(UINT nRA;)
    KH_DEBUG(TtyColor nCol;)

    KH_DEBUG(nCount = 0;)
    KH_DEBUG(nASize = 0;)
    KH_DEBUG(szComment[63] = 0;)
    KH_DEBUG(nFSize = 0;)
    KH_DEBUG(pChunk = mspace_get_top_chunk(s_msp);)
    KH_DEBUG(while (pChunk) {)
        KH_DEBUG(size = mspace_get_chunk_size(pChunk) - 12;)
        KH_DEBUG(if (!bLeak || mspace_chunk_inuse(pChunk)) {)
            KH_DEBUG(pszComment = mspace_get_chunk_comment(pChunk);)
            KH_DEBUG(nRA = mspace_get_chunk_ra(pChunk);)
            KH_DEBUG(if ((UINT)pszComment <= 0xFFFFF || (UINT)pszComment >= (UINT)(EERAM::_HEAP_START + EERAM::_HEAP_SIZE)) {)
                KH_DEBUG(pszComment = "-";)
            KH_DEBUG(})
            KH_DEBUG(if (nCount == 0) {)
                KH_DEBUG(if (bLeak) {)
                    KH_DEBUG(TRACE("---------------- memory leak report ----------------\n");)
                KH_DEBUG(} else {)
                    KH_DEBUG(TRACE("---------------- memory report ----------------\n");)
                KH_DEBUG(})
                KH_DEBUG(TRACE(" ADDRESS   SIZE    TYPE\n");)
            KH_DEBUG(})
            KH_DEBUG(if (mspace_chunk_inuse(pChunk)) {)
                KH_DEBUG(nASize += size + 12;)
                KH_DEBUG(nCol = _TTY_RED;)
                KH_DEBUG(strncpy(szComment, pszComment, 63);)
                KH_DEBUG(pszComment = szComment;)
            KH_DEBUG(} else {)
                KH_DEBUG(nFSize += size + 12;)
                KH_DEBUG(nCol = _TTY_BLUE;)
                KH_DEBUG(sprintf(szComment, "FREE ");)
                KH_DEBUG(strncpy(szComment + 5, pszComment, 58);)
                KH_DEBUG(pszComment = szComment;)
            KH_DEBUG(})
            KH_DEBUG(nCount++;)
            KH_DEBUG(TRACE_COL(nCol, "%8x%7x  %-6s   ->  %s (RA:%8x)\n", mspace_chunk2mem(pChunk), size,
                      s_pszSize[mspace_chunk_is_small(size) ? 0 : 1], pszComment, nRA);)
        KH_DEBUG(})
        KH_DEBUG(pChunk = mspace_get_next_chunk(s_msp, pChunk);)
    KH_DEBUG(})
    KH_DEBUG(if (nCount) {)
        KH_DEBUG(if (!bLeak) {)
            KH_DEBUG(TRACE("All:%08x HoleFree:%08x AllFree:%08x Use:%08x(%f%%) Start:%08x End:%08x\n",
                  CHeapMan::getAllHeapSize(), nFSize, CHeapMan::getAllHeapSize() - nASize, nASize,
                  (FLOAT)nASize / CHeapMan::getAllHeapSize() * 100.0f, EERAM::_HEAP_START,
                  EERAM::_HEAP_START + EERAM::_HEAP_SIZE);)
            KH_DEBUG(TRACE("-------------- memory report end --------------\n");)
        KH_DEBUG(} else {)
            KH_DEBUG(TRACE("-------------- memory leak report end --------------\n");)
        KH_DEBUG(})
    KH_DEBUG(})
KH_DEBUG(})

KH_DEBUG(void _heap_dump(char* pszFileName, bool bCreate) {)
    KH_DEBUG(int ihost;)
    KH_DEBUG(char szMes[512];)
    KH_DEBUG(void* pChunk;)
    KH_DEBUG(size_t size;)
    KH_DEBUG(int nFree;)
    KH_DEBUG(char* pszComment;)

    KH_DEBUG(ihost = CFileIO::openFile(pszFileName, 0x602, 0x1FF);)
    KH_DEBUG(if (ihost >= 0) {)
        KH_DEBUG(for (pChunk = mspace_get_top_chunk(s_msp); pChunk; pChunk = mspace_get_next_chunk(s_msp, pChunk)) {)
            KH_DEBUG(size = mspace_get_chunk_size(pChunk);)
            KH_DEBUG(nFree = !mspace_chunk_inuse(pChunk);)
            KH_DEBUG(pszComment = nFree ? NULL : mspace_get_chunk_comment(pChunk);)
            KH_DEBUG(if (!pszComment) {)
                KH_DEBUG(pszComment = "-";)
            KH_DEBUG(})
            KH_DEBUG(sprintf(szMes, "0x%08x, %s, %s, 0x%08x, %s\n", mspace_chunk2mem(pChunk), s_pszInuse[nFree],
                    s_pszSize[mspace_chunk_is_small(size) ? 0 : 1], size, pszComment);)
            KH_DEBUG(CFileIO::writeFile(ihost, szMes, strlen(szMes));)
        KH_DEBUG(})
        KH_DEBUG(sprintf(szMes, "0x%08x, %s, %s, 0x%08x\n", mspace_get_rest_heap(s_msp), "     ", "     ",
                mspace_get_rest_heap_size(s_msp));)
        KH_DEBUG(CFileIO::closeFile(ihost);)
    KH_DEBUG(})
KH_DEBUG(})

KH_DEBUG(void dump_heap(char* pszFileName, bool bCreate) {)
    KH_DEBUG(WaitSema(s_nSemaID);)
    KH_DEBUG(_heap_dump(pszFileName, bCreate);)
    KH_DEBUG(SignalSema(s_nSemaID);)
KH_DEBUG(})

}

void CHeapMan::createHeap(void* base, size_t capacity) {
    SemaParam sparam;

    if (!s_msp) {
        s_base = base;
        s_msp = create_mspace_with_base(base, capacity, 0);
        sparam.initCount = 1;
        sparam.maxCount = 1;
        s_nSemaID = CreateSema(&sparam);
        s_heap_size_all = capacity;
        s_heap_used = capacity - mspace_get_rest_heap_size(s_msp);
    }
}

KH_DEBUG(void CHeapMan::reportMemory() {)
    KH_DEBUG(WaitSema(s_nSemaID);)
    KH_DEBUG(_report_memory_leak(0);)
    KH_DEBUG(SignalSema(s_nSemaID);)
KH_DEBUG(})

void CHeapMan::resetHeap() {
    WaitSema(s_nSemaID);
    KH_DEBUG(_report_memory_leak(1);)
    s_msp = create_mspace_with_base(s_base, s_heap_size_all, 0);
    s_heap_used = s_heap_size_all - mspace_get_rest_heap_size(s_msp);
    SignalSema(s_nSemaID);
}

size_t CHeapMan::getFreeHeapSize() {
    return s_heap_size_all - s_heap_used;
}

size_t CHeapMan::getUsedHeapSize() {
    return s_heap_used;
}

float CHeapMan::getUsedHeapRatio() {
    return (FLOAT)s_heap_used * 100.0f / s_heap_size_all;
}

size_t CHeapMan::getAllHeapSize() {
    return s_heap_size_all;
}

KH_DEBUG(size_t CHeapMan::getMaxFreeBlockSize() {)
    KH_DEBUG(size_t nMax;)
    KH_DEBUG(void* pChunk;)
    KH_DEBUG(size_t rest;)
    KH_DEBUG(size_t size;)

    KH_DEBUG(nMax = 0;)
    KH_DEBUG(WaitSema(s_nSemaID);)
    KH_DEBUG(for (pChunk = mspace_get_top_chunk(s_msp); pChunk; pChunk = mspace_get_next_chunk(s_msp, pChunk)) {)
        KH_DEBUG(if (!mspace_chunk_inuse(pChunk)) {)
            KH_DEBUG(size = mspace_get_chunk_size(pChunk) - 12;)
            KH_DEBUG(if (nMax < size) {)
                KH_DEBUG(nMax = size;)
            KH_DEBUG(})
        KH_DEBUG(})
    KH_DEBUG(})
    KH_DEBUG(rest = mspace_get_rest_heap_size(s_msp);)
    KH_DEBUG(if (nMax < rest) {)
        KH_DEBUG(nMax = rest;)
    KH_DEBUG(})
    KH_DEBUG(SignalSema(s_nSemaID);)
    KH_DEBUG(return nMax;)
KH_DEBUG(})

KH_DEBUG(CHAR* CHeapMan::getDebugComment(void* pMem) {)
    KH_DEBUG(return mspace_get_chunk_comment(mspace_mem2chunk(pMem));)
KH_DEBUG(})

KH_DEBUG(static UINT _analyzeHeapPerFile(_HEAPINFO_F* pOutTbl) {)
    KH_DEBUG(char szFile[32];)
    KH_DEBUG(UINT nCount;)
    KH_DEBUG(void* pChunk;)
    KH_DEBUG(size_t size;)
    KH_DEBUG(CHAR* pszComment;)
    KH_DEBUG(_HEAPINFO_F* pInfo;)
    KH_DEBUG(CHAR* p;)
    KH_DEBUG(UINT i;)

    KH_DEBUG(nCount = 1;)
    KH_DEBUG(strcpy(pOutTbl->_szFile, "unknown");)
    KH_DEBUG(*(UINT*)pOutTbl = 0; /* scalar store: schedules right after the copy, as in the original */)
    KH_DEBUG(for (pChunk = mspace_get_top_chunk(s_msp); pChunk; pChunk = mspace_get_next_chunk(s_msp, pChunk)) {)
        KH_DEBUG(if (!mspace_chunk_inuse(pChunk)) {)
            KH_DEBUG(continue;)
        KH_DEBUG(})
        KH_DEBUG(pInfo = NULL;)
        KH_DEBUG(size = mspace_get_chunk_size(pChunk) - 12;)
        KH_DEBUG(pszComment = mspace_get_chunk_comment(pChunk);)
        KH_DEBUG(if (!pszComment) {)
            KH_DEBUG(pInfo = pOutTbl;)
        KH_DEBUG(} else {)
            KH_DEBUG(p = strrchr(pszComment, '/');)
            KH_DEBUG(if (p) {)
                KH_DEBUG(pszComment = p;)
            KH_DEBUG(})
            KH_DEBUG(strncpy(szFile, pszComment + 1, 27);)
            KH_DEBUG(szFile[27] = 0;)
            KH_DEBUG(p = strrchr(szFile, '(');)
            KH_DEBUG(if (p) {)
                KH_DEBUG(*p = 0;)
            KH_DEBUG(})
            KH_DEBUG(for (i = 1; i < nCount; i++) {)
                KH_DEBUG(if (strcmp(pOutTbl[i]._szFile, szFile) == 0) {)
                    KH_DEBUG(pInfo = &pOutTbl[i];)
                    KH_DEBUG(break;)
                KH_DEBUG(})
            KH_DEBUG(})
            KH_DEBUG(if (!pInfo) {)
                KH_DEBUG(if (nCount >= 1024) {)
                    KH_DEBUG(TRACE("_analyzeHeapPerFile() : \202\261\202\352\210\310\217\343\217W\214v\202\305\202\253\202\334\202\271\202\361\n");)
                    KH_DEBUG(return nCount;)
                KH_DEBUG(})
                KH_DEBUG(pInfo = &pOutTbl[nCount++];)
                KH_DEBUG(strcpy(pInfo->_szFile, szFile);)
                KH_DEBUG(pInfo->_nSize = 0;)
            KH_DEBUG(})
        KH_DEBUG(})
        KH_DEBUG(pInfo->_nSize += size;)
    KH_DEBUG(})
    KH_DEBUG(return nCount;)
KH_DEBUG(})

KH_DEBUG(static void _sortHeapInfoPerFile(const _HEAPINFO_F* pInfoTbl, USHORT* pIdxTbl, UINT nCount) {)
    KH_DEBUG(UINT i;)
    KH_DEBUG(UINT nSize;)
    KH_DEBUG(UINT j;)
    KH_DEBUG(UINT idx;)
    KH_DEBUG(UINT k;)

    KH_DEBUG(pIdxTbl[0] = 0;)
    KH_DEBUG(for (i = 1; i < nCount; i++) {)
        KH_DEBUG(nSize = pInfoTbl[i]._nSize;)
        KH_DEBUG(for (j = 0; j < i; j++) {)
            KH_DEBUG(if (pInfoTbl[pIdxTbl[j]]._nSize < nSize) {)
                KH_DEBUG(for (k = i; k >= j + 1; k--) {)
                    KH_DEBUG(pIdxTbl[k] = pIdxTbl[k - 1];)
                KH_DEBUG(})
                KH_DEBUG(break;)
            KH_DEBUG(})
        KH_DEBUG(})
        KH_DEBUG(pIdxTbl[j] = i;)
    KH_DEBUG(})
KH_DEBUG(})

KH_DEBUG(void dbgReportHeapPerFile() {)
    KH_DEBUG(_HEAPINFO_F infoTbl[1024];)
    KH_DEBUG(UINT nFileCount;)
    KH_DEBUG(USHORT idxTbl[1024];)
    KH_DEBUG(FLOAT fAllSize;)
    KH_DEBUG(UINT i;)
    KH_DEBUG(UINT idx;)
    KH_DEBUG(UINT sum;)

    KH_DEBUG(nFileCount = _analyzeHeapPerFile(infoTbl);)
    KH_DEBUG(_sortHeapInfoPerFile(infoTbl, idxTbl, nFileCount);)
    KH_DEBUG(fAllSize = (FLOAT)s_heap_size_all / 100.0f;)
    KH_DEBUG(TRACE("----------------- heap report -----------------\n");)
    KH_DEBUG(for (i = 0; i < nFileCount; i++) {)
        KH_DEBUG(idx = idxTbl[i];)
        KH_DEBUG(sum = infoTbl[idx]._nSize;)
        KH_DEBUG(TRACE(" %-27s : %8d (%.2f%%)\n", infoTbl[idx]._szFile, sum, sum / fAllSize);)
    KH_DEBUG(})
    KH_DEBUG(TRACE("--------------- heap report end ---------------\n");)
KH_DEBUG(})
// SOURCE DATA: .data .rodata .bss
// SOURCE DATA SLUS_217.99_retail: .bss .data
