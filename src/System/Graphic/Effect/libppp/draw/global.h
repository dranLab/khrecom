#ifndef SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_GLOBAL_H
#define SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_GLOBAL_H

#include "System/Graphic/Effect/libppp/pppPart.h"

typedef struct { /* 0x60 */
    u_word size;
    u_word obj;
    u_word texture;
    u_word clut;
    u_word model;
    u_word vib;
    u_word sound;
    u_word camera;
    u_word particle;
    u_word etc;
    u_word res[5];
    u_word table;
    u_short initialize;
    u_short obj_c;
    u_short texture_c;
    u_short clut_c;
    u_short model_c;
    u_short vib_c;
    u_short sound_c;
    u_short camera_c;
    u_short particle_c;
    u_short etc_c;
    u_short res2[6];
} OEF2_HEADER;

typedef struct { /* 0x20 */
    u_word tex0_l;
    u_word tex0_h;
    u_char absx;
    u_char absy;
    u_char absw;
    u_char absh;
    u_short w;
    u_short h;
    u_word size;
    u_word data;
    u_char psm;
    u_char counter;
    char res[6];
} OEF2_TEXINFO;

typedef struct { /* 0x10 */
    u_short gs;
    u_short size;
    u_word data;
    u_char psm;
    u_char counter;
    char res[6];
} OEF2_CLUTINFO;

void opc();
void opc2();
void opc3();
void op_init_global();
void ops_init_spr();
void op_num_info(void *gs);
void *op_spr_alloc(word mem);
void *op_spr_rev_alloc(word mem);
void op_spr_free(word mem);
void op_spr_rev_free(word mem);
int op_spr_rev_check(word mem);
int op_spr_check(word mem);
int op_file_read(char *fn, char *adr);
int op_file_size(char *fn);
void op_oef2_set(OEF2_HEADER *table, void *data);
void op_oef2_set2(OEF2_HEADER *table);
void op_oef2_set2_16(char *oef2_16);
void *op_oef2_a_get(OEF2_HEADER *table, int code, int number);
float op_random(float ran);
float op_random_p(float ran);

#endif /* SYSTEM_GRAPHIC_EFFECT_LIBPPP_DRAW_GLOBAL_H */
