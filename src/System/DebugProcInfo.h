#ifndef SYSTEM_DEBUGPROCINFO_H
#define SYSTEM_DEBUGPROCINFO_H

#include "khtypes.h"
#include "System/Graphic/StrPrim.h"

/* CFnt: a CStrPrim with the 0x8000 style bit always set (from the original's font header; its inline functions
 * are emitted where it is used). setStyle is defined after the class: the constructor calls it out of line. */
class CFnt : public CStrPrim {
public:
    CFnt() { setStyle(0); }
    // PARTIAL: added for Form/DebugMenu
    CFnt(INT nX, INT nY, const G_RGBA* pColor, INT nSize, const CHAR* pszText, UINT nStyle, INT nZ)
        : CStrPrim(nX, nY, pColor, nSize, pszText, nStyle | 0x8000, nZ) {}
#ifdef DEBUGPROCINFO_H_CFNT_DECL_ONLY
    virtual ~CFnt();
#else
    virtual ~CFnt() {}
#endif
    void setStyle(UINT nStyle);
};

/* MATCH: Game/Entity constructs a CFnt but has no weak copies of its destructor, setStyle or vtable; there the
 * members are only declared (DEBUGPROCINFO_H_CFNT_DECL_ONLY) and bind to the copies in the other objects. */
#ifndef DEBUGPROCINFO_H_CFNT_DECL_ONLY
inline void CFnt::setStyle(UINT nStyle) { m_Style = nStyle | 0x8000; }
#endif

class CDebugProcInfo {
public:
    static void initialize();
    static void finalize();
    static void dispProcInfo();
    static void changeProcCounterColor(BYTE r, BYTE g, BYTE b);
    static void dispPadInfo();
};

#endif /* SYSTEM_DEBUGPROCINFO_H */
