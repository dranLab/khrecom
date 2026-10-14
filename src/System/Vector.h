// PARTIAL: FVECTOR/FMATRIX layouts shared by all units; the inline operators are added as units need them.
// The original header's inline functions leave "System/Vector.h(252)" in only a few objects' .rodata, so it
// must not get string literals (asserts) until a unit that has them reproduces them.
#ifndef SYSTEM_VECTOR_H
#define SYSTEM_VECTOR_H

#include "khtypes.h"

typedef float _FVECTOR[4];
union FVECTOR {
    _FVECTOR v;
    struct {
        FLOAT x;
        FLOAT y;
        FLOAT z;
        FLOAT w;
    };
    u_long128 _q;
    unsigned long _ul[2];
    unsigned int _ui[4];
    // PARTIAL: added for Game/Player/FriendAladdin
    void set(FLOAT inX, FLOAT inY, FLOAT inZ, FLOAT inW = 1.0f) {
        x = inX;
        y = inY;
        z = inZ;
        w = inW;
    }
    // PARTIAL: added for Game/Player/FriendAriel
    void init() {
        _q = 0;
        w = 1.0f;
    }
};

typedef float _FMATRIX[4][4];
union FMATRIX {
    _FMATRIX m;
    FVECTOR v[4];
    void setTrans(const FVECTOR& vecIn) { v[3] = vecIn; }
};

// PARTIAL: added for System/MainTaskFactory. Only __alloc/__getSize are known (out-of-class template members,
// instantiated wherever an inline function uses push_back); the other members are guesses.
#include <string.h>
extern "C" void* com_malloc_d(size_t bytes, char* msg, unsigned int ra);
extern "C" void com_free(void* mem);
extern "C" void AssertApp(char* pszFile, int nLine, char* pszMsg, char* pszExp);

/* __FILE__ as in the original (the header is found through "-I src" here). Line numbers are not known. */
#line 200 "System/Vector.h"
template <class T>
class CVector {
private:
    T* m_pObj;
    INT m_iMax;
    INT m_iIdx;
    INT m_iStatus;

public:
    CVector() : m_pObj(NULL), m_iMax(0), m_iIdx(0), m_iStatus(0) {}
    virtual ~CVector() { __cleanmem(); }
    BOOL push_back(const T& x) {
        if (m_iIdx >= m_iMax) {
            if (__alloc(__getSize(m_iMax)) == NULL) {
                return 0;
            }
        }
        m_pObj[m_iIdx++] = x;
        return 1;
    }
    INT size() const { return m_iIdx; }
    T& operator[](INT i) {
        if (!(!((m_pObj == __null)))) AssertApp(__FILE__, __LINE__, "!((m_pObj==__null))", 0);
        return m_pObj[i];
    }

private:
    void __cleanmem() {
        if (m_pObj != NULL) {
            com_free(m_pObj);
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
