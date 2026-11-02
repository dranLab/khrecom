#ifndef GAME_CARDKIND_H
#define GAME_CARDKIND_H

#include "khtypes.h"

struct CARD_DATA { // 0x8
    union {
        BYTE nCategory;
        struct {
            BYTE nCategory0 : 4;
            BYTE nCategory1 : 4;
        };
    };
    BYTE nTexNo;
    BYTE nTexPos;
    BYTE nFlag;
    LPCTSTR pName;
};

struct CARD { // 0x4
    union {
        UINT m_nId;
        struct {
            BYTE m_nKind;
            BYTE m_fgPremium;
            CHAR m_nNumber;
            BYTE m_nCategory0 : 4;
            BYTE m_nCategory1 : 4;
        };
        struct {
            USHORT m_nKind2;
            CHAR m_nNumber2;
            BYTE m_nCategory20 : 4;
            BYTE m_nCategory21 : 4;
        };
    };

    void init(/* parameters unknown */);
    BYTE getTexNo(/* parameters unknown */);
    BYTE getTexPos(/* parameters unknown */);
    LPCTSTR getName();
};

#endif /* GAME_CARDKIND_H */
