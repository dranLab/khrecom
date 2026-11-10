// The original header's inline functions leave "System/Vector.h(252)" in only a few objects' .rodata, so it
// must not get string literals (asserts) until a unit that has them reproduces them.
#ifndef SYSTEM_VECTOR_H
#define SYSTEM_VECTOR_H

#include "khtypes.h"
#include "System/Coord.h" /* FVECTOR, IVECTOR, FMATRIX */

// PARTIAL: added for System/MainTaskFactory. Only __alloc/__getSize are known (out-of-class template members,
// instantiated wherever an inline function uses push_back); the other members are guesses.
#include <string.h>
extern "C" void* (com_malloc_d)(size_t bytes, char* msg, unsigned int ra);
extern "C" void com_free(void* mem);
extern "C" void (AssertApp)(char* pszFile, int nLine, char* pszMsg, char* pszExp);

/* __FILE__ as in the original (the header is found through "-I src" here). Line numbers are not known. */
/* MATCH: path override for System/TaskMan ("c:/KHCOM/MainApp/System/Vector.h") */
#ifndef VECTOR_H_PATH
#define VECTOR_H_PATH "System/Vector.h"
#endif
#line 200 VECTOR_H_PATH
template <class T>
class CVector {
private:
    T* m_pObj;
    INT m_iMax;
    INT m_iIdx;
    INT m_iStatus;

public:
    CVector() : m_pObj(NULL), m_iMax(0), m_iIdx(0), m_iStatus(0) {}
    virtual ~CVector() { __cleanup(); }
    // PARTIAL: reserve added for System/TaskMan
    void reserve(INT iNum) {
        if (m_iMax < iNum) {
            __alloc(iNum);
        }
    }
    // PARTIAL: push_back/clear as in Game/Atel/EventTrap (BOOL result and clear's __init for System/TaskMan)
    BOOL push_back(const T& x) {
        if (m_iIdx >= m_iMax) {
            __alloc(__getSize(m_iMax));
        }
        if (m_iIdx < m_iMax) {
            m_pObj[m_iIdx] = x;
            m_iIdx++;
            return TRUE;
        }
        return FALSE;
    }
    void clear() {
        __cleanup();
        __init();
    }
    INT size() const { return m_iIdx; }
    T& operator[](INT i) {
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
        return m_pObj[i];
    }
    /* PARTIAL: added for Form/DebugWindow (names unknown; the original line of back()'s assert is 162) */
    T* getBuffer() { return m_pObj; }
    T& back() {
#line 162
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
#line 233
        return *(m_pObj + m_iIdx - 1);
    }

    /* PARTIAL: added for System/TaskMan (lines of the asserts from the original) */
#line 150
    T* begin() {
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
        return m_pObj;
    }
#line 161
    T* end() {
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
        return m_pObj + m_iIdx;
    }

private:
    void __init() {
        m_pObj = NULL;
        m_iMax = 0;
        m_iIdx = 0;
        m_iStatus = 0;
    }
    void __cleanup() {
        for (INT i = 0; i < m_iIdx; i++) {
            m_pObj[i].~T();
        }
        __cleanmem();
    }
    void __cleanmem() {
        if (m_pObj != NULL) {
            /* PARTIAL: delete, not com_free (Game/Atel/EventTrap): same code, but the larger RTL makes a synthesized
             * destructor containing it too big to inline, which changes where gcc emits it */
            delete m_pObj;
        }
        m_pObj = NULL;
    }
    T* __alloc(INT iNum);
    INT __getSize(INT iNum);
};

template <class T>
T* CVector<T>::__alloc(INT iNum) {
    if (m_iStatus & 1) {
        return NULL;
    }
    if (iNum < 4) {
        iNum = 4;
    }
    T* p = (T*)com_malloc_d(iNum * sizeof(T), __FILE__ "(252)", (unsigned int)-1);
    if (p != NULL) {
        if (m_pObj != NULL) {
            if (m_iIdx > 0) {
                memcpy(p, m_pObj, sizeof(T) * m_iIdx);
                __cleanmem();
            }
        }
        m_iMax = iNum;
        m_pObj = p;
    } else {
        m_iStatus |= 1;
    }
    return p;
}

template <class T>
INT CVector<T>::__getSize(INT iNum) {
    if (iNum < 256) {
        iNum *= 2;
    } else {
        iNum += 256;
    }
    return iNum;
}

#endif /* SYSTEM_VECTOR_H */
