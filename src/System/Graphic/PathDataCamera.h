#ifndef SYSTEM_GRAPHIC_PATHDATACAMERA_H
#define SYSTEM_GRAPHIC_PATHDATACAMERA_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Camera.h"
#include "System/Graphic/HermitePathCamera.h"

class CPathData;
class CHermitePath;
class CDbgArrow;
struct PATH_DATA;

class CPathDataCamera : public CTask, public ICamera {
private:
    UINT m_iStatus;                 /* 0x50 */
    CPathData* m_pPathData;         /* 0x54 */
    INT m_iKind;                    /* 0x58 */
    HANDLE m_hHPC;                  /* 0x5c */
    CHermitePathCamera* m_pHPC;     /* 0x60 */
    CHermitePath* m_pHPM;           /* 0x64 */
    CTask* m_pTarget;               /* 0x68 */
    INT m_iNum;                     /* 0x6c */
    INT m_iCNum;                    /* 0x70 */
    CDbgArrow* m_DDPathCamArrow;    /* 0x74 */

public:
    CPathDataCamera(CTask* p, const CHAR* pszInName, INT iKind, const CTask* pTarget, INT iNum, const CTask* pParent);
    CPathDataCamera(CTask* p, const PATH_DATA* pPathData, INT iKind, const CTask* pTarget, INT iNum,
                    const CTask* pParent);
    CPathDataCamera(CTask* p, const CPathData* pCPathData, INT iKind, const CTask* pTarget, INT iNum,
                    const CTask* pParent);
    virtual ~CPathDataCamera();
    virtual void main();
    virtual void disp();
    virtual BOOL isEnd();
    virtual BOOL setEnable(BOOL bInFlag) { return m_pHPC->setEnable(bInFlag); }
    virtual void out() { CTask::out(); }
    virtual void ignorePause(BOOL bInFlag) {
        CTask::ignorePause(bInFlag);
        m_pHPC->ignorePause(bInFlag);
    }
    virtual void stop() {
        _stop();
        m_pHPC->stop();
    }
    virtual void run() {
        _run();
        m_pHPC->run();
    }
#ifndef PATHDATACAMERA_H_PATH /* MATCH: Game/Atel/CameraMan names it "System/Graphic/PathDataCamera.h" */
#define PATHDATACAMERA_H_PATH "c:/KHCOM/MainApp/System/Graphic/PathDataCamera.h"
#endif
#line 58 PATHDATACAMERA_H_PATH
    void initBGCheck(BOOL fgTransparency) {
        if (!m_pHPC) AssertApp(__FILE__, __LINE__, "m_pHPC", 0);
        m_pHPC->initBGCheck(fgTransparency);
    }
    BOOL isStop() { return m_iStatus & 2; }
    HANDLE getCameraHandle() { return m_hHPC; }
    virtual void ignorePlaySpeed(BOOL bInFlag) {
        CTask::ignorePlaySpeed(bInFlag);
        m_pHPC->ignorePlaySpeed(bInFlag);
    }
    void entryDebugArrow();

private:
    void _init(INT iKind, CTask* pTarget, INT iNum, CTask* pParent);
    void update(FLOAT fr, BOOL bUpdate);
    void _stop() { m_iStatus |= 2; }
    void _run() { m_iStatus &= ~2; }
};

#endif /* SYSTEM_GRAPHIC_PATHDATACAMERA_H */
