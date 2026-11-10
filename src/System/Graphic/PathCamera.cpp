
#include "System/HeapMan.h"
#include "System/MainApp.h"
#include "System/Graphic/PathCamera.h"
#include "Game/Collision/CollUtil.h"
#include "Game/Collision/CollMapCamera.h"

extern "C" float tanf(float);
void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);



#include "System/RsrcDB.h"




#define PI 3.14159265358979323846

CPathCamera::CPathCamera(CTask* pInParent) : CTask(pInParent, 0x4000) {
    setPriority(0x8000);
    CTask::ignorePlaySpeed(TRUE);
    m_pCamData = NULL;
    m_pCamera = NULL;
    m_pCollMapCamera = NULL;
    m_fgRun = FALSE;
    m_fLocal = FALSE;
    m_fgBGCheck = FALSE;
    m_Pos.init();
    m_GazePos.init();
}

CPathCamera::~CPathCamera() {
    deleteCamera();
    if (m_pCollMapCamera) {
        delete m_pCollMapCamera;
    }
    m_pCollMapCamera = NULL;
}

#line 47
INT CPathCamera::init(const CHAR* inCamName, BOOL enableDataRoll, BOOL fgTarget, INT inCamPri) {
    char camName[24];
    if (strlen(inCamName) >= 16) {
        return FALSE;
    }
    strcpy(camName, inCamName);
    strcat(camName, ".cam");
    m_pCamData = (CAMDATA*)CRsrcDB::getRsrcCAM(camName);
    if (!m_pCamData) AssertApp(__FILE__, __LINE__, "m_pCamData", 0);
    if (!m_pCamData) {
        return FALSE;
    }
    m_fgTarget = fgTarget;
    m_fgDataRoll = enableDataRoll;
    m_fCurrentTimeq = 0.0f;
    m_TotalFrame = m_pCamData->frameNum;
    m_pTrans = m_pCamData->trans;
    m_pTseq = m_pCamData->tseq;
    m_pAngle = m_pCamData->angle;
    m_pAseq = m_pCamData->aseq;
    m_pRoll = m_pCamData->roll;
    m_pRseq = m_pCamData->rseq;
    deleteCamera();
    m_pCamera = new (__FILE__ "(71)") CCamera(this, inCamPri);
    m_pCamera->setPriority(0x8000);
    m_fRate = 1.0f;
    m_next = 0;
    m_prev = 0;
    m_total = 0.0f;
    m_play = 0.0f;
    m_skipFlag = 1;
    m_flag0 = 0;
    m_nextA = 0;
    m_prevA = 0;
    m_totalA = 0.0f;
    m_playA = 0.0f;
    m_skipFlagA = 1;
    m_nextR = 0;
    m_prevR = 0;
    m_totalR = 0.0f;
    m_playR = 0.0f;
    m_skipFlagR = 1;
    m_Roll = 0.0f;
    stop();
    move(0);
    return TRUE;
}

void CPathCamera::initBGCheck(BOOL fgTransparency) {
    m_fgBGCheck = !fgTransparency ? 1 : -1;
    if (m_pCollMapCamera) {
        delete m_pCollMapCamera;
    }
    m_pCollMapCamera = NULL;
    m_pCollMapCamera = new (__FILE__ "(127)") CCollMapCamera;
}

void CPathCamera::stopBGCheck() {
    m_fgBGCheck = FALSE;
    if (m_pCollMapCamera) {
        delete m_pCollMapCamera;
    }
    m_pCollMapCamera = NULL;
}

void CPathCamera::main() {
    move(m_fgRun ? getFrameRate() : 0);
}

void CPathCamera::move(TIMEQ timeq) {
    FLOAT f;
    FLOAT racio;
    INT next;
    INT prev;
    CAMFRAME* trans0;
    CAMFRAME* trans1;
    FVECTOR vec0;
    FVECTOR vec1;

    if (!m_pCamData) return;
    if (!m_pCamera) return;

    if (m_skipFlag) {
        m_skipFlag = 0;
        if (m_next == -1) return;
        f = 0.0f;
    } else {
        if (m_next == -1) return;
        f = timeq * m_fRate;
    }
    m_play += f;
    m_fCurrentTimeq += f;
    while (m_play >= m_total) {
        m_play -= m_total;
        nextFrame();
    }
    racio = m_play / m_total;
    next = m_next;
    prev = m_prev;
    if (next < 0) {
        racio = 0.0f;
        next = prev;
    } else if (prev < 0) {
        racio = 0.0f;
        prev = next;
    }
    trans0 = &m_pTrans[prev];
    trans1 = &m_pTrans[next];
    vec0.set(trans0->tx, trans0->ty, trans0->tz);
    vec1.set(trans1->tx, trans1->ty, trans1->tz);
    sceVu0InterVectorXYZ(vec0.v, vec1.v, vec0.v, racio);
    if (m_fLocal) {
        ApplyMatrix(vec0, getParent()->getWorld(0), vec0);
    }
    m_pCamera->setPos(vec0);
    m_Pos = vec0;
    if (!m_fgTarget) {
        vec0.set(trans0->ox, trans0->oy, trans0->oz);
        vec1.set(trans1->ox, trans1->oy, trans1->oz);
        sceVu0InterVectorXYZ(vec0.v, vec1.v, vec0.v, racio);
    } else {
        vec0 = m_exTargetPos;
    }

    if (m_skipFlagA) {
        m_skipFlagA = 0;
        if (m_nextA == -1) return;
        f = 0.0f;
    } else {
        if (m_nextA == -1) return;
        f = timeq * m_fRate;
    }
    m_playA += f;
    while (m_playA >= m_totalA) {
        m_playA -= m_totalA;
        nextFrameA();
    }
    racio = m_playA / m_totalA;
    next = m_nextA;
    prev = m_prevA;
    if (next < 0) {
        racio = 0.0f;
        next = prev;
    } else if (prev < 0) {
        racio = 0.0f;
        prev = next;
    }
    {
        FLOAT angle0 = m_pAngle[prev];
        FLOAT angle1 = m_pAngle[next];
        angle0 = angle1 * racio + angle0 * (1.0f - racio);
        FLOAT scrz = tanf((90.0f - angle0 * 0.5f) * (PI * 2) / 360.0);
        m_pCamera->setProjectionDist(scrz * 256.0f - 512.0f);
    }

    if (m_pAngle && m_fgDataRoll) {
        if (m_skipFlagR) {
            m_skipFlagR = 0;
            if (m_nextR == -1) return;
            f = 0.0f;
        } else {
            if (m_nextR == -1) return;
            f = timeq;
        }
        m_playR += f;
        while (m_playR >= m_totalR) {
            m_playR -= m_totalR;
            nextFrameR();
        }
        racio = m_playR / m_totalR;
        next = m_nextR;
        prev = m_prevR;
        if (next < 0) {
            racio = 0.0f;
            next = prev;
        } else if (prev < 0) {
            racio = 0.0f;
            prev = next;
        }
        FLOAT roll0 = m_pRoll[prev];
        FLOAT roll1 = m_pRoll[next];
        m_Roll = roll1 * racio + roll0 * (1.0f - racio);
    }

    if (m_fLocal) {
        FVECTOR look;
        ApplyMatrix(look, getParent()->getWorld(0), vec0);
        m_GazePos = look;
    } else {
        m_GazePos = vec0;
    }
    if (m_pCollMapCamera) {
        FVECTOR vResult;
        if (m_pCollMapCamera->checkBGforCamera(m_Pos, m_GazePos, vResult, m_fgBGCheck < 0)) {
            m_pCamera->setPos(vResult);
        }
    }
    m_pCamera->lookAt(m_GazePos, m_Roll);
}

void CPathCamera::nextFrame() {
    if (m_next == -1) return;
    m_pTseq++;
    m_next = m_pTseq->next;
    m_prev = m_pTseq->prev;
    if (m_next != -1) {
        m_total = m_pTseq->tq;
    } else {
        m_flag0 |= 1;
        m_total = 1.0f;
    }
}

void CPathCamera::nextFrameA() {
    if (m_nextA == -1) return;
    m_pAseq++;
    m_nextA = m_pAseq->next;
    m_prevA = m_pAseq->prev;
    if (m_nextA != -1) {
        m_totalA = m_pAseq->tq;
    } else {
        m_totalA = 1.0f;
    }
}

void CPathCamera::nextFrameR() {
    if (m_nextR == -1) return;
    m_pRseq++;
    m_nextR = m_pRseq->next;
    m_prevR = m_pRseq->prev;
    if (m_nextR != -1) {
        m_totalR = m_pRseq->tq;
    } else {
        m_totalR = 1.0f;
    }
}

INT CPathCamera::isEnd() {
    if (!m_pCamData || !m_pCamera) return FALSE;
    return m_flag0 == 1;
}

void CPathCamera::setOffsetX(FLOAT fInX) {
    if (m_pCamera) m_pCamera->setOffsetX(fInX);
}

void CPathCamera::setOffsetY(FLOAT fInY) {
    if (m_pCamera) m_pCamera->setOffsetY(fInY);
}

void CPathCamera::setOffsetXY(FLOAT fInX, FLOAT fInY) {
    if (m_pCamera) m_pCamera->setOffsetXY(fInX, fInY);
}

BOOL CPathCamera::setEnable(BOOL bInFlag) {
    if (!m_pCamera) return FALSE;
    m_pCamera->setEnable(bInFlag);
    if (bInFlag) {
        run();
    } else {
        stop();
    }
    return TRUE;
}

void CPathCamera::out() {
    CTask::out();
}
// SOURCE DATA: .rodata
