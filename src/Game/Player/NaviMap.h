#ifndef GAME_PLAYER_NAVIMAP_H
#define GAME_PLAYER_NAVIMAP_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Vector.h"
#include "System/Graphic/Gprim.h"
#include "Game/CardKind.h"

class CModel;
class CLightUnit;
class C2DCard;
namespace D2 {
class C2DSeqCtrl;
}

namespace Gauge {

class CNaviMap : public CTask {
public:
    static BOOL m_bBattleEnd;

private:
    CModel* m_pModels;            /* 0x040 */
    INT m_nModelCount;            /* 0x044 */
    CLightUnit* m_pLight;         /* 0x048 */
    FLOAT m_fDeg;                 /* 0x04c */
    FLOAT m_fRotOfs;              /* 0x050 */
    FVECTOR m_vPos;               /* 0x060 */
    FMATRIX m_mWS;                /* 0x070 */
    G_XYZ m_PosScreen;            /* 0x0b0 */
    FVECTOR m_vPosWSC;            /* 0x0c0 */
    FVECTOR m_vCardRot;           /* 0x0d0 */
    INT m_nWorldID;               /* 0x0e0 */
    UINT m_uiState;               /* 0x0e4 */
    UINT m_uiRoomState;           /* 0x0e8 */
    UINT m_uiCardState;           /* 0x0ec */
    UINT m_uiDispFlag;            /* 0x0f0 */
    HANDLE m_hBack;               /* 0x0f4 */
    HANDLE m_hFront;              /* 0x0f8 */
    CARD m_cardRoom;              /* 0x0fc */
    C2DCard* m_pCard;             /* 0x100 */
    D2::C2DSeqCtrl* m_pRoomName;  /* 0x104 */

public:
    CNaviMap(CTask* pParent);
    virtual ~CNaviMap();
    virtual void main();
    virtual void disp();

private:
    void _initialize();
    void changeLocalWS();
    void calcDispFlag();
    void initRoomName(const CHAR* inFile, UINT inIdxSeq);
};

extern UINT g_uiFlag_WorldStart[27];

}  // namespace Gauge

#endif /* GAME_PLAYER_NAVIMAP_H */
