
#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_RYJMEGABIRTHUSERCTRL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_RYJMEGABIRTHUSERCTRL_H

#include "System/Graphic/Effect/libppp/partMng.h"

void RyjMegaBirthUserCtrDrawShapeRefPos(int shape_id, pppCVECTOR *color, u_char fog, u_char near);
void RyjMegaBirthUserCtrDrawShape(int shape_id, pppCVECTOR *color, u_char fog, u_char near);
void RyjMegaBirthUserCtrDrawShapeLine(int shape_id, pppCVECTOR *color, u_char fog, u_char near, float shape_scale, float length, u_int num);
void RyjMegaBirthUserCtrlDrawModel(int model_id, pppCVECTOR *color, u_char fog, u_char near);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_RYJPROG_RYJMEGABIRTHUSERCTRL_H */
