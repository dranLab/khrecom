#ifndef SYSTEM_VRAMMAN_H
#define SYSTEM_VRAMMAN_H

#include "khtypes.h"

class CBuf;
struct TexInfo;

/* VRAM layout (in 256-byte blocks for textures, 64-byte blocks for CLUTs), set by CVramMan::mapping. */
namespace VRAM {
extern INT _FRAME_SIZE;
extern INT _FRAME_AREA_END;
extern INT _FRAME0_START;
extern INT _FRAME0_END;
extern INT _FRAME1_START;
extern INT _FRAME1_END;
extern INT _ZBUF_START;
extern INT _ZBUF_SIZE;
extern INT _ZBUF_END;
extern INT _FONT_AREA_START;
extern INT _FONT_AREA_END;
extern INT _TEX_AREA_START;
extern INT _TEX_TMP1_START;
extern INT _TEX_TMP1_SIZE;
extern INT _TEX_TMP1_END;
extern INT _TEX_TMP2_START;
extern INT _TEX_TMP2_SIZE;
extern INT _TEX_TMP2_END;
extern INT _TEX_PREFIX_START;
extern INT _TEX_PREFIX_SIZE;
extern INT _TEX_PREFIX_END;
extern INT _TEX_FIX_START;
extern INT _TEX_FIX_SIZE;
extern INT _TEX_FIX_END;
extern INT _TEX_AREA_END;
extern INT _TEX_AREA_SIZE;
extern INT _CLUT_AREA_START;
extern INT _CLUT_AREA_END;
extern INT _CLUT_AREA_SIZE;
extern INT _CLUT_TMP1_START;
extern INT _CLUT_TMP1_SIZE;
extern INT _CLUT_TMP1_END;
extern INT _CLUT_TMP2_START;
extern INT _CLUT_TMP2_SIZE;
extern INT _CLUT_TMP2_END;
extern INT _CLUT_FONT_START;
extern INT _CLUT_FONT_SIZE;
extern INT _CLUT_FONT_END;
extern INT _CLUT_PREFIX_START;
extern INT _CLUT_PREFIX_SIZE;
extern INT _CLUT_PREFIX_END;
extern INT _CLUT_FIX_START;
extern INT _CLUT_FIX_END;
extern INT _CLUT_FIX_SIZE;
extern INT _FRAME0_SIZE;
extern INT _FRAME1_SIZE;
}

class CVramMan {
public:
    static void initialize(INT nThreadID);
    static void finalize();
    static void mapping(BOOL bNTSC);
    static INT allocTex(UINT nTexSize, UINT nClutSize, INT& nOutTex, INT& nOutClut, INT nType);
    static void release(UINT nVpos, UINT nCpos, INT nType);
    static void releaseTex(UINT nPos, INT nType);
    static void releaseClut(UINT nPos, INT nType);
    static void releaseAll();
    static void compact(INT nType);
    static INT getUsedRatio();
    static void transTexProc();
    static BOOL isTransTexDone();
    static BOOL waitTransTexDone(INT nThreadID);
    static BOOL reqTransTex(const TexInfo& texInfo, CBuf& data);
    static void halt(BOOL bFlag);
    static INT getHeapSize();
    static void dbgReportAllocatedUnit();
};

#endif // SYSTEM_VRAMMAN_H
