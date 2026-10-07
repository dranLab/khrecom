// PARTIAL: added for System/Graphic/2d/2DBase
#ifndef SYSTEM_SJOINT_H
#define SYSTEM_SJOINT_H

#include "khtypes.h"

/* Intrusive singly linked list node (original header name unknown). */
template <class T>
class CSJointT {
private:
    CSJointT<T>* m_pNext;
public:
    CSJointT() : m_pNext(0) {}
    virtual ~CSJointT() {}
    T* getNext() { return (T*)m_pNext; }
protected:
    void setNext(CSJointT<T>* pNext) { m_pNext = pNext; }
};

#endif /* SYSTEM_SJOINT_H */
