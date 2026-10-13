#ifndef GAME_ATWATER_H
#define GAME_ATWATER_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/Graphic/Gprim.h"

class CAtWaterPrim : public CGPrim {
private:
    FLOAT m_fRate;

public:
    CAtWaterPrim();
    virtual ~CAtWaterPrim() {}
    virtual UINT getPacketSize() const { return 0x1FD; }
    void update(FLOAT fr);

private:
    virtual FIX4 calcZ() { return 0xFFFFC02; }
    virtual void makePkt();
};

class CAtWater : public CTask {
private:
    CAtWaterPrim* m_pAtWater;
    static CAtWater* m_pInstance;

public:
    CAtWater(CTask* p);
    virtual ~CAtWater();
    virtual void main();
    virtual void disp();
    static void create(CTask* p);
    static void remove() {
        if (m_pInstance) {
            m_pInstance->out();
        }
    }
};

#endif /* GAME_ATWATER_H */
