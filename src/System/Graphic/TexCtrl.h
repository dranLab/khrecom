#ifndef SYSTEM_GRAPHIC_TEXCTRL_H
#define SYSTEM_GRAPHIC_TEXCTRL_H

#include "khtypes.h"

#include "System/Task.h"

typedef unsigned char TIM2_UCHAR8;
typedef unsigned int TIM2_UINT32;
typedef unsigned short TIM2_UINT16;

struct TIM2_FILEHEADER {
    unsigned char FileId[4];
    TIM2_UCHAR8 FormatVersion;
    TIM2_UCHAR8 FormatId;
    TIM2_UINT16 Pictures;
    unsigned char pad[8];
};

class CTexAnm;
typedef CTask CBfObj;

class CTexCtrl : public CTask {
public:
    CTexCtrl(CBfObj* pInParent);
    virtual ~CTexCtrl();
    virtual void main();

private:
    void _scrollTexUV();
    static void _doTexScroll(TIM2_FILEHEADER& inTim2, INT nInType, INT inFpsCnt);

public:
    static INT StopUvScroll(CHAR* nITexName);
    static INT StartUvScroll(CHAR* nITexName);
    static INT InitUvScroll(CHAR* nITexName);
    static INT ChangeFPSUvScroll(CHAR* nITexName, INT nInFps);
    static void transPrefixTex();
    static void addFixTexAnm(CTexAnm* pTexAnm, INT nTexAnm);
    static void delFixTexAnm(CTexAnm* pTexAnm, INT nTexAnm);
};

#endif /* SYSTEM_GRAPHIC_TEXCTRL_H */
