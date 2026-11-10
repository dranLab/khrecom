// System/Graphic/VuCode/vu0.h: the original's VU0 inlines (c:/KHCOM/MainApp/System/Graphic/VuCode/vu0.h). The ref
// has no such file (no types are defined in it); its inlines and their lines come from the original's .mdebug line
// markers, the names are ours. System/Coord.h includes it after the vector classes and before FVECTOR::set's
// definition (Coord.h:730): an inline here that calls FVECTOR::set keeps the call (ModelMan reset/setupDrawEnv).
#ifndef SYSTEM_GRAPHIC_VUCODE_VU0_H
#define SYSTEM_GRAPHIC_VUCODE_VU0_H

/* BEGIN GEN */
#line 15
inline void vu0SetClipRange() {
    FVECTOR vMax;
    FVECTOR vMin;
    vMax.set(4096.0f, 4096.0f, 0.0f, 65535.0f);
    vMin.set(0.0f, 0.0f, 0.0f, 0.0f);





    asm volatile("lqc2 $vf1, 0x0(%0)\n"
                 "lqc2 $vf2, 0x0(%1)\n"
                 "viaddi $vi1, $vi0, 1\n"
                 "vsqi.xyzw $vf1, ($vi1++)\n"
                 "vsqi.xyzw $vf2, ($vi1++)" : : "r"(&vMax), "r"(&vMin));
}
#line 36
inline void vu0SetFogParam(FLOAT fFar) {
    FVECTOR vFogParam;
    vFogParam.set(0.0f, fFar, 0.0f, 0.0f);





    asm volatile("lqc2 $vf1, 0x0(%0)\n"
                 "viaddi $vi1, $vi0, 0\n"
                 "vsqi.xyzw $vf1, ($vi1++)" : : "r"(&vFogParam));
}
#line 55
inline void vu0CheckBBox(FVECTOR* v, const FMATRIX* m) {







    asm volatile("lqc2 $vf20, 0x0(%0)\n"
                 "lqc2 $vf27, 0x10(%0)\n"
                 "lqc2 $vf21, 0x0(%1)\n"
                 "lqc2 $vf22, 0x10(%1)\n"
                 "lqc2 $vf23, 0x20(%1)\n"
                 "lqc2 $vf24, 0x30(%1)\n"
                 "vcallms 0x20" : : "r"(v), "r"(m));
}
#line 114
inline void vu0LoadMatrixVF20(const FMATRIX* m) {




    asm volatile("lqc2 $vf20, 0x0(%0)\n"
                 "lqc2 $vf21, 0x10(%0)\n"
                 "lqc2 $vf22, 0x20(%0)\n"
                 "lqc2 $vf23, 0x30(%0)" : : "r"(m));
}
#line 129
inline void vu0LoadMatrixVF28(const FMATRIX* m) {


    asm volatile("lqc2 $vf28, 0x0(%0)\n"
                 "lqc2 $vf29, 0x10(%0)\n"
                 "lqc2 $vf30, 0x20(%0)\n"
                 "lqc2 $vf31, 0x30(%0)" : : "r"(m));
}
#line 190
inline void vu0StoreMatrixVF16(FMATRIX* m) {




    asm volatile("sqc2 $vf16, 0x0(%0)\n"
                 "sqc2 $vf17, 0x10(%0)\n"
                 "sqc2 $vf18, 0x20(%0)\n"
                 "sqc2 $vf19, 0x30(%0)" : : "r"(m) : "memory");
}
#line 246
inline void vu0StoreVF2(FVECTOR* v) {





    asm volatile("sqc2 $vf2, 0x0(%0)" : : "r"(v) : "memory");
}
#line 256
inline void vu0StoreVF3(FVECTOR* v) {





    asm volatile("sqc2 $vf3, 0x0(%0)" : : "r"(v) : "memory");
}
#line 331
inline void vu0WaitMicro() {





    asm volatile("qmfc2.i $0, $vf0");
}
#line 348
inline void vu0MulMatrixVF28VF20() {


    asm volatile("vmulax.xyzw $ACC, $vf28, $vf20x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf20y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf20z\n"
                 "vmaddw.xyzw $vf16, $vf31, $vf20w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf21x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf21y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf21z\n"
                 "vmaddw.xyzw $vf17, $vf31, $vf21w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf22x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf22y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf22z\n"
                 "vmaddw.xyzw $vf18, $vf31, $vf22w\n"
                 "vmulax.xyzw $ACC, $vf28, $vf23x\n"
                 "vmadday.xyzw $ACC, $vf29, $vf23y\n"
                 "vmaddaz.xyzw $ACC, $vf30, $vf23z\n"
                 "vmaddw.xyzw $vf19, $vf31, $vf23w");
}
#line 383
inline void vu0UnitMatrix(FMATRIX& m) {

    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)" : : "r"(&m) : "memory");
}
#line 396
inline void vu0ApplyMatrix(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1) {


    asm volatile("lqc2 $vf8, 0x0(%2)\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x10(%1)\n"
                 "lqc2 $vf6, 0x20(%1)\n"
                 "lqc2 $vf7, 0x30(%1)\n"
                 "vmulax.xyzw $ACC, $vf4, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf5, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf6, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf7, $vf8w\n"
                 "sqc2 $vf8, 0x0(%0)" : : "r"(&v0), "r"(&m0), "r"(&v1) : "memory");
}
#line 415
inline void vu0ApplyMatrix33(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1) {


    asm volatile("lqc2 $vf8, 0x0(%2)\n"
                 "lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x10(%1)\n"
                 "lqc2 $vf6, 0x20(%1)\n"
                 "vmulax.xyz $ACC, $vf4, $vf8x\n"
                 "vmadday.xyz $ACC, $vf5, $vf8y\n"
                 "vmaddz.xyz $vf8, $vf6, $vf8z\n"
                 "sqc2 $vf8, 0x0(%0)" : : "r"(&v0), "r"(&m0), "r"(&v1) : "memory");
}
#line 430
inline void vu0MulMatrix(FMATRIX& m0, const FMATRIX& m1, const FMATRIX& m2) {










    asm volatile("lqc2 $vf2, 0x0(%1)\n"
                 "lqc2 $vf3, 0x10(%1)\n"
                 "lqc2 $vf4, 0x20(%1)\n"
                 "lqc2 $vf5, 0x30(%1)\n"
                 "lqc2 $vf6, 0x0(%2)\n"
                 "lqc2 $vf7, 0x10(%2)\n"
                 "lqc2 $vf8, 0x20(%2)\n"
                 "lqc2 $vf9, 0x30(%2)\n"
                 "vmulax.xyzw $ACC, $vf2, $vf6x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf6y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf6z\n"
                 "vmaddw.xyzw $vf6, $vf5, $vf6w\n"
                 "vmulax.xyzw $ACC, $vf2, $vf7x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf7y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf7z\n"
                 "vmaddw.xyzw $vf7, $vf5, $vf7w\n"
                 "vmulax.xyzw $ACC, $vf2, $vf8x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf8y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf8z\n"
                 "vmaddw.xyzw $vf8, $vf5, $vf8w\n"
                 "vmulax.xyzw $ACC, $vf2, $vf9x\n"
                 "vmadday.xyzw $ACC, $vf3, $vf9y\n"
                 "vmaddaz.xyzw $ACC, $vf4, $vf9z\n"
                 "vmaddw.xyzw $vf9, $vf5, $vf9w\n"
                 "sqc2 $vf6, 0x0(%0)\n"
                 "sqc2 $vf7, 0x10(%0)\n"
                 "sqc2 $vf8, 0x20(%0)\n"
                 "sqc2 $vf9, 0x30(%0)" : : "r"(&m0), "r"(&m1), "r"(&m2) : "memory");
}
#line 473
inline void vu0OuterProduct(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vsub.w $vf6, $vf6, $vf6\n"
                 "vopmula.xyz $ACC, $vf4, $vf5\n"
                 "vopmsub.xyz $vf6, $vf5, $vf4\n"
                 "sqc2 $vf6, 0x0(%0)" : : "r"(&v0), "r"(&v1), "r"(&v2) : "memory");
}
#line 535
inline FLOAT vu0LengthSq(const FVECTOR& v) {
    INT iTmp;
    FLOAT ret;
    /* MATCH: iTmp is an input the asm overwrites (qmfc2.ni into it, mtc1 from it; C has no GPR -> FPR move
     * without memory): read before written, it is live from the caller's entry and ranks as one use, which
     * gives the original's registers (s3 in EffectMan _updateCoord, s5/s6 in DispList's ShadowMap insert, t1 in
     * PlayerManager getWorldRotY); "=r" or "+r" do not */

    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vaddy.x $vf6, $vf0, $vf4y\n"
                 "vaddz.x $vf7, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf6, $vf4y\n"
                 "vmaddz.x $vf5, $vf7, $vf4z\n"
                 "qmfc2.ni %2, $vf5\n"
                 "mtc1 %2, %0" : "=f"(ret) : "r"(&v), "r"(iTmp));
    return ret;
}
#line 562
inline void vu0Normalize(FVECTOR& v0, const FVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vaddy.x $vf5, $vf0, $vf4y\n"
                 "vaddz.x $vf6, $vf0, $vf4z\n"
                 "vmulax.x $ACC, $vf4, $vf4x\n"
                 "vmadday.x $ACC, $vf5, $vf4y\n"
                 "vmaddz.x $vf5, $vf6, $vf4z\n"
                 "vrsqrt $Q, $vf0w, $vf5x\n"
                 "vsub.xyzw $vf6, $vf0, $vf0\n"
                 "vwaitq\n"
                 "vmulq.xyz $vf6, $vf4, $Q\n"
                 "sqc2 $vf6, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 606
inline void vu0DivVector(FVECTOR& v0, const FVECTOR& v1, FLOAT q) {








    asm volatile("lqc2 $vf4, 0x0(%0)" : : "r"(&v1)); register FLOAT q8 asm("$8") = q; asm volatile("qmtc2.ni %0, $vf5\n" "vdiv $Q, $vf0w, $vf5x\n" "vwaitq\n" "vmulq.xyzw $vf4, $vf4, $Q\n" "sqc2 $vf4, 0x0(%1)" : : "r"(q8), "r"(&v0) : "memory");
}
#line 620
inline void vu0DivVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT q) {








    asm volatile("lqc2 $vf4, 0x0(%0)" : : "r"(&v1)); register FLOAT q8 asm("$8") = q; asm volatile("qmtc2.ni %0, $vf5\n" "vdiv $Q, $vf0w, $vf5x\n" "vwaitq\n" "vmulq.xyz $vf4, $vf4, $Q\n" "sqc2 $vf4, 0x0(%1)" : : "r"(q8), "r"(&v0) : "memory");
}
#line 634
inline void vu0AddVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vadd.xyzw $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1), "r"(&v2) : "memory");
}
#line 645
inline void vu0SubVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vsub.xyzw $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1), "r"(&v2) : "memory");
}
#line 656
inline void vu0MulVector(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vmul.xyzw $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1), "r"(&v2) : "memory");
}
#line 667
inline void vu0ScaleVector(FVECTOR& v0, const FVECTOR& v1, FLOAT t) {




    /* MATCH: the mfc1 stays in the asm: from C (a $3 register local) the scheduler moves the caller's parameter
     * copies (Math3D QuaternionRotationAxis) */
    asm volatile("lqc2 $vf4, 0x0(%1)\n" "mfc1 $3, %2\n" "qmtc2.ni $3, $vf5\n" "vmulx.xyzw $vf4, $vf4, $vf5x\n" "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1), "f"(t) : "$3", "memory");
}
#line 679
inline void vu0ScaleVectorXYZ(FVECTOR& v0, const FVECTOR& v1, FLOAT t) {




    /* MATCH: the mfc1 stays in the asm (as in vu0ScaleVector): from C gcc loads a constant factor straight into
     * $3 (DispList CDispListShadowMap::insert: lui/mtc1/mfc1 in the original) */
    asm volatile("lqc2 $vf4, 0x0(%1)\n" "mfc1 $3, %2\n" "qmtc2.ni $3, $vf5\n" "vmulx.xyz $vf4, $vf4, $vf5x\n" "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1), "f"(t) : "$3", "memory");
}
#line 691
inline void vu0FTOI0Vector(IVECTOR& v0, const FVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vftoi0.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 700
inline void vu0FTOI4Vector(IVECTOR& v0, const FVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vftoi4.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 709
inline void vu0FTOI12Vector(IVECTOR& v0, const FVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vftoi12.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 718
inline void vu0FTOI15Vector(IVECTOR& v0, const FVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vftoi15.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 727
inline void vu0ITOF0Vector(FVECTOR& v0, const IVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vitof0.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 736
inline void vu0ITOF4Vector(FVECTOR& v0, const IVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vitof4.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 745
inline void vu0ITOF12Vector(FVECTOR& v0, const IVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vitof12.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 754
inline void vu0ITOF15Vector(FVECTOR& v0, const IVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "vitof15.xyzw $vf4, $vf4\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1) : "memory");
}
#line 768
inline void vu0AddVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vadd.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1), "r"(&v2) : "memory");
}
#line 781
inline void vu0SubVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vsub.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1), "r"(&v2) : "memory");
}
#line 794
inline void vu0MulVectorXYZ(FVECTOR& v0, const FVECTOR& v1, const FVECTOR& v2) {


    asm volatile("lqc2 $vf4, 0x0(%1)\n"
                 "lqc2 $vf5, 0x0(%2)\n"
                 "vmul.xyz $vf4, $vf4, $vf5\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&v1), "r"(&v2) : "memory");
}
#line 808
inline void vu0ApplyMatrixXYZ(FVECTOR& v0, const FMATRIX& m0, const FVECTOR& v1) {


    asm volatile("lqc2 $vf4, 0x0(%2)\n"
                 "lqc2 $vf5, 0x0(%1)\n"
                 "lqc2 $vf6, 0x10(%1)\n"
                 "lqc2 $vf7, 0x20(%1)\n"
                 "lqc2 $vf8, 0x30(%1)\n"
                 "vmulax.xyzw $ACC, $vf5, $vf4x\n"
                 "vmadday.xyzw $ACC, $vf6, $vf4y\n"
                 "vmaddaz.xyzw $ACC, $vf7, $vf4z\n"
                 "vmaddw.xyz $vf4, $vf8, $vf0w\n"
                 "sqc2 $vf4, 0x0(%0)" : : "r"(&v0), "r"(&m0), "r"(&v1) : "memory");
}
/* END GEN */

#endif /* SYSTEM_GRAPHIC_VUCODE_VU0_H */
