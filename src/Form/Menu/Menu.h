#ifndef FORM_MENU_MENU_H
#define FORM_MENU_MENU_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/PadHandler.h"

class CMenu : public CTask, public CPadHandler {
protected:
    unsigned char (*m_nCardKind10)[10]; /* 0x50 */
    USHORT* m_nCardKind;                /* 0x54 */
    HANDLE m_hLay;                      /* 0x58 */
    INT m_nStep;                        /* 0x5c */
    FLOAT m_fTimer;                     /* 0x60 */
    LPCTSTR m_pLadName;                 /* 0x64 */
    LPCTSTR m_pSeqName;                 /* 0x68 */
    INT m_nCardKindMax;                 /* 0x6c */
    INT m_fgPadDisable;                 /* 0x70 */
    BOOL m_bFpsHalf;                    /* 0x74 */
    INT m_buttonTrg;                    /* 0x78 */

public:
    CMenu(CTask* p, INT fgUseCardArray);
    virtual ~CMenu();

protected:
    INT _isButtonTrg(UINT b, UINT nInPad);
    INT _isButtonDown(UINT b, UINT nInPad);
    INT _isButtonRTrg(UINT b, UINT nInPad);
    INT _isButtonRpt(UINT b, UINT nInPad);
    INT _getAnalogRX(UINT nInPad);
    INT _getAnalogRY(UINT nInPad);
    INT _getAnalogLX(UINT nInPad);
    INT _getAnalogLY(UINT nInPad);
    BOOL _isTrgOK(UINT nInPad);
    BOOL _isTrgCANCEL(UINT nInPad);
    BOOL _isTrgACTION(UINT nInPad);
    BOOL _isTrgNEXT(UINT nInPad);
    void initCardData(INT initKind);
};

#endif /* FORM_MENU_MENU_H */
