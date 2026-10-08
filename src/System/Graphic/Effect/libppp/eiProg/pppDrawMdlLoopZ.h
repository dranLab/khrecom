#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLLOOPZ_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLLOOPZ_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x28 */
    pppCDT cdt;
    int m_model;
    float m_tu;
    float m_dtu;
    float m_ddtu;
    float m_tv;
    float m_dtv;
    float m_ddtv;
    u_char m_acalc;
    u_char m_afix;
    u_char m_draw;
    u_char m_fog;
    u_char m_near;
    u_char m_scis;
    u_char m_zCrct;
} PDrawMdlLoopZ;

typedef struct { /* 0x20 */
    void *m_pp;
    float m_tu;
    float m_dtu;
    float m_ddtu;
    float m_tv;
    float m_dtv;
    float m_ddtv;
    u_char m_uv;
} VDrawMdlLoopZ;

void pppDrawMdlLoopZConstruct(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlLoopZConstruct2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlLoopZDestruct(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlLoopZCalc(pppPObject *pobj, PDrawMdlLoopZ *p, pppCtrlTable *ctbl);
void pppDrawMdlLoopZDraw(pppPObject *pobj, PDrawMdlLoopZ *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLLOOPZ_H */
