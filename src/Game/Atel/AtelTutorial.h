#ifndef GAME_ATEL_ATELTUTORIAL_H
#define GAME_ATEL_ATELTUTORIAL_H

#include "System/Task.h"
#include "System/Graphic/2d/2DSeqCtrl.h"

class CAtelTutorial : public CTask {
public:
    CAtelTutorial(CTask* pParent);
    virtual ~CAtelTutorial();
    virtual void main();
};

class CAtelCardSeq : public CTask {
private:
    D2::C2DSeqCtrl* m_pSeqArrow; /* 0x40 */

public:
    CAtelCardSeq(CTask* pParent, INT in_type, INT in_posX, INT in_posY);
    virtual ~CAtelCardSeq();
    virtual void main();
};

#endif /* GAME_ATEL_ATELTUTORIAL_H */
