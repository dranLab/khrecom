// PARTIAL: added for System/Graphic/Effect/libppp/keProg/pppKeMdlTfd
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEMDL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEMDL_H

#include "System/Graphic/Effect/libppp/pppPart.h"

void KeMdl_MkTkClut(u_int colBs, u_int colSs, float lv, u_int *pCltBuf);
void KeMdl_Draw(int mdlIdx, u_short *pCol, int cOfs, int u, int v, u_char flg, sceVu0FMATRIX *zMat);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_KEPROG_KEMDL_H */
