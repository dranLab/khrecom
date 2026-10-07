#include "Game/CollParam.h"
#include "System/MainApp.h"
#include "System/Debug.h"
#include <string.h>
#include "include_asm.h"

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CollParam", getAtkKind__C14ATK_COLL_PARAM);

INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/Game/CollParam", addrMapping__17COLL_PARAM_HEADER);

// Both functions below MATCH (funcdiff), and with "// SOURCE DATA: .rodata" the unit's .rodata is
// complete - but the yaml cuts this unit's .rodata at 0x37CB14 (the start of EffectMotion), in the
// middle of the TRACE string (0x37CB10..0x37CB4F). With the boundary at 0x37CB50 they can be enabled.
#if 0
#line 15
ATK_KIND ATK_COLL_PARAM::getAtkKind() const {
    static const ATK_KIND g_AtkKind[21] = {
        /* [0] = */ ATK_KIND_DMG_SMALL,
        /* [1] = */ ATK_KIND_DMG_BIG,
        /* [2] = */ ATK_KIND_DMG_BLOWN,
        /* [3] = */ ATK_KIND_GM_DAMAGE,
        /* [4] = */ ATK_KIND_GM_FAINT,
        /* [5] = */ ATK_KIND_DMG_TOSS,
        /* [6] = */ ATK_KIND_DMG_BEAT,
        /* [7] = */ ATK_KIND_WITHOUT,
        /* [8] = */ ATK_KIND_STUN,
        /* [9] = */ ATK_KIND_GM_BIG_DAMAGE,
        /* [10] = */ ATK_KIND_DMG_VOLLEY,
        /* [11] = */ ATK_KIND_CONFUSE,
        /* [12] = */ ATK_KIND_GM_JACK_BOX,
        /* [13] = */ ATK_KIND_GM_TENTACLE,
        /* [14] = */ ATK_KIND_STOP,
        /* [15] = */ ATK_KIND_GM_DECISION,
        /* [16] = */ ATK_KIND_THUNDER,
        /* [17] = */ ATK_KIND_THUNDRA,
        /* [18] = */ ATK_KIND_THUNDAGA,
        /* [19] = */ ATK_KIND_GM_CONFUSE,
        /* [20] = */ ATK_KIND_DARK_PUNISHER
    };

    if (!(0 <= iAtkKind && iAtkKind < (sizeof(g_AtkKind)/sizeof((g_AtkKind)[0])))) AssertApp(__FILE__, __LINE__, "0 <= iAtkKind && iAtkKind < (sizeof(g_AtkKind)/sizeof((g_AtkKind)[0]))", 0);
    return g_AtkKind[iAtkKind];
}

BOOL COLL_PARAM_HEADER::addrMapping() {
    if (uiVersion & 0x80000000) {
        for (INT i = 0; i < iAtkCollParamNum; i++) {
            pAtkCollParam[i].bEnable = 1;
        }
        return 1;
    }
    if (strncmp(szCode, "@COP", 4) != 0) {
        return 0;
    }
    if ((uiVersion & 0x7FFFFFFF) != 3) {
        TRACE("****************** ASD\x82\xcc\x83o\x81[\x83W\x83\x87\x83\x93\x82\xaa\x8c\xc3\x82\xa2\x82\xc5\x82\xb7 ******************\n");
        return 0;
    }
    uiVersion |= 0x80000000;

    if (iBodyCollParamNum > 0) {
        pBodyCollParam = (BODY_COLL_PARAM*)((INT)this + (INT)pBodyCollParam);
    } else {
        pBodyCollParam = NULL;
    }

    if (iAtkCollParamNum > 0) {
        pAtkCollParam = (ATK_COLL_PARAM*)((INT)this + (INT)pAtkCollParam);
        for (INT i = 0; i < iAtkCollParamNum; i++) {
            pAtkCollParam[i].bEnable = 1;
            pAtkCollParam[i].uiFlag = 0;
        }
    } else {
        pAtkCollParam = NULL;
    }

    pOtherParam = (OTHER_PARAM*)((INT)this + (INT)pOtherParam);
    return 1;
}
#endif
