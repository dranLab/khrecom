#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZXY.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixX.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixY.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZ.h"

void pppGetRotMatrixZXY(sceVu0FMATRIX mp, pppIVECTOR *ang) {
    sceVu0FMATRIX m0;
    sceVu0FMATRIX m1;
    sceVu0FMATRIX m01;
    sceVu0FMATRIX m2;

    pppGetRotMatrixY(m0, ang->y);
    pppGetRotMatrixX(m1, ang->x);
    sceVu0MulMatrix(m01, m0, m1);
    pppGetRotMatrixZ(m2, ang->z);
    sceVu0MulMatrix(mp, m01, m2);
}
