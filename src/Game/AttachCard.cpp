
#define CARD_H_PATH "c:/KHCOM/MainApp/Game/Card.h" /* MATCH: this unit names Card.h so (header default "Game/Card.h") */
#define LINKEDBUF_H_PATH "System/LinkedBuf.h" /* MATCH: this unit names LinkedBuf.h by the relative path (LinkedBuf.h switches __FILE__ only when it is defined) */
#include "System/Graphic/Gprim.h"
#include "System/Graphic/modeldata.h"
#include "System/Graphic/Model.h"
#include "Game/Card.h"
#include "System/HeapMan.h"
#include "Game/AttachCard.h"
#include "System/Graphic/LightUnit.h"
#include "System/LinkedBuf.h"
#include "System/Graphic/Camera.h"
#include "Game/Movement.h"
#include <string.h>



#define ASSERT(e) if (!(e)) AssertApp(__FILE__, __LINE__, #e, 0)

typedef int MODELTAG[4];

#include "System/RsrcDB.h"





#line 256

CAttachCard::CAttachCard(CTask* inParent) : CEntity(inParent, (ObjType::eObjType)1) {
    m_pCardModel = NULL;
}

CAttachCard::~CAttachCard() {
    if (m_pCardModel) {
        if (m_pCardModel->m_pData) DelayedDelete((MODELTAG*)m_pCardModel->m_pData, TRUE);
        else if (m_pCardModel->m_pSemiData) DelayedDelete((MODELTAG*)m_pCardModel->m_pSemiData, TRUE);
        DelayedDelete(m_pCardModel, FALSE);
    }
}

BOOL CAttachCard::setup(CARD_KIND inKind, CCoord* inParent, const FVECTOR* inPos, const FVECTOR* inRot, FLOAT inScale) {
    m_CardKind = inKind;
#line 54
    m_pMove = new (__FILE__ "(54)") CMovement(this);
    if (!m_pMove) return FALSE;
    m_pMove->setFlag(9, 0);
    FVECTOR pos;
    if (inPos) {
        pos = *inPos;
    } else {
        pos.init();
    }
    if (inRot) setRot(*inRot);
    FVECTOR scale;
    scale.set(inScale, inScale, inScale);
    addCoord(1);
    Crd(0).setParent(inParent);
    setOldPos(pos);
    setPos(pos);
    setScale(scale);
    setStartPos(pos);
    updateCoord();
    coordFix();
    return initCard_();
}

static inline void _setCard(CARD& c, USHORT kind, BYTE num) {
    c.m_nId = 0;
    c.m_nKind2 = kind;
    c.m_nNumber2 = num;
    c.m_nCategory20 = cardDataTbl[c.m_nKind].nCategory0;
    c.m_nCategory21 = cardDataTbl[c.m_nKind].nCategory1;
}

#line 110
BOOL CAttachCard::initCard_() {
    static const int g_iCardModelIndex[14] = { 0, 0, 1, 2, 3, 4, 4, 5, 0, 6, 7, 8, 9, 10 };
    static const G_ST g_St[4] = { { 0.f, 0.f }, { 0.25f, 0.f }, { 0.f, 0.25f }, { 0.25f, 0.25f } };
    INT groupID = 0xA028;
    INT count = CRsrcDB::getModelCount(groupID);
    if (count <= 0) return FALSE;
    CARD card;
    _setCard(card, m_CardKind, 0);
    INT mdlIndex;
    if (m_CardKind != 0xB9) mdlIndex = g_iCardModelIndex[card.m_nCategory21];
    else mdlIndex = 10;
#line 124
    ASSERT(0 <= mdlIndex && mdlIndex < count);
    FVECTOR texST;
    char texName[16];
    CCard::getCardTexInfo(card, texName, texST);
    G_ST st[4];
    st[0].set(texST.x, texST.y);

    st[1].s = texST.z;
    st[1].t = texST.y;
    st[2].s = texST.x;
    st[2].t = texST.w;
    st[3].s = texST.z;
    st[3].t = texST.w;
    MODELDATA* orgMdl = CRsrcDB::getRsrcMDL(groupID)[mdlIndex];
#line 162
    ASSERT(orgMdl);
#line 164
    MODELDATA* d = m_pCardModel = new (__FILE__ "(164)") MODELDATA;
    if (!d) return FALSE;
    ((u_long128*)d)[0] = ((u_long128*)orgMdl)[0];
    ((u_long128*)d)[1] = ((u_long128*)orgMdl)[1];
    m_pCardModel->m_pData = NULL;
    m_pCardModel->m_pSemiData = NULL;
    INT dataSize1 = 0;
    INT dataSize2 = 0;
    if (orgMdl->m_pData) {
        MODELTAG* end = (MODELTAG*)(orgMdl->m_pSemiData ? orgMdl->m_pSemiData : orgMdl->m_pBBox);
        dataSize1 = (INT)end - (INT)orgMdl->m_pData;
        dataSize1 >>= 4;
    }
    if (orgMdl->m_pSemiData) {
        dataSize2 = (INT)orgMdl->m_pBBox - (INT)orgMdl->m_pSemiData;
        dataSize2 >>= 4;
    }
#line 191
    MODELTAG* newMdl = new (__FILE__ "(191)") MODELTAG[dataSize1 + dataSize2];
    if (!newMdl) return FALSE;
    memcpy(newMdl, orgMdl->m_pData, (dataSize1 + dataSize2) * sizeof(MODELTAG));
    if (orgMdl->m_pData) m_pCardModel->m_pData = newMdl;
    if (orgMdl->m_pSemiData) m_pCardModel->m_pSemiData = newMdl + dataSize1;
    MODELTAG* tag = (MODELTAG*)m_pCardModel->m_pData;
    if (tag) {
        INT index = 0;
        while (!(tag[index][0] == 0x60000000 && !tag[index][1] && !tag[index][2] && !tag[index][3])) {
            index++;
            INT num = (USHORT)tag[index][1];
            INT mode = (USHORT)tag[index][3];
            index += 2;
            for (INT i = 0; i < num; i++) {
                index += 2;
                if (MODELDATA::isNormal(mode)) index++;
                if (MODELDATA::isColorI(mode) || MODELDATA::isColorF(mode)) index++;
                if (MODELDATA::isTexture(mode)) {
                    FLOAT* s = (FLOAT*)&tag[index][0];
                    FLOAT* t = (FLOAT*)&tag[index][1];
                    for (INT j = 0; j < 4; j++) {
                        if (g_St[j].s == *s && g_St[j].t == *t) {
                            G_ST& r = st[j];
                            *s = r.s;
                            *t = r.t;
                            break;
                        }
                    }
                    index++;
                }
            }
            do { index++; } while (0); /* a loop construct (macro?): its loop note aligns the test label below */
        }
    }
    m_iModelCount = 1;
#line 290
    m_pModels = new (__FILE__ "(290)") CModel[1];
    if (!m_pModels) return FALSE;
    if (m_pCardModel->enableLight()) {
#line 297
        m_pNLight = new (__FILE__ "(297)") CLightUnit;
        if (!m_pNLight) return FALSE;
    }
    if (m_pCardModel->enablePointLight()) {
#line 304
        m_pGLight = new (__FILE__ "(304)") CGimmickLightUnit;
        if (!m_pGLight) return FALSE;
    }
    m_pModels->link(this, m_pCardModel);
    if (m_pCardModel->enablePointLight()) {
        m_pModels->setLightUnit(m_pGLight);
    } else if (m_pCardModel->enableLight()) {
        m_pModels->setLightUnit(m_pNLight);
    }
    m_pModels->changeTexture("dummy", texName);
    if (m_pNLight) m_pNLight->setAnchor(&Crd(0), 2);
    if (m_pGLight) m_pGLight->setAnchor(&Crd(0), 2);
    m_iID = groupID;
    return TRUE;
}

// SOURCE DATA: .rodata
