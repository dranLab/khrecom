#ifndef SYSTEM_DEBUGPROCINFO_H
#define SYSTEM_DEBUGPROCINFO_H

#include "khtypes.h"
#include "System/Graphic/StrPrim.h"

/* CFnt: a CStrPrim with the 0x8000 style bit always set (from the original's font header; its inline functions
 * are emitted where it is used). setStyle is defined after the class: the constructor calls it out of line. */
class CFnt : public CStrPrim {
public:
    CFnt() { setStyle(0); }
    virtual ~CFnt() {}
    void setStyle(UINT nStyle);
};

inline void CFnt::setStyle(UINT nStyle) { m_Style = nStyle | 0x8000; }

class CDebugProcInfo {
public:
    static void initialize();
    static void finalize();
    static void dispProcInfo();
    static void changeProcCounterColor(BYTE r, BYTE g, BYTE b);
    static void dispPadInfo();
};

#endif /* SYSTEM_DEBUGPROCINFO_H */
