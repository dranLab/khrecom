#ifndef SYSTEM_GRAPHIC_2D_2DBASE_H
#define SYSTEM_GRAPHIC_2D_2DBASE_H

#include "khtypes.h"
#include "System/SJoint.h"

namespace D2 {

class C2DFnt;
struct SeqControl;

enum D2BASE_TYPE {
    D2BASE_TYPE_SPR = 0,
    D2BASE_TYPE_SEQ = 0,
    D2BASE_TYPE_LAY = 0x40000000,
    D2BASE_TYPE_CHK = -0x40000000
};

class C2DBase : public CSJointT<C2DBase> {
private:
    static char m_tblID[64];
    UINT m_uiID;
protected:
    char m_pBaseName[24];
    HANDLE m_hBase;

public:
    C2DBase();
    virtual ~C2DBase();
    static void initialize();
    virtual BOOL init(void* pData, const CHAR* pBaseName) = 0;
    virtual HANDLE create(UINT inNo) = 0;
    virtual void destroy(HANDLE handle) = 0;
    virtual INT getIdMax() = 0;
    virtual INT getUnitIdMax() { return 0; }
    virtual INT getSeqIdMax(UINT inUnitNo) { return 0; }
    virtual INT getSprIdMax(UINT inUnitNo) { return 0; }
    virtual HANDLE createSeq(UINT inUnitNo, UINT inNo, SeqControl* pSeqControl) { return 0; }
    virtual HANDLE createFnt(UINT inSeqIdx, UINT inSeqNo, C2DFnt* p2DFnt, SeqControl* pSeqControl) { return 0; }
    virtual HANDLE replaceFnt(HANDLE hCurrent, UINT inSeqIdx, UINT inSeqNo, C2DFnt* p2DFnt, SeqControl* pSeqControl) { return 0; }
    virtual HANDLE createSpr(UINT inUnitNo, UINT inNo) { return 0; }
    virtual HANDLE create(UINT inNo, SeqControl* pSeqControl) { return 0; }
    virtual TIMEQ getTime(HANDLE handle) { return 0; }
    virtual TIMEQ getTimeMax(HANDLE handle) { return 0; }
    virtual BOOL isEnd(HANDLE handle) { return 0; }
    virtual void enableDisp(HANDLE handle, BOOL bEnable) {}
    CHAR* getBaseName() { return m_pBaseName; }
    HANDLE getBaseHandle() { return m_hBase; }
    D2BASE_TYPE getType();
protected:
    void makeBaseHandle(D2BASE_TYPE d2baseType);
};

}  // namespace D2

#endif /* SYSTEM_GRAPHIC_2D_2DBASE_H */
