#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixXYZ.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixX.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixY.h"
#include "System/Graphic/Effect/libppp/stdProg/pppGetRotMatrixZ.h"

void pppGetRotMatrixXYZ(sceVu0FMATRIX mp, pppIVECTOR *ang) {
    sceVu0FMATRIX m0;
    sceVu0FMATRIX m1;
    sceVu0FMATRIX m01;
    sceVu0FMATRIX m2;

    pppGetRotMatrixZ(m0, ang->z);
    pppGetRotMatrixY(m1, ang->y);
    sceVu0MulMatrix(m01, m0, m1);
    pppGetRotMatrixX(m2, ang->x);
    sceVu0MulMatrix(mp, m01, m2);
}
