#ifndef FORM_MENU_BTLREPORTCTRL_H
#define FORM_MENU_BTLREPORTCTRL_H

#include "Form/Menu/SaveLoadCtrl.h"
#include "System/Graphic/2d/2DFnt.h"

class C2DSeqStrings : public C2DSeqAgent {
private:
    D2::C2DSeqCtrl* m_pCtrl; /* 0x70 */
    D2::C2DFnt* m_pFnt;      /* 0x74 */
    char m_Str[128];         /* 0x78 */

public:
    C2DSeqStrings(CTask* pParent);
    virtual ~C2DSeqStrings();
    virtual void dispose();
    void setupSeq(SeqTbl stbl, const CHAR* pszLad, SHORT nUnit, const CHAR* pStr, SHORT nLayer, INT in_FontSize);
    void setPos(FVECTOR& vPos);
    void changeText(const CHAR* pStr);
    void setStyle(INT in_Style);
    void setDisp(BOOL in_Flag);
    D2::C2DSeqCtrl* getSeqCtrl() { return m_pCtrl; }
};

#endif /* FORM_MENU_BTLREPORTCTRL_H */
