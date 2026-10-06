#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZ.h"
#include "System/Graphic/Effect/libppp/draw/sin_t.h"

void pppGetRotMatrixZ(sceVu0FMATRIX mp, word r) {
    float s;
    float c;

    s = eff_sin_t[((u_int)r >> 4) & 0xfff];
    c = eff_sin_t[((u_int)(r + 0x4000) >> 4) & 0xfff];

    mp[0][0] = c;
    mp[1][0] = -s;
    mp[2][0] = 0.0f;
    mp[3][0] = 0.0f;
    mp[0][1] = s;
    mp[1][1] = c;
    mp[2][1] = 0.0f;
    mp[3][1] = 0.0f;
    mp[0][2] = 0.0f;
    mp[1][2] = 0.0f;
    mp[2][2] = 1.0f;
    mp[3][2] = 0.0f;
    mp[0][3] = 0.0f;
    mp[1][3] = 0.0f;
    mp[2][3] = 0.0f;
    mp[3][3] = 1.0f;
}
