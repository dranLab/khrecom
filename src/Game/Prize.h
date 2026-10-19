#ifndef GAME_PRIZE_H
#define GAME_PRIZE_H

#include "khtypes.h"
#include "Game/Entity.h"
#include "Game/EntityFactory.h" // PRIZE_PARAM
#include "System/Fsm.h"

struct MODELDATA;

class CPrize : public CEntity {
private:
    FVECTOR m_vGetVel;         /* 0x1d0 */
    INT m_iStep;               /* 0x1e0 */
    INT m_PrizeKind;           /* 0x1e4 CARD_KIND */
    INT m_iNumber;             /* 0x1e8 */
    FLOAT m_fSpeed;            /* 0x1ec */
    HANDLE m_hEffectHandle;    /* 0x1f0 */
    FLOAT m_fDrawRadius;       /* 0x1f4 */
    BOOL m_bHoming;            /* 0x1f8 */
    BOOL m_bSECall;            /* 0x1fc */
    BOOL m_bReturnCard;        /* 0x200 */
    BOOL m_bFlyHigh;           /* 0x204 */
    INT m_iBoundCounter;       /* 0x208 */
    FLOAT m_fPrizeRate;        /* 0x20c */
    MODELDATA* m_pCardModel;   /* 0x210 */
    CStateTemplate<CPrize> m_State[2]; /* 0x214 */

public:
    CPrize(CTask* inParent);
    virtual ~CPrize();
    virtual INT getTypeID() const;
    BOOL setup(const PRIZE_PARAM& inParam);
    virtual void onOut();
    INT getPrizeKind() const { return m_PrizeKind; }
    void move(const FVECTOR& inPos);
    void enableReturnCard(BOOL inSw) { m_bReturnCard = inSw; }
    void enableMarluxia3Mode(BOOL inSw);
    void forceGetPrize();
    void flyHigh(BOOL bAll);

private:
    void beginPrizeMain_();
    void prizeMain_();
    void endPrizeMain_();
    void beginCardMain_();
    void cardMain_();
    void endCardMain_();
    void beginCardMainSpecial_();
    void cardMainSpecial_();
    void endCardMainSpecial_();
    void flashing_();
    void playerPrizeGet_();
    BOOL initPrize_();
    BOOL initCard_();
    BOOL initReloadCard_();
    void attachCardEffect_();
    void playBoundSE_();
    void playGetSE_();
    void cardRotation_();
    BOOL isGimmickCard_();
    BOOL isFriendCard_();
    BOOL isDraw_();
    void findPosition_(const PRIZE_PARAM& inParam);
};

#endif /* GAME_PRIZE_H */
