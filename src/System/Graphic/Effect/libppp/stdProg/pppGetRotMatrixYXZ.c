#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixYXZ.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixX.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixY.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZ.h"

void pppGetRotMatrixYXZ(sceVu0FMATRIX mp, pppIVECTOR *ang) {
    sceVu0FMATRIX m0;
    sceVu0FMATRIX m1;
    sceVu0FMATRIX m01;
    sceVu0FMATRIX m2;

    pppGetRotMatrixZ(m0, ang->z);
    pppGetRotMatrixX(m1, ang->x);
    sceVu0MulMatrix(m01, m0, m1);
    pppGetRotMatrixY(m2, ang->y);
    sceVu0MulMatrix(mp, m01, m2);
}
