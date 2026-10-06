#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZYX.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixX.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixY.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZ.h"

void pppGetRotMatrixZYX(sceVu0FMATRIX mp, pppIVECTOR *ang) {
    sceVu0FMATRIX m0;
    sceVu0FMATRIX m1;
    sceVu0FMATRIX m01;
    sceVu0FMATRIX m2;

    pppGetRotMatrixX(m0, ang->x);
    pppGetRotMatrixY(m1, ang->y);
    sceVu0MulMatrix(m01, m0, m1);
    pppGetRotMatrixZ(m2, ang->z);
    sceVu0MulMatrix(mp, m01, m2);
}
