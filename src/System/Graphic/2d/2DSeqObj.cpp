#define GPRIM_H_PATH "System/Graphic/GPrim.h" /* MATCH: this unit names Gprim.h so (header default "System/Graphic/Gprim.h") */
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h" /* MATCH: this unit names 2DMan.h so (header default "System/Graphic/2D/2DMan.h") */
#include "System/TaskMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DSeqObj.h"
#include "System/Graphic/2d/2DSeqCtrl.h"
#include "System/Graphic/2d/2DSprObj.h"
#include "System/Graphic/2d/2DFntObj.h"
#include "System/Graphic/2d/2DSpr.h"
#include "System/Graphic/2d/2DSeq.h"



#define SJ_FOREACH(T, pEntry, list) \
    for (T *pEntry = (list).getHead(), *__pn = pEntry ? pEntry->getNext() : NULL; pEntry; \
         pEntry = __pn, __pn = __pn ? __pn->getNext() : NULL)



#include "System/RsrcDB.h"
#include "System/HeapMan.h"




namespace D2 {







UINT C2DSeqObj::m_uiID = 0;

C2DSeqObj::C2DSeqObj() {
    m_pScissoringEnd = NULL;
    initialize();
    m_fgParent = -1;
    m_pSeqControl = NULL;
}

C2DSeqObj::C2DSeqObj(SeqControl* pSeqControl) {
    m_pScissoringEnd = NULL;
    initialize();
    m_pSeqControl = pSeqControl;
    m_fgParent = -1;
}

#line 40
void C2DSeqObj::initialize() {
    m_uiID++;
    m_p2DObjList = new (__FILE__ "(43)") C2DObjList;
    m_qTimeExt = -1;
    m_bEnable = 1;
    m_qTime = 0;
    m_qTimeMax = 0;
    m_bEnd = 0;
    m_fgMainSkip = 0;
    if (m_pScissoringEnd) delete m_pScissoringEnd;
    m_pScissoringEnd = NULL;
}

C2DSeqObj::~C2DSeqObj() {
    if (m_p2DObjList) delete m_p2DObjList;
    if (m_pScissoringEnd) delete m_pScissoringEnd;
    m_pScissoringEnd = NULL;
}

HANDLE C2DSeqObj::create(HANDLE handle, UINT nSpr, const TexInfo* pTexInfo, SPR_GROUP* pSprGroupe,
                         SPR_PARTS* pSprParts) {
    m_bSeq = 0;
    if (C2DMan::m_fg2DViewer) setParent();
    createSub(nSpr, pTexInfo, pSprGroupe, pSprParts);
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        pEntry->enableDisp(TRUE);
    }
    m_hBaseHandle = handle;
    C2DSpr::addList(*this);
    return _getHandle();
}

HANDLE C2DSeqObj::create(HANDLE handle, const TexInfo* pTexInfo, SEQ_SEQUENCE* pSeqSequence,
                         SEQ_ANIMATION* pSeqAnimation, SPR_GROUP* pSprGroupe, SPR_PARTS* pSprParts,
                         SPR_SPRITE* pSprSprite, LAY_SCREEN* pLayScreen) {
    m_bSeq = 1;
    if (C2DMan::m_fg2DViewer) setParent();
    INT nIndex = pSeqSequence->pos;
    INT nAnim = pSeqSequence->max;
    m_nLoop = pSeqSequence->loop;
    SEQ_ANIMATION* pSeqAnim = &pSeqAnimation[nIndex];
    C2DSprObj* p2DSprObjLast = NULL;
    m_nLoopFrame = pSeqSequence->frame;
    for (INT i = 0; i < nAnim; i++, pSeqAnim++) {
        INT iSpriteId = pSeqAnim->sprite_id;
        SPR_SPRITE* pSprS = &pSprSprite[iSpriteId];
        INT nIndex = pSprS->pos;
        C2DSprObj* p2DSprObj = createSub(pSprS->max, pTexInfo, &pSprGroupe[nIndex], pSprParts);
        TIMEQ qTimeMax = p2DSprObj->initForSeq(pSeqAnim, pLayScreen);
        if (m_qTimeMax < qTimeMax) {
            m_qTimeMax = qTimeMax;
            p2DSprObjLast = p2DSprObj;
        }
    }
    if (p2DSprObjLast) p2DSprObjLast->setLast(TRUE);
    m_hBaseHandle = handle;
    C2DSpr::addList(*this);
    return _getHandle();
}

#line 139
C2DSprObj* C2DSeqObj::createSub(UINT nSpr, const TexInfo* pTexInfo, SPR_GROUP* pSprGroupe, SPR_PARTS* pSprParts) {
    C2DSprObj* p2DSprObj;
    INT i;
    p2DSprObj = new (__FILE__ "(142)") C2DSprObj(m_pSeqControl, pTexInfo);
    p2DSprObj->init(nSpr);
    for (i = 0; i < nSpr; i++) {
        p2DSprObj->initSprite(i, &pSprGroupe[i], &pSprParts[pSprGroupe[i].parts_id]);
    }
    get2DObjList()->pushBack(*p2DSprObj);
    return p2DSprObj;
}

#line 157
HANDLE C2DSeqObj::createFnt(HANDLE handle, C2DFnt* p2DFnt) {
    C2DFntObj* p2DFntObj;
    m_bSeq = 1;
    m_nLoop = 0;
    m_nLoopFrame = 0;
    p2DFntObj = new (__FILE__ "(164)") C2DFntObj(m_pSeqControl);
    m_qTimeMax = p2DFntObj->init(p2DFnt);
    get2DObjList()->pushBack(*p2DFntObj);
    m_hBaseHandle = handle;
    C2DSpr::addList(*this);
    return _getHandle();
}

void C2DSeqObj::_main() {
    if (!m_bSeq || !m_bEnable) return;
    if (m_fgMainSkip) {
        m_fgMainSkip = 0;
        return;
    }
    INT& rParent = C2DSprObj::m_fgParent;
    rParent = m_fgParent;
    if (m_qTimeExt < 0) {
        if (m_qTime >= m_qTimeMax) {
            if (m_nLoop >= 0) {
                if (m_nLoop == 1) {
                    m_nLoop = -1;
                } else {
                    if (m_nLoop >= 2) m_nLoop--;
                    INT startTime = m_nLoopFrame * 50;
                    INT overTime = m_qTime - m_qTimeMax;
                    INT totalTime = m_qTimeMax - startTime;
                    INT t;
                    if (totalTime) {
                        t = overTime % totalTime;
                    } else {
                        t = 0;
                    }
                    m_qTime = startTime + t;
                    restart();
                }
            }
        } else if (m_qTime + C2DMan::m_qTimeD >= m_qTimeMax) {
            m_bEnd = 1;
        } else {
            m_bEnd = 0;
        }
        SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
            pEntry->_main(m_qTime);
        }
        m_qTime += C2DMan::m_qTimeD;
    } else {
        SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
            pEntry->_main(m_qTimeExt);
        }
    }
}

#line 240
void C2DSeqObj::_disp() {
    INT fgScissoringEnd;
    if (m_bEnable <= 0) return;
    if (m_pSeqControl && m_pSeqControl->pParentSeqControl &&
        ((FLOAT*)m_pSeqControl->pParentSeqControl)[0xbc / 4] == 0.0f)
        return;
    fgScissoringEnd = 0;
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        INT r = pEntry->_disp();
        if (r) fgScissoringEnd = r;
    }
    if (fgScissoringEnd) {
        if (!m_pScissoringEnd) {
#line 253
            m_pScissoringEnd = new (__FILE__ "(253)") CScissoring;
            m_pScissoringEnd->setSemiTrans(ALPHA_BLEND);
            m_pScissoringEnd->setDefault();
        }
        m_pScissoringEnd->setLayerFG(fgScissoringEnd);
        m_pScissoringEnd->disp();
    }
}

void C2DSeqObj::restart() {
    m_bEnd = 0;
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        pEntry->restart();
    }
}

void C2DSeqObj::restart(TIMEQ qStart) {
    m_qTimeExt = -1;
    m_qTime = qStart;
    m_bEnd = 0;
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        pEntry->restart();
    }
}

void C2DSeqObj::changeTimeExt(TIMEQ qTimeExt) {
    m_qTimeExt = qTimeExt;
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        pEntry->restart();
    }
}

void C2DSeqObj::changeTexture(INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd) {
    C2DObj* p2DObj = get2DObjList()->getHead();
    if (p2DObj) {
        TexInfo* pTexInfo = NULL;
        if (pTexName) {
            pTexInfo = CRsrcDB::getRsrcTex(pTexName, NAME_CASE_SENSITIVE);
            if (!pTexInfo) pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
        }
        p2DObj->changeTexture(inSprIdx, pTexInfo, pStart, pEnd);
    }
}

void C2DSeqObj::changeTexture(INT inSeqIdx, INT inSprIdx, CHAR* pTexName, G_UV* pStart, G_UV* pEnd) {
    C2DObj* p2DObj = get2DObjList()->getHead();
    INT cnt = 0;
    while (cnt++ < inSeqIdx) p2DObj = p2DObj->getNext();
    if (p2DObj) {
        TexInfo* pTexInfo = NULL;
        if (pTexName) {
            pTexInfo = CRsrcDB::getRsrcTex(pTexName, NAME_CASE_SENSITIVE);
            if (!pTexInfo) pTexInfo = CRsrcDB::getRsrcTex("error", NAME_CASE_SENSITIVE);
        }
        p2DObj->changeTexture(inSprIdx, pTexInfo, pStart, pEnd);
    }
}

void C2DSeqObj::changeLayer(INT nLayer) {
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        pEntry->changeLayer(nLayer);
    }
}

void C2DSeqObj::shiftBase(FLOAT fX, FLOAT fY) {
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        pEntry->shiftBase(fX, fY);
    }
}

void C2DSeqObj::shiftColor(FLOAT fR, FLOAT fG, FLOAT fB, FLOAT fA) {
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        pEntry->shiftColor(fR, fG, fB, fA);
    }
}

void C2DSeqObj::setFukidasiSize(FLOAT fSizeX, FLOAT fSizeY) {
    SJ_FOREACH(C2DObj, pEntry, *get2DObjList()) {
        pEntry->setFukidasiSize(fSizeX, fSizeY);
    }
}

}  // namespace D2

// SOURCE DATA: .data .rodata
