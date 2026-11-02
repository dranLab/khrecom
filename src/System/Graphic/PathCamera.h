#ifndef SYSTEM_GRAPHIC_PATHCAMERA_H
#define SYSTEM_GRAPHIC_PATHCAMERA_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"
#include "System/Graphic/Camera.h"

class CCollMapCamera;

extern INT VSYNC_TIMEQ;

struct CAMFRAME {
    FLOAT tx;
    FLOAT ty;
    FLOAT tz;
    FLOAT ox;
    FLOAT oy;
    FLOAT oz;
};

struct CAMSEQ {
    SHORT prev;
    SHORT next;
    TIMEQ tq;
};

struct CAMDATA {
    INT frameNum;
    CAMFRAME* trans;
    CAMSEQ* tseq;
    float* angle;
    CAMSEQ* aseq;
    float* roll;
    CAMSEQ* rseq;
    INT dummy;
};

class CPathCamera : public CTask, public ICamera {
private:
    FVECTOR m_Pos;
    FVECTOR m_GazePos;
    FVECTOR m_exTargetPos;
    CAMDATA* m_pCamData;
    CCamera* m_pCamera;
    FLOAT m_fRate;
    FLOAT m_Roll;
    INT m_fgRun;
    INT m_fgDataRoll;
    FLOAT m_speed;
    CAMFRAME* m_pTrans;
    CAMSEQ* m_pTseq;
    FLOAT* m_pAngle;
    CAMSEQ* m_pAseq;
    FLOAT* m_pRoll;
    CAMSEQ* m_pRseq;
    INT m_next;
    INT m_prev;
    FLOAT m_total;
    FLOAT m_play;
    INT m_flag0;
    INT m_skipFlag;
    INT m_nextA;
    INT m_prevA;
    FLOAT m_totalA;
    FLOAT m_playA;
    INT m_skipFlagA;
    INT m_nextR;
    INT m_prevR;
    FLOAT m_totalR;
    FLOAT m_playR;
    INT m_skipFlagR;
    FLOAT m_fCurrentTimeq;
    INT m_TotalFrame;
    INT m_fgTarget;
    INT m_fLocal;
    BOOL m_fgBGCheck;
    CCollMapCamera* m_pCollMapCamera;

public:
    CPathCamera(CTask* pInParent);
    virtual ~CPathCamera();
    virtual BOOL setEnable(BOOL bInFlag);
    virtual void out();
    virtual INT isEnd();
    virtual void run() { m_fgRun = TRUE; }
    virtual void stop() { m_fgRun = FALSE; }
    virtual void ignorePause(BOOL bInFlag) {
        CTask::ignorePause(bInFlag);
        m_pCamera->ignorePause(bInFlag);
    }
    virtual void ignorePlaySpeed(BOOL bInFlag) {
        CTask::ignorePlaySpeed(bInFlag);
        m_pCamera->ignorePlaySpeed(bInFlag);
    }
    virtual void main();
    void move(TIMEQ timeq);
    INT init(const CHAR* inCamName, BOOL enableDataRoll, BOOL fgTarget, INT inCamPri);
    void deleteCamera() {
        if (m_pCamera) {
            delete m_pCamera;
        }
        m_pCamera = NULL;
    }
    FVECTOR& getPosition() { return m_Pos; }
    FVECTOR& getGazePosition() { return m_GazePos; }
    FMATRIX& getLocalMatrix() { return Crd(0).getLocalMatrix(); }
    void setRate(FLOAT rate) { m_fRate = rate; }
    // MATCH: written as an if for Game/Gimmick/GimmickTreasureBox (the result is a constant 1 on the true path)
    BOOL isCamEnd() {
        if (m_next == -1) return TRUE;
        return FALSE;
    }
    void setRoll(FLOAT roll) { m_Roll = roll; }
    void setOffsetX(FLOAT fInX);
    void setOffsetY(FLOAT fInY);
    void setOffsetXY(FLOAT fInX, FLOAT fInY);
    void setLocal(BOOL in_flag) { m_fLocal = in_flag; }
    INT getTotalFrame(BOOL fg) {
        if (!fg) {
            return (INT)(m_TotalFrame * (50.0f / VSYNC_TIMEQ));
        }
        return m_TotalFrame;
    }
    FLOAT getCurrentFrame(BOOL fg) {
        if (fg) {
            return m_fCurrentTimeq / 50.0f;
        }
        return m_fCurrentTimeq / VSYNC_TIMEQ;
    }
    void initBGCheck(BOOL fgTransparency);
    void stopBGCheck();

protected:
    CCamera* getCamera() { return m_pCamera; }

private:
    void nextFrame();
    void nextFrameA();
    void nextFrameR();
};

#endif /* SYSTEM_GRAPHIC_PATHCAMERA_H */
