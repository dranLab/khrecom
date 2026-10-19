#ifndef SYSTEM_GRAPHIC_FRECFONT_H
#define SYSTEM_GRAPHIC_FRECFONT_H

#include "khtypes.h"

struct RsrcInfo;
struct G_XY;
class CBuf;
class CFntM2;

class CFrecFont {
public:
    static void initialize();
    static void finalize();
    static BOOL setupFontTex(const RsrcInfo& info, const CBuf& src);
    static void setupFonts();
    static USHORT fromSJIS(USHORT nType, USHORT nSJIS);
    static USHORT toSJIS(USHORT nType, USHORT nCode);
    static void makePkt(CFntM2& prim);
    static INT getDispSize(G_XY& outSize, const CFntM2& prim, G_XY* pLineSize, INT* pNumCharcter);
    static UINT getPacketSize(const CFntM2& prim);
    static USHORT getFontSize(USHORT nType);
    static USHORT getFontMaxCode(USHORT nType);
};

#endif /* SYSTEM_GRAPHIC_FRECFONT_H */
