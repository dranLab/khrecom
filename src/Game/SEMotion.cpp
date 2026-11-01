#define ANIM_H_GETFLAG_DECL_ONLY /* MATCH: the original calls CAnim::getFlag out of line */
#line 1
#define TASK_H_NO_SLC_OVERFLOW_MSG
#define COLLUTIL_H_PATH "c:/KHCOM/MainApp/Game/Collision/CollUtil.h"
#define BATTLEENEMY_H_PATH "c:/KHCOM/MainApp/Game/Enemy/BattleEnemy.h"
#include "include_asm.h"
#include "khtypes.h"

#include "Game/Collision/CollInfo.h"











/* The unit's .text has an unnamed copy of CAttachMotion's inline destructor (and .rodata one of its vtable): our
 * compiled copies are only kept with these lines in config/SLUS_217.99/symbol_addrs.txt
 *     _$_13CAttachMotion_00218F98 = 0x00218F98; // type:func size:0x30
 *     _vt$13CAttachMotion_004856C8 = 0x004856C8; // size:0x30 */
#include "System/HeapMan.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/Model.h"
#include "System/RsrcData.h"
#include "System/Buf.h"
#include "System/Debug.h"
#include "Game/SEMotion.h"

#define RET_ADDR() ({ UINT nRA; __asm__ volatile("daddu %0, $31, $0" : "=r"(nRA)); nRA; })

inline void* operator new[](size_t size, char* msg) throw() {
    return com_malloc_d(size, msg, RET_ADDR());
}
inline void operator delete[](void* mem) { com_free(mem); }

inline BOOL SES_HEADER::addrMapping() {
    if (!(uiVersion & 0x80000000)) {
        if (strncmp(szCode, "@SES", 4) != 0) {
            return FALSE;
        }
        if ((uiVersion & 0x7FFFFFFF) != 1) {
            TRACE("****************** SES\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
            return FALSE;
        }
        uiVersion |= 0x80000000;
        pDataTbl = (SE_DATA_TBL*)((UINT)this + (UINT)pDataTbl);
        for (INT i = 0; i < iDataTblNum; i++) {
            pDataTbl[i].pSeData = (SE_DATA*)((UINT)this + (UINT)pDataTbl[i].pSeData);
        }
    }
    return TRUE;
}

#include "Game/Collision/CollUtilA.h"
#include "Game/Collision/CollUtil.h"
#include "System/Graphic/Anim.h"
#include "Game/Enemy/EnemyParam.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/CardBattle.h"
#include "Game/Enemy/BattleEnemy.h"
#include "Game/Entity.h"
#include "Game/Movement.h"
#include "Game/Collision/CollObj.h"
#include "Game/Collision/CollMap.h"
#include "Game/SceneInfo.h"
#include "System/Graphic/Camera.h"
#include "System/Sound/SoundMan.h"
#include "System/Rnd.h"

#include "System/Graphic/MotData.h"








class CRsrcData {
private:
    CBuf* m_pDataSrc;
    UBYTE _m_0x04[0x40];

public:
    virtual ~CRsrcData();
    BYTE* getRsrcData() { return m_pDataSrc ? m_pDataSrc->getBufPtr() : NULL; }
};

class CRsrcANN : public CRsrcData {
private:
    INT m_nCount;

public:
    INT getMotCount() const { return m_nCount; }
    CMotData* getMotData(INT nInMotNo) {
        if (nInMotNo < 0 || nInMotNo >= m_nCount) {
            return NULL;
        }
        CMotData** p = (CMotData**)getRsrcData();
        if (!p) {
            return NULL;
        }
        return p[nInMotNo];
    }
};

#include "System/RsrcDB.h"





/* Inline in the original's collision header: the weak copies of its destructors and collideMap end this
 * object. */
class CPointCollision : public CCollision { /* PARTIAL: stand-in for Game/Gimmick/GimmickCarry CPointCollision: gated by COLLOBJ_H_COLLISION_CLASSES (CollObj.h comes in early via Movement.h) */
public:
    CPointCollision() { m_Type = COLL_TYPE_POINT; }
    virtual ~CPointCollision() {}
    virtual INT collideMap() { return CCollUtil::getInstance()->_detectMCD(*this); }
};

void ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1);

inline INT _seSeq(INT nFile, INT nID) { return nFile * 1000 + nID; }
inline INT _voiceSeq(INT nFile, INT nID) { return _seSeq(nFile, nID) | 0x10000000; }

#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

namespace SEMotion {
static const INT g_iSoraFileIDTbl[14] = {661, 641, 656, 601, 621, 616, 626, 611, 606, 631, 651, 646, 636, 661};
static const INT g_iRikuFileIDTbl[14] = {665, 645, 660, 605, 625, 620, 630, 615, 610, 635, 651, 650, 640, 665};
static const INT g_iDonaldFileIDTbl[14] = {662, 642, 657, 602, 622, 617, 627, 612, 607, 632, 652, 647, 637, 662};
static const INT g_iGoofyFileIDTbl[14] = {663, 643, 658, 603, 623, 618, 628, 613, 608, 633, 653, 648, 638, 663};
static const INT g_iPooFileIDTbl[14] = {654, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654};
const INT* g_pFileIDTbl[5] = {
    g_iSoraFileIDTbl, g_iRikuFileIDTbl, g_iDonaldFileIDTbl, g_iGoofyFileIDTbl, g_iPooFileIDTbl,
};
}

CSEMotionCtrl::CSEMotionCtrl(CSEMotion* inParent) {
    m_iStep = 3;
    m_pParent = inParent;
    m_iCounter = 0;
    m_bVolumeOff = FALSE;
    m_pSeData = NULL;
    m_hHandle = 0;
}

CSEMotionCtrl::~CSEMotionCtrl() {
    stop();
}

#line 188
BOOL CSEMotionCtrl::initalize(CEntity* inOwner) {
    ASSERT(inOwner);
    m_pOwner = inOwner;
    return TRUE;
}

#line 197
void CSEMotionCtrl::set(const SE_DATA* inData) {
    ASSERT(inData);
    m_iStep = 0;
    m_pSeData = inData;
    m_iFileID = inData->iFileID;
    m_iSeID = inData->iSeID;
    m_hHandle = 0;
    m_iCounter = inData->iCallCount;
    m_iFootChange = 0;
    m_bVolumeOff = FALSE;
    if (m_iCounter <= 0) {
        m_iCounter = -1;
    }
}

#line 212
void CSEMotionCtrl::set(const SE_DATA* inData, INT inSeID) {
    ASSERT(inData);
    m_iStep = 0;
    m_pSeData = inData;
    m_iFileID = inData->iFileID;
    m_iSeID = inSeID;
    m_hHandle = 0;
    m_iCounter = inData->iCallCount;
    m_iFootChange = 0;
    m_bVolumeOff = FALSE;
    if (m_iCounter <= 0) {
        m_iCounter = -1;
    }
}

void CSEMotionCtrl::update(TIMEQ inCurrentTime) {
    if (!m_pSeData) {
        return;
    }
    switch (m_iStep) {
    case 1:
        if (inCurrentTime >= m_pSeData->getEnd()) {
            m_iStep = 2;
            fadeoutSE_(m_pSeData->iFadeTime);
        }
        break;
    case 0:
        if (inCurrentTime >= m_pSeData->getStart() || m_pOwner->isMotionEnd()) {
            m_bVolumeOff = FALSE;
            if (m_pSeData->isFoot()) {
                if (setupFootSE_()) {
                    INT footID = m_pSeData->iSeID % 16;
                    if (footID >= 1 && footID <= 8) {
                        m_iSeID += m_iFootChange * 2;
                        m_iFootChange ^= 1;
                    }
                } else {
                    m_bVolumeOff = TRUE;
                }
            }
            playPanAndVol_();
            if (m_pSeData->getEnd() >= 0) {
                m_iStep = 1;
            } else {
                m_iStep = 2;
            }
        }
    case 2:
        if (m_pOwner->getAnim()->isLoop(-1)) {
            if (m_pOwner->getAnim()->getFlag(0) & 0x80) {
                if (m_iCounter > 0 && --m_iCounter <= 0) {
                    m_iStep = 3;
                } else {
                    m_iStep = 0;
                }
            }
        } else if (m_pOwner->isMotionEnd()) {
            fadeoutSE_(m_pSeData->iFadeTime);
            m_iStep = 3;
        }
        break;
    }
    setPanAndVol_();
}

void CSEMotionCtrl::stop() {
    if (m_pSeData) {
        fadeoutSE_(m_pSeData->iFadeTime);
    }
    m_hHandle = 0;
    m_pSeData = NULL;
}

#line 341
BOOL CSEMotionCtrl::setupFootSE_() {
    ASSERT(m_pSeData);
    INT id = m_pOwner->getID();
    INT index = CSceneInfo::getCurrentFloorNumber() <= 0;
    switch ((UINT)id / 10000) {
    case 1:
        switch ((UINT)id % 10000) {
        case 401:
        case 409:
            index = 2;
            break;
        case 402:
            index = 3;
            break;
        }
        break;
    case 10:
        if ((UINT)id % 10000 == 1005) {
            index = 4;
        }
        break;
    }
    m_iFileID = SEMotion::g_pFileIDTbl[index][CSceneInfo::getCurrentWorldID()];
    INT footID = m_pSeData->iSeID % 16;
    m_iSeID = footID;
    CMovement* move = m_pOwner->m_pMove;
    if (!move) {
        return FALSE;
    }
    switch (footID) {
    case 13:
    case 14:
    case 15: {
        CPointCollision coll;
        coll.setStartPos(m_pOwner->Crd(0).m_World.getTrans());
        coll.m_inWE.set(0.0f, 0.0f, 1.0f);
        ApplyMatrix(coll.m_inWE, m_pOwner->Crd(0).m_World, coll.m_inWE);
        if (coll.collideMap()) {
            COLL_INFO_UNION info;
            info.info = coll.m_outInfo;
            m_iSeID += info.infoID * 16;
        }
        break;
    }
    case 10:
        m_iSeID = ((COLL_INFO_UNION&)move->getJumpCollInfo()).infoID * 16 + 10;
        break;
    default: {
        COLL_INFO_UNION info;
        CCollMap* collMap = move->getMapCollision();
#line 425
        ASSERT(collMap);
        FVECTOR start = move->getPos();
        FVECTOR end = start;
        start.y += 0.05f;
        end.y -= 0.4f;
        collMap->setStartPos(start);
        collMap->setEndPos(end);
        if (!collMap->collideRideForCheck(FALSE)) {
            collMap->setStartPos(start);
            collMap->setEndPos(end);
            if (!collMap->collideMap()) {
                return FALSE;
            }
            info = collMap->getDetectInfo();
        } else {
            collMap->setStartPos(start);
            collMap->setEndPos(end);
            if (!collMap->collideMap()) {
                info = collMap->getDetectInfoO();
            } else {
                info = collMap->getDetectInfo();
            }
        }
        m_iSeID += info.infoID * 16;
        break;
    }
    }
    return TRUE;
}

void CSEMotionCtrl::playPanAndVol_() {
    if (!m_bVolumeOff) {
        FVECTOR offset;
        offset.set(m_pSeData->fOffsetX, m_pSeData->fOffsetY, m_pSeData->fOffsetZ);
        m_hHandle = m_pOwner->playSEPanAndVol(m_iFileID, m_iSeID, m_pSeData->iBoneIndex, &offset,
                                              m_pSeData->isVoice() != 0);
    }
}

#line 521
void CSEMotionCtrl::setPanAndVol_() {
    if (m_hHandle && !m_bVolumeOff) {
        ASSERT(m_pOwner);
        ASSERT(m_pSeData);
        FVECTOR offset;
        offset.set(m_pSeData->fOffsetX, m_pSeData->fOffsetY, m_pSeData->fOffsetZ);
        m_pOwner->setSEPanAndVol(m_hHandle, m_iFileID, m_iSeID, m_pSeData->iBoneIndex, &offset,
                                 m_pSeData->isVoice() != 0);
    }
}

void CSEMotionCtrl::fadeoutSE_(INT inTime) {
    if (m_pSeData->isVoice() && inTime <= 0) {
        return;
    }
    INT t = inTime > 0 ? inTime : 5;
    INT id = m_pSeData->isVoice() ? _voiceSeq(m_iFileID, m_iSeID) : _seSeq(m_iFileID, m_iSeID);
    CSound::fadeoutSE(m_hHandle, id, t);
}

CSEMotion::CSEMotion(CEntity* inOwner) : CAttachMotion(inOwner) {
    m_iID = 0;
    m_pHeader = NULL;
    m_iSEMotCtrlCount = 0;
    m_iNowCtrlCount = 0;
    m_pSEMotCtrl = NULL;
    m_iRandomDataNum = 0;
    m_pRandomData = NULL;
    m_hVoice = 0;
    m_iPriority = 0;
    m_hBeforeVoice = 0;
    m_iBeforeFileID = 0;
    m_iBeforeVoiceID = 0;
    m_iBeforePriority = 0;
    m_pBeforeSeData = NULL;
    m_tqVoiceTimer = 0;
}

CSEMotion::~CSEMotion() {
    delete[] m_pSEMotCtrl;
    m_pSEMotCtrl = NULL;
    delete[] m_pRandomData;
    m_pRandomData = NULL;
}

BOOL CSEMotion::initalize(INT inID) {
    SES_HEADER* header = (SES_HEADER*)CRsrcDB::getFirstData(0x30, inID, 0);
    if (!header) {
        delete[] m_pSEMotCtrl;
        m_pSEMotCtrl = NULL;
        delete[] m_pRandomData;
        m_pRandomData = NULL;
        m_pHeader = NULL;
        return FALSE;
    }
    m_iID = inID;
    return initialize_(header);
}

void CSEMotion::stop() {
    INT n = m_iNowCtrlCount;
    CSEMotionCtrl* ctrl = m_pSEMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        const SE_DATA* data = ctrl->getSeData();
        if (data && data->isVoice() && m_hBeforeVoice && m_iBeforePriority < data->sPriority) {
            m_hBeforeVoice = ctrl->getHandle();
            m_iBeforeFileID = ctrl->getFileID();
            m_iBeforeVoiceID = ctrl->getSeID();
            m_iBeforePriority = data->sPriority;
            m_tqVoiceTimer = 30000;
        } else {
            ctrl->stop();
        }
    }
    m_iNowCtrlCount = 0;
}

/* Matches except that `this` and `ctrl` swap registers (s1/s2). */
#if 0
void CSEMotion::update() {
    TIMEQ current = m_pOwner->getMotionCurrentTime();
    INT n = m_iNowCtrlCount;
    CSEMotionCtrl* ctrl = m_pSEMotCtrl;
    for (INT i = 0; i < n; i++, ctrl++) {
        ctrl->update(current);
    }
    if (m_hBeforeVoice) {
        m_tqVoiceTimer -= m_pOwner->getFrameRate();
        if (m_tqVoiceTimer > 0 && CSound::isSEPlaying(m_hBeforeVoice, _voiceSeq(m_iBeforeFileID, m_iBeforeVoiceID))) {
            m_pOwner->setSEPanAndVol(m_hBeforeVoice, m_iBeforeFileID, m_iBeforeVoiceID, 0, NULL, 1);
        } else {
            m_hBeforeVoice = 0;
        }
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/SEMotion", update__9CSEMotion);
#endif

#line 765
void CSEMotion::playFootStopSE() {
    INT id = m_pOwner->getID();
    INT index = CSceneInfo::getCurrentFloorNumber() <= 0;
    switch ((UINT)id / 10000) {
    case 1:
        switch ((UINT)id % 10000) {
        case 401:
        case 409:
            index = 2;
            break;
        case 402:
            index = 3;
            break;
        }
        break;
    case 10:
        if ((UINT)id % 10000 == 1005) {
            index = 4;
        }
        break;
    }
    INT fileID = SEMotion::g_pFileIDTbl[index][CSceneInfo::getCurrentWorldID()];
#line 772
    ASSERT(m_pOwner->m_pMove);
    COLL_INFO_UNION info;
    CCollMap* collMap = m_pOwner->m_pMove->getMapCollision();
#line 776
    ASSERT(collMap);
    FVECTOR start = m_pOwner->Crd(0).m_World.getTrans();
    FVECTOR end = start;
    start.y += 0.05f;
    end.y -= 0.4f;
    collMap->setStartPos(start);
    collMap->setEndPos(end);
    if (!collMap->collideRideForCheck(FALSE)) {
        collMap->setStartPos(start);
        collMap->setEndPos(end);
        if (!collMap->collideMap()) {
            return;
        }
        info = collMap->getDetectInfo();
    } else {
        FVECTOR ride = *collMap->getOutPos();
        collMap->setStartPos(start);
        collMap->setEndPos(end);
        if (!collMap->collideMap()) {
            info = collMap->getDetectInfoO();
        } else {
            info = collMap->getOutPos()->y < ride.y ? collMap->getDetectInfoO() : collMap->getDetectInfo();
        }
    }
    m_pOwner->playSEPanAndVol(fileID, (info.infoID << 4) | 9, 0, NULL, 0);
}

void CSEMotion::playVoice(INT inFileID, INT inID, INT inPriority) {
    if (!m_hBeforeVoice || m_iBeforePriority < inPriority) {
        m_hBeforeVoice = m_pOwner->playSEPanAndVol(inFileID, inID, 0, NULL, 1);
        m_iBeforeFileID = inFileID;
        m_iBeforeVoiceID = inID;
        m_iBeforePriority = inPriority;
        m_tqVoiceTimer = 30000;
    }
}

BOOL CSEMotion::initialize_(SES_HEADER* inHeader) {
    if (!inHeader->addrMapping()) {
        return FALSE;
    }
    CRsrcANN* ann = CRsrcDB::getRsrcANN(m_iID);
    if (!ann) {
        return FALSE;
    }
    delete[] m_pSEMotCtrl;
    m_pSEMotCtrl = NULL;
    delete[] m_pRandomData;
    m_pRandomData = NULL;
    m_pHeader = inHeader;
    for (INT i = 0; i < m_pHeader->iDataTblNum; i++) {
        INT index = -1;
        for (INT j = 0; j < ann->getMotCount(); j++) {
            CMotData* motData = ann->getMotData(j);
            if (motData && strcmp(motData->GetName(), m_pHeader->pDataTbl[i].szMotionName) == 0) {
                index = j;
                break;
            }
        }
        m_pHeader->pDataTbl[i].iMotionIndex = index;
    }
    m_iSEMotCtrlCount = 16;
    m_iNowCtrlCount = 0;
#line 887
    m_pSEMotCtrl = new (__FILE__ "(887)") CSEMotionCtrl[16](this);
    for (INT i = 0; i < m_iSEMotCtrlCount; i++) {
        m_pSEMotCtrl[i].initalize(m_pOwner);
    }
    INT count = 0;
    INT n = m_pHeader->iDataTblNum;
    SE_DATA_TBL* tbl = m_pHeader->pDataTbl;
    for (INT i = 0; i < n; i++, tbl++) {
        SE_DATA* seData = tbl->pSeData;
        for (INT j = 0; j < tbl->iSeDataNum; j++, seData++) {
            if (seData->isRandom()) {
                count++;
            }
        }
    }
    if (count > 0) {
        m_iRandomDataNum = count;
#line 909
        RANDOM_DATA* rd = m_pRandomData = new (__FILE__ "(909)") RANDOM_DATA[count];
        if (!rd) {
            return FALSE;
        }
        tbl = m_pHeader->pDataTbl;
        for (INT i = 0; i < n; i++, tbl++) {
            SE_DATA* seData = tbl->pSeData;
            for (INT j = 0; j < tbl->iSeDataNum; j++, seData++) {
                if (seData->isRandom()) {
                    rd->pSrc = seData;
                    rd->iBefore = -1;
                    rd++;
                }
            }
        }
    }
    return TRUE;
}

BOOL CSEMotion::set_(INT inIndex, INT inNo, BOOL inStop) {
    if (!m_pHeader) {
        return FALSE;
    }
    if (inStop) {
        stop();
    }
    BOOL voice;
    if (isDamagetMotion_(inIndex)) {
        voice = FALSE;
    } else {
        voice = isPlayVoice_();
    }
    INT count = m_iNowCtrlCount;
    INT n = m_pHeader->iDataTblNum;
    SE_DATA_TBL* tbl = m_pHeader->pDataTbl;
    const SE_DATA* voiceData = NULL;
    for (INT i = 0; i < n; i++, tbl++) {
        if (inIndex != tbl->iMotionIndex) {
            continue;
        }
        const SE_DATA* seData = tbl->pSeData;
        for (INT j = 0; j < tbl->iSeDataNum; j++, seData++) {
            if (seData->iNo != inNo) {
                continue;
            }
            if (seData->isVoice()) {
                if (voice && !(m_iBeforePriority < seData->sPriority)) {
                    continue;
                }
                if (voiceData) {
                    if (voiceData->tqStart == seData->tqStart) {
                        continue;
                    }
                } else {
                    voiceData = seData;
                }
            }
            if (seData->isRandom()) {
                INT seID = randomSelect_(seData);
                m_pSEMotCtrl[count].set(seData, seID);
            } else {
                m_pSEMotCtrl[count].set(seData);
            }
            if (++count >= m_iSEMotCtrlCount) {
                m_iNowCtrlCount = count;
                return TRUE;
            }
        }
    }
    m_iNowCtrlCount = count;
    return count > 0;
}

INT CSEMotion::randomSelect_(const SE_DATA* inData) {
    INT seID = inData->iSeID;
    INT n = m_iRandomDataNum;
    RANDOM_DATA* data = m_pRandomData;
    for (INT i = 0; i < n; i++, data++) {
        if (data->pSrc != inData) {
            continue;
        }
        short rndData[4];
        if (data->iBefore != -1) {
            SHORT cnt = 0;
            SHORT value = 0;
            for (INT j = 0; j < 4; j++) {
                if (inData->sRandom[j][0] == data->iBefore) {
                    value = inData->sRandom[j][1];
                    rndData[j] = 0;
                } else {
                    rndData[j] = inData->sRandom[j][1];
                    if (inData->sRandom[j][0] != -1 && rndData[j] > 0) {
                        cnt++;
                    }
                }
            }
            SHORT revise = 0;
            if (cnt) {
                revise = value / cnt + 1;
            }
            for (INT j = 0; j < 4; j++) {
                if (rndData[j] > 0) {
                    rndData[j] += revise;
                }
            }
        } else {
            for (INT j = 0; j < 4; j++) {
                rndData[j] = inData->sRandom[j][1];
            }
        }
        INT probability = RND::genrand_int31() % 100;
        INT total = 0;
        for (INT j = 0; j < 4; j++) {
            if (inData->sRandom[j][0] == -1) {
                continue;
            }
            if (rndData[j] > 0) {
                if (probability >= total && probability <= total + rndData[j]) {
                    seID = inData->sRandom[j][0];
                    break;
                }
                total += rndData[j];
            }
        }
        data->iBefore = seID;
        break;
    }
    return seID;
}

BOOL CSEMotion::isDamagetMotion_(INT inIndex) const {
    if (m_pOwner->getTypeID() != 0x1004) {
        return FALSE;
    }
    ENEMY_PARAM* param = ((CBattleEnemy*)m_pOwner)->getEnemyParam();
    if (!param) {
        return FALSE;
    }
    return param->isDamageMotion(inIndex);
}

BOOL CSEMotion::isPlayVoice_() {
    if (m_hBeforeVoice) {
        if (CSound::isSEPlaying(m_hBeforeVoice, _voiceSeq(m_iBeforeFileID, m_iBeforeVoiceID))) {
            return TRUE;
        }
        m_hBeforeVoice = 0;
    }
    return FALSE;
}

// SOURCE DATA: .data .rodata
