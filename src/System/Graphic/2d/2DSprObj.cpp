#define GPRIM_H_PATH "System/Graphic/Gprim.h"
#define D2MAN_H_PATH "c:/KHCOM/MainApp/System/Graphic/2d/2DMan.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "System/Graphic/2d/2DSprObj.h"
#include "include_asm.h"

using namespace D2;

inline void operator delete[](void* pv) { com_free(pv); }

INT C2DSprObj::m_fgParent;

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", ApplyMatrix2__FR7FVECTORRC7FMATRIXRC8FVECTOR2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", AddVector2__FR8FVECTOR2RC8FVECTOR2T1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", AddVector2__FR7FVECTORRC8FVECTOR2T1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", InterVector2__FR8FVECTOR2RC8FVECTOR2T1f);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", InterVector2__FR7FVECTORRC8FVECTOR2T1f);
C2DSprObj::C2DSprObj(SeqControl* pSeqControl, const TexInfo* pTexInfo) {
    m_pSprPrim = NULL;
    m_vRot._q = 0;
    m_nPoly = 0;
    m_fgDisp = 0;
    m_Flg = 0;
    m_vPos._u64 = 0;
    m_vOfs._u64 = 0;
    m_vSwy._u64 = 0;
    m_vScl.x = 1.0f;
    m_vScl.y = 1.0f;
    m_vCol.set(1.0f, 1.0f, 1.0f, 1.0f);
    m_pSeqControl = pSeqControl;
    m_pTexInfo = pTexInfo;
    m_fgRotTransPers = -1;
    m_iStep = 0;
    m_pScissoring = NULL;
}
C2DSprObj::~C2DSprObj() {
    if (m_pSprPrim) delete[] m_pSprPrim;
    m_pSprPrim = NULL;
    if (m_pScissoring) delete m_pScissoring;
    m_pScissoring = NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", init__Q22D29C2DSprObjUi);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x322F6432\n.word 0x72705344\n.word 0x2E6A624F\n.word 0x28707063\n.word 0x29343531\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", initSprite__Q22D29C2DSprObjiPQ22D29SPR_GROUPPQ22D29SPR_PARTS);
__asm__(".rdata\n.align 3\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x322F6432\n.word 0x72705344\n.word 0x2E6A624F\n.word 0x00707063\n.word 0x754E6E69\n.word 0x3D3E206D\n.word 0x00003020\n.word 0x00000000\n.word 0x4B2F3A63\n.word 0x4D4F4348\n.word 0x69614D2F\n.word 0x7070416E\n.word 0x7379532F\n.word 0x2F6D6574\n.word 0x70617247\n.word 0x2F636968\n.word 0x322F6432\n.word 0x72705344\n.word 0x2E6A624F\n.word 0x28707063\n.word 0x29343931\n.word 0x00000000\n.text\n");
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", setFukidasiSize__Q22D29C2DSprObjff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", getFukidasiRect__Q22D29C2DSprObjPA3_8FVECTOR2);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", initForSeq__Q22D29C2DSprObjPQ22D213SEQ_ANIMATIONPQ22D210LAY_SCREEN);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", _main__Q22D29C2DSprObji);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", move__Q22D29C2DSprObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", getRect__Q22D29C2DSprObjP8FVECTOR2RQ22D27SprPrim);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", _disp__Q22D29C2DSprObj);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", changeTexture__Q22D29C2DSprObjiPC7TexInfoP4G_UVT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", changeLayer__Q22D29C2DSprObji);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", shiftBase__Q22D29C2DSprObjff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/2d/2DSprObj", restart__Q22D29C2DSprObj);

// SOURCE DATA: .rodata
