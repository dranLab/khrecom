#include "Game/CardKind.h"
#include "System/Ctd/CtdMan.h"
#include "Game/SceneInfo.h"

#line 231

LPCTSTR CARD::getName() {
    static LPCTSTR pBlankText = "";

    if (m_nKind < 0xDB) {
        return cardDataTbl[m_nKind].pName;
    } else if (m_nKind == 0xDB) {
        UINT ctdID = CSceneInfo::getCurrentRoomID() + 0xFE;
        return CCtdMan::getText(ctdID);
    } else {
        if (CSceneInfo::getRoomInfo(-1) == NULL) {
            return "";
        }
        UINT ctdID = CSceneInfo::getCurrentFloorNumber() * 2 + 0x122 + (CSceneInfo::getCurrentRoomID() != 0);
        return CCtdMan::getText(ctdID);
    }
}

// SOURCE DATA: .data .rodata
