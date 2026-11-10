// System/Graphic/VuCode/vu0.h: the original's VU0 inlines (c:/KHCOM/MainApp/System/Graphic/VuCode/vu0.h). The ref
// has no such file (no types are defined in it); its inlines and their lines come from the original's .mdebug line
// markers, the names are ours. System/Coord.h includes it after the vector classes and before FVECTOR::set's
// definition (Coord.h:730): an inline here that calls FVECTOR::set keeps the call (ModelMan reset/setupDrawEnv).
#ifndef SYSTEM_GRAPHIC_VUCODE_VU0_H
#define SYSTEM_GRAPHIC_VUCODE_VU0_H

/* 391: the unit matrix (FMATRIX::init at Coord.h:910 calls it) */
#line 383
inline void vu0UnitMatrix(FMATRIX& m) {
    asm volatile("vmr32.xyzw $vf1, $vf0\n"
                 "sqc2 $vf0, 0x30(%0)\n"
                 "vmr32.xyzw $vf2, $vf1\n"
                 "sqc2 $vf1, 0x20(%0)\n"
                 "vmr32.xyzw $vf3, $vf2\n"
                 "sqc2 $vf2, 0x10(%0)\n"
                 "sqc2 $vf3, 0x0(%0)\n"
                 : : "r"(&m) : "memory");
}

#endif /* SYSTEM_GRAPHIC_VUCODE_VU0_H */
