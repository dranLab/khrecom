#include "Game/Debug/DebugWork.h"
#include <string.h>

DEBUG_WORK DebugWork::g_dw;

void DebugWork::initialize() {
    memset(&g_dw, 0, sizeof(g_dw));

    g_dw.nakamich.iCamSpeed = 1;
    g_dw.sys.m_iBlurAlpha = 0x44;
    g_dw.nakamich.fCamVAngle = 53.1f;
    g_dw.sys.m_fGlareMaxPer = 1.0f;
    g_dw.sys.m_iBlurCR = 0x80;
    g_dw.sys.m_iBlurCG = 0x80;
    g_dw.sys.m_iBlurCB = 0x80;
    g_dw.sys.m_iBlurCA = 0x40;
    g_dw.sys.m_iFukidasiDisp = 1;
    g_dw.sys.m_fMapBright = 1.0f;
    g_dw.sys.m_iDmgDisp = 0;
    g_dw.event.iAtelCoord = 0;
    g_dw.event.iNpcCoordDisp = 0;
    g_dw.event.iNpcID = 0;
    g_dw.event.iEventline = 0;
    g_dw.event.pClass = NULL;
    g_dw.event.pEventlineClass = NULL;
    memset(&g_dw.atelwork, 0, sizeof(g_dw.atelwork));
    memset(&g_dw.gimPrize, 0, sizeof(g_dw.gimPrize));
    memset(&g_dw.jiminy, 0, sizeof(g_dw.jiminy));
    memset(&g_dw.gimDropProb, 0, sizeof(g_dw.gimDropProb));

    g_dw.sys.m_iDispCounter = 1;
    g_dw.fog.Near = 20;
    g_dw.fog.Far = 100;
    g_dw.fog.r = 0x80;
    g_dw.fog.g = 0x80;
    g_dw.fog.b = 0x80;
    g_dw.fog.max = 255;
    g_dw.fog.min = 0;
    g_dw.fog.inc = 10;
    g_dw.gimthrow.DirPow = 1.0f;
    g_dw.blur.Alpha = 0x40;
    g_dw.gimthrow.MaxAngle = 45.0f;
    g_dw.gimthrow.MinAngle = 0.0f;
    g_dw.blur.Xofs = 10;
    g_dw.blur.Yofs = 10;
    g_dw.player.iCardNumLock = -1;
    g_dw.sound.iSE_Handle = -1;
}

// SOURCE DATA: .bss
