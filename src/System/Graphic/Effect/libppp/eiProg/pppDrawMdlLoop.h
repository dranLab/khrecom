#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLLOOP_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLLOOP_H

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
} PDrawMdlLoop;

typedef struct { /* 0x20 */
    void *m_pp;
    float m_tu;
    float m_dtu;
    float m_ddtu;
    float m_tv;
    float m_dtv;
    float m_ddtv;
    u_char m_uv;
} VDrawMdlLoop;

void pppDrawMdlLoopConstruct(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlLoopConstruct2(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlLoopDestruct(pppPObject *pobj, pppCtrlTable *ctbl);
void pppDrawMdlLoopCalc(pppPObject *pobj, PDrawMdlLoop *p, pppCtrlTable *ctbl);
void pppDrawMdlLoopDraw(pppPObject *pobj, PDrawMdlLoop *p, pppCtrlTable *ctbl);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_EIPROG_PPPDRAWMDLLOOP_H */
