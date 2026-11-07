/* libat3dec.c (ATRAC3 decoder core, middleware) */
#include "include_asm.h"

void *memcpy(void *dst, const void *src, unsigned int n);

/* joint stereo weights of the previous (0) and the current (1) frame */
typedef struct {
    unsigned short *w[4]; /* per band: weights of the two channels */
    unsigned short *g; /* gains */
    int n; /* bands below n use gain 0x8000 */
} At3Joint;

/* decoder work area (no type information: fields named by use) */
typedef struct {
    At3Joint joint[2]; /* 0x0000 */
    char ch[2][0x33E0]; /* 0x0030: per-channel state */
    int maxpos; /* 0x67F0: highest coefficient written */
    int nch; /* 0x67F4 */
    int mode; /* 0x67F8 */
    int frameSize; /* 0x67FC */
    unsigned char *stream; /* 0x6800 */
    unsigned char buf[0xC0]; /* 0x6804: byte-reversed second channel */
    int qoff; /* 0x68C4 */
    int bitpos; /* 0x68C8 */
} At3Work;

void iqmf(char *ch);
extern short a_wind[];
extern unsigned short chswTable[][2];
extern unsigned short chwtTable[][2];
void *memset(void *dst, int c, unsigned int n);
int imdct(char *ch, int band, int gain);
extern unsigned short subgaintable[];
extern const int a_qtstart[];
void __assert(const char *file, int line, const char *expr);

#define assert(e) ((e) ? (void)0 : __assert(__FILE__, __LINE__, #e))
#define AT2_NQUS 32
#define QTSTART(i) (assert((unsigned int)i < AT2_NQUS+1), a_qtstart[i])
extern signed char huf_maxlen[7][2];
extern signed char *huf_prefixtbl[7][2];
extern signed char *huf_lentbl[7][2];
extern int *huf_deqtbl[3][7];

typedef struct {
    int w[23];
} At3Blk23;

/* one of the 4 QMF bands of a channel (At3Chan.band) */
typedef struct {
    At3Blk23 hdr; /* 0x000 */
    int spec[233]; /* 0x05C */
    At3Blk23 save; /* 0x400 */
    int ov[128]; /* 0x45C */
    int x65C;
} At3Band;

/* the state of one channel (At3Work.ch) */
typedef struct {
    At3Band band[4]; /* 0x0000 */
    char _pad1980[0x1A40 - 0x1980];
    int spec[1024]; /* 0x1A40 */
    char _pad2A40[0x2A60 - 0x2A40];
    At3Blk23 qmfhead; /* 0x2A60 */
    char _pad2ABC[0x3260 - 0x2ABC];
    At3Blk23 qmfsave; /* 0x3260 */
    char _pad32BC[0x32D4 - 0x32BC];
    int gain[4][8][2]; /* 0x32D4 */
    char _pad33D4[0x33E0 - 0x33D4];
} At3Chan;

void h_d_make(float *a, float *b, float *c, float *d);
void h_iqmf_fb1(float *in, float *out, int odd);
void h_iqmf_fb2(float *p);

int coefpost[768] = {
    0x1E81, 0x0000, 0x349E, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xDDF7, 0x0000, 0x7F8C, 0x0000, 0x0F4E, 0x0000, 0x3CF6, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF724, 0x0000, 0x42C3, 0x0000,
    0x2549, 0x0000, 0x2FE8, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xCAC7, 0x0000, 0x9C42, 0x0000, 0x070C, 0x0000, 0x4156, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFDE1, 0x0000, 0x20E5, 0x0000,
    0x1B1E, 0x0000, 0x36A7, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xE55E, 0x0000, 0x71B2, 0x0000, 0x131B, 0x0000, 0x3B00, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF295, 0x0000, 0x51CB, 0x0000,
    0x2811, 0x0000, 0x2D9B, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xC0D1, 0x0000, 0xA866, 0x0000, 0x0389, 0x0000, 0x4362, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFF6B, 0x0000, 0x1144, 0x0000,
    0x2072, 0x0000, 0x335F, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xD91E, 0x0000, 0x87A1, 0x0000, 0x0D05, 0x0000, 0x3E24, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF96E, 0x0000, 0x399F, 0x0000,
    0x2387, 0x0000, 0x313C, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xD065, 0x0000, 0x94B0, 0x0000, 0x0940, 0x0000, 0x4020, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFC7F, 0x0000, 0x2A38, 0x0000,
    0x1900, 0x0000, 0x37D9, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xE966, 0x0000, 0x692D, 0x0000, 0x155A, 0x0000, 0x39D2, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xEF68, 0x0000, 0x5AAA, 0x0000,
    0x29A7, 0x0000, 0x2C2B, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xBA7D, 0x0000, 0xAF62, 0x0000, 0x018F, 0x0000, 0x44A3, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFFE1, 0x0000, 0x07DA, 0x0000,
    0x1DD7, 0x0000, 0x3508, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xDF83, 0x0000, 0x7CD0, 0x0000, 0x1012, 0x0000, 0x3C91, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF64E, 0x0000, 0x45CB, 0x0000,
    0x25DB, 0x0000, 0x2F75, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xC8D9, 0x0000, 0x9EBC, 0x0000, 0x0653, 0x0000, 0x41BE, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFE43, 0x0000, 0x1DC7, 0x0000,
    0x1BCF, 0x0000, 0x3640, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xE3F4, 0x0000, 0x7480, 0x0000, 0x1259, 0x0000, 0x3B64, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF391, 0x0000, 0x4ECF, 0x0000,
    0x2786, 0x0000, 0x2E13, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xC2DE, 0x0000, 0xA605, 0x0000, 0x0437, 0x0000, 0x42F8, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFF30, 0x0000, 0x1466, 0x0000,
    0x2114, 0x0000, 0x32F3, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xD770, 0x0000, 0x8A49, 0x0000, 0x0C42, 0x0000, 0x3E89, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFA1F, 0x0000, 0x368E, 0x0000,
    0x22ED, 0x0000, 0x31AB, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xD234, 0x0000, 0x921F, 0x0000, 0x09FF, 0x0000, 0x3FBA, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFBF5, 0x0000, 0x2D50, 0x0000,
    0x1848, 0x0000, 0x383F, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xEAAB, 0x0000, 0x664E, 0x0000, 0x1617, 0x0000, 0x396E, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xEE47, 0x0000, 0x5D99, 0x0000,
    0x2A2B, 0x0000, 0x2BAD, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xB852, 0x0000, 0xB1A8, 0x0000, 0x00EC, 0x0000, 0x450F, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFFF5, 0x0000, 0x04B6, 0x0000,
    0x1F29, 0x0000, 0x3435, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xDC62, 0x0000, 0x8243, 0x0000, 0x0E8B, 0x0000, 0x3D5A, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF7F1, 0x0000, 0x3FBA, 0x0000,
    0x24B5, 0x0000, 0x305A, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xCCAE, 0x0000, 0x99C2, 0x0000, 0x07C6, 0x0000, 0x40EE, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFD74, 0x0000, 0x2402, 0x0000,
    0x1A6B, 0x0000, 0x370D, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xE6BF, 0x0000, 0x6EDF, 0x0000, 0x13DB, 0x0000, 0x3A9B, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF18F, 0x0000, 0x54C3, 0x0000,
    0x289A, 0x0000, 0x2D22, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xBEBC, 0x0000, 0xAAC1, 0x0000, 0x02DD, 0x0000, 0x43CC, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFF9C, 0x0000, 0x0E21, 0x0000,
    0x1FCE, 0x0000, 0x33CA, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xDAC4, 0x0000, 0x84F5, 0x0000, 0x0DC8, 0x0000, 0x3DBF, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF8B4, 0x0000, 0x3CAE, 0x0000,
    0x241F, 0x0000, 0x30CC, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xCE8E, 0x0000, 0x973C, 0x0000, 0x0882, 0x0000, 0x4087, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFCFE, 0x0000, 0x271E, 0x0000,
    0x19B6, 0x0000, 0x3773, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xE817, 0x0000, 0x6C08, 0x0000, 0x149B, 0x0000, 0x3A37, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF080, 0x0000, 0x57B9, 0x0000,
    0x2921, 0x0000, 0x2CA7, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xBCA0, 0x0000, 0xAD14, 0x0000, 0x0234, 0x0000, 0x4437, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFFC4, 0x0000, 0x0AFE, 0x0000,
    0x1D2C, 0x0000, 0x3570, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xE107, 0x0000, 0x7A10, 0x0000, 0x10D5, 0x0000, 0x3C2D, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF56E, 0x0000, 0x48CF, 0x0000,
    0x266B, 0x0000, 0x2F00, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xC6E2, 0x0000, 0xA130, 0x0000, 0x059D, 0x0000, 0x4226, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFE9C, 0x0000, 0x1AA8, 0x0000,
    0x1C7E, 0x0000, 0x35D8, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xE282, 0x0000, 0x774A, 0x0000, 0x1197, 0x0000, 0x3BC8, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xF484, 0x0000, 0x4BD1, 0x0000,
    0x26FA, 0x0000, 0x2E8A, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xC4E4, 0x0000, 0xA39E, 0x0000, 0x04E9, 0x0000, 0x428F, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFEEB, 0x0000, 0x1787, 0x0000,
    0x21B4, 0x0000, 0x3287, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xD5BA, 0x0000, 0x8CEB, 0x0000, 0x0B80, 0x0000, 0x3EEF, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFAC5, 0x0000, 0x337C, 0x0000,
    0x2251, 0x0000, 0x3219, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xD3FB, 0x0000, 0x8F88, 0x0000, 0x0ABF, 0x0000, 0x3F54, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFB62, 0x0000, 0x3067, 0x0000,
    0x178E, 0x0000, 0x38A4, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xEBE8, 0x0000, 0x636B, 0x0000, 0x16D3, 0x0000, 0x3909, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xED1C, 0x0000, 0x6084, 0x0000,
    0x2AAD, 0x0000, 0x2B2E, 0x0000, 0x7FFF, 0x0000, 0x7FFF, 0x0000,
    0xB620, 0x0000, 0xB3E8, 0x0000, 0x004E, 0x0000, 0x457C, 0x0000,
    0x7FFF, 0x0000, 0x7FFF, 0x0000, 0xFFFF, 0x0000, 0x0192, 0x0000,
};

int cbi[508] = {
    0xFFFB, 0x0000, 0xFFFB, 0x0000, 0xFFEC, 0x0000, 0xFFEC, 0x0000,
    0x0324, 0x0000, 0x0324, 0x0000, 0xFFB1, 0x0000, 0xFFB1, 0x0000,
    0xB73A, 0x0000, 0xB73A, 0x0000, 0x0648, 0x0000, 0x0648, 0x0000,
    0xB2C9, 0x0000, 0xB2C9, 0x0000, 0xFEC4, 0x0000, 0xFEC4, 0x0000,
    0xEDB3, 0x0000, 0xEDB3, 0x0000, 0xB968, 0x0000, 0xB968, 0x0000,
    0x5F0F, 0x0000, 0x5F0F, 0x0000, 0x0C90, 0x0000, 0x0C90, 0x0000,
    0xEB4B, 0x0000, 0xEB4B, 0x0000, 0xB086, 0x0000, 0xB086, 0x0000,
    0x64DD, 0x0000, 0x64DD, 0x0000, 0xFB15, 0x0000, 0xFB15, 0x0000,
    0xFBAD, 0x0000, 0xFBAD, 0x0000, 0xEED9, 0x0000, 0xEED9, 0x0000,
    0x2EDC, 0x0000, 0x2EDC, 0x0000, 0xBDAF, 0x0000, 0xBDAF, 0x0000,
    0xD318, 0x0000, 0xD318, 0x0000, 0x5C22, 0x0000, 0x5C22, 0x0000,
    0x90D4, 0x0000, 0x90D4, 0x0000, 0x1918, 0x0000, 0x1918, 0x0000,
    0xFA73, 0x0000, 0xFA73, 0x0000, 0xEA0A, 0x0000, 0xEA0A, 0x0000,
    0x3505, 0x0000, 0x3505, 0x0000, 0xABEB, 0x0000, 0xABEB, 0x0000,
    0xD696, 0x0000, 0xD696, 0x0000, 0x67BE, 0x0000, 0x67BE, 0x0000,
    0x8B9A, 0x0000, 0x8B9A, 0x0000, 0xEC83, 0x0000, 0xEC83, 0x0000,
    0xFF0E, 0x0000, 0xFF0E, 0x0000, 0xFC3B, 0x0000, 0xFC3B, 0x0000,
    0x15F7, 0x0000, 0x15F7, 0x0000, 0xF109, 0x0000, 0xF109, 0x0000,
    0xC3E2, 0x0000, 0xC3E2, 0x0000, 0x2BC4, 0x0000, 0x2BC4, 0x0000,
    0xA4D2, 0x0000, 0xA4D2, 0x0000, 0xC5E4, 0x0000, 0xC5E4, 0x0000,
    0xF40C, 0x0000, 0xF40C, 0x0000, 0xD14D, 0x0000, 0xD14D, 0x0000,
    0x4D50, 0x0000, 0x4D50, 0x0000, 0x563E, 0x0000, 0x563E, 0x0000,
    0xE33C, 0x0000, 0xE33C, 0x0000, 0x9368, 0x0000, 0x9368, 0x0000,
    0x75E6, 0x0000, 0x75E6, 0x0000, 0x31F1, 0x0000, 0x31F1, 0x0000,
    0xFE71, 0x0000, 0xFE71, 0x0000, 0xF9C8, 0x0000, 0xF9C8, 0x0000,
    0x1C38, 0x0000, 0x1C38, 0x0000, 0xE76C, 0x0000, 0xE76C, 0x0000,
    0xC7DE, 0x0000, 0xC7DE, 0x0000, 0x3817, 0x0000, 0x3817, 0x0000,
    0x9FF7, 0x0000, 0x9FF7, 0x0000, 0xA268, 0x0000, 0xA268, 0x0000,
    0xF5DF, 0x0000, 0xF5DF, 0x0000, 0xD848, 0x0000, 0xD848, 0x0000,
    0x474D, 0x0000, 0x474D, 0x0000, 0x6D74, 0x0000, 0x6D74, 0x0000,
    0xE046, 0x0000, 0xE046, 0x0000, 0x88F6, 0x0000, 0x88F6, 0x0000,
    0x7B70, 0x0000, 0x7B70, 0x0000, 0xB505, 0x0000, 0xB505, 0x0000,
    0xFFD4, 0x0000, 0xFFD4, 0x0000, 0xFF4E, 0x0000, 0xFF4E, 0x0000,
    0x096C, 0x0000, 0x096C, 0x0000, 0xFD3B, 0x0000, 0xFD3B, 0x0000,
    0xBB8F, 0x0000, 0xBB8F, 0x0000, 0x12D5, 0x0000, 0x12D5, 0x0000,
    0xAE3C, 0x0000, 0xAE3C, 0x0000, 0xF4FA, 0x0000, 0xF4FA, 0x0000,
    0xEFF5, 0x0000, 0xEFF5, 0x0000, 0xC1D8, 0x0000, 0xC1D8, 0x0000,
    0x5932, 0x0000, 0x5932, 0x0000, 0x2590, 0x0000, 0x2590, 0x0000,
    0xE8BF, 0x0000, 0xE8BF, 0x0000, 0xA736, 0x0000, 0xA736, 0x0000,
    0x6A9B, 0x0000, 0x6A9B, 0x0000, 0xD4DB, 0x0000, 0xD4DB, 0x0000,
    0xFCC0, 0x0000, 0xFCC0, 0x0000, 0xF314, 0x0000, 0xF314, 0x0000,
    0x28AB, 0x0000, 0x28AB, 0x0000, 0xCD9F, 0x0000, 0xCD9F, 0x0000,
    0xCF7A, 0x0000, 0xCF7A, 0x0000, 0x504D, 0x0000, 0x504D, 0x0000,
    0x95F7, 0x0000, 0x95F7, 0x0000, 0x4A50, 0x0000, 0x4A50, 0x0000,
    0xF913, 0x0000, 0xF913, 0x0000, 0xE4AA, 0x0000, 0xE4AA, 0x0000,
    0x3B27, 0x0000, 0x3B27, 0x0000, 0x9880, 0x0000, 0x9880, 0x0000,
    0xD9F2, 0x0000, 0xD9F2, 0x0000, 0x731A, 0x0000, 0x731A, 0x0000,
    0x864C, 0x0000, 0x864C, 0x0000, 0x61F8, 0x0000, 0x61F8, 0x0000,
    0xFF85, 0x0000, 0xFF85, 0x0000, 0xFE13, 0x0000, 0xFE13, 0x0000,
    0x0FB3, 0x0000, 0x0FB3, 0x0000, 0xF854, 0x0000, 0xF854, 0x0000,
    0xBFC7, 0x0000, 0xBFC7, 0x0000, 0x1F56, 0x0000, 0x1F56, 0x0000,
    0xA994, 0x0000, 0xA994, 0x0000, 0xE1C6, 0x0000, 0xE1C6, 0x0000,
    0xF213, 0x0000, 0xF213, 0x0000, 0xC9D1, 0x0000, 0xC9D1, 0x0000,
    0x5348, 0x0000, 0x5348, 0x0000, 0x3E34, 0x0000, 0x3E34, 0x0000,
    0xE610, 0x0000, 0xE610, 0x0000, 0x9D80, 0x0000, 0x9D80, 0x0000,
    0x7049, 0x0000, 0x7049, 0x0000, 0x8E3A, 0x0000, 0x8E3A, 0x0000,
    0xFDAC, 0x0000, 0xFDAC, 0x0000, 0xF6BA, 0x0000, 0xF6BA, 0x0000,
    0x2274, 0x0000, 0x2274, 0x0000, 0xDB94, 0x0000, 0xDB94, 0x0000,
    0xCBBC, 0x0000, 0xCBBC, 0x0000, 0x4447, 0x0000, 0x4447, 0x0000,
    0x9B03, 0x0000, 0x9B03, 0x0000, 0x78AD, 0x0000, 0x78AD, 0x0000,
    0xF78C, 0x0000, 0xF78C, 0x0000, 0xDEBE, 0x0000, 0xDEBE, 0x0000,
    0x413F, 0x0000, 0x413F, 0x0000, 0x839C, 0x0000, 0x839C, 0x0000,
    0xDD2D, 0x0000, 0xDD2D, 0x0000, 0x7E2F, 0x0000, 0x7E2F, 0x0000,
    0x80E8, 0x0000, 0x80E8, 0x0000,
};

const short i_qmfCh[48] = {
    -6, -1, 21, -4, -58, 17, 139, -17, -288, 54, 536, -52,
    -919, -4, 1487, 168, -2332, -534, 3716, 1287, -6798, -2982, 31756, 9035,
    9035, 31756, -2982, -6798, 1287, 3716, -534, -2332, 168, 1487, -4, -919,
    -52, 536, 54, -288, -17, 139, 17, -58, -4, 21, -1, -6,
};

const short i_qmfBh[24] = {
    1, 4, -17, 17, -54, 52, 4, -168, 534, -1287, 2982, -9035,
    -31756, 6798, -3716, 2332, -1487, 919, -536, 288, -139, 58, -21, 6,
};

const short i_qmfAh[24] = {
    6, -21, 58, -139, 288, -536, 919, -1487, 2332, -3716, 6798, -31756,
    -9035, 2982, -1287, 534, -168, 4, 52, -54, 17, -17, 4, 1,
};

static void channel_conv_joint(At3Work *w);
static int gainc_window(char *ch, int band);
static void f2t_nodata(At3Band *bands, int band);

/* Parked: close: loop.c bases the inner loop on the channel 0 samples (the original on channel 1) and strength-reduces the
   band offset (the original recomputes (3 * band) * 136 from a counter) */
#if 0
static void channel_conv_joint(At3Work *w)
{
    int band;
    int k;
    unsigned short w0, w1, g0, g1;
    unsigned short n0, n1, h0, h1;
    unsigned short dw0, dw1, dg0, dg1;
    int cw0, cw1, cg0, cg1;
    int ndw0, ndw1, ndg0, ndg1;
    unsigned short *pw;
    unsigned short *nw;
    int (*sp)[3320];
    int idx;
    int *pr;
    int x;
    int y;
    int m;
    int j;

    sp = (int (*)[3320])w->ch;
    j = 0;
    for (band = 0; band < 4; band++) {
        g1 = 0x8000;
        g0 = 0x8000;
        h1 = 0x8000;
        h0 = 0x8000;
        if (band >= w->joint[1].n) {
            h1 = w->joint[1].g[1];
            h0 = w->joint[1].g[0];
        }
        if (band >= w->joint[0].n) {
            g1 = w->joint[0].g[1];
            g0 = w->joint[0].g[0];
        }
        nw = w->joint[1].w[band];
        pw = w->joint[0].w[band];
        w0 = pw[0];
        w1 = pw[1];
        dw0 = (nw[0] - w0) >> 3;
        dw1 = (nw[1] - w1) >> 3;
        dg0 = (short)(h0 - g0) >> 3;
        dg1 = (short)(h1 - g1) >> 3;
        cw0 = (dw0 << 3) + w0;
        cw1 = (dw1 << 3) + w1;
        cg0 = (dg0 << 3) + g0;
        cg1 = (dg1 << 3) + g1;
        idx = band * 408 + 35;
        ndw0 = -dw0;
        ndw1 = -dw1;
        ndg0 = -dg0;
        ndg1 = -dg1;
        pr = &((int (*)[136])(w->ch[1]))[j + band][23];
        for (k = 0; k < 256; k++) {
            n0 = w0;
            h0 = g0;
            n1 = w1;
            h1 = g1;
            if (k - 8 < 0) {
                n0 = cw0;
                h0 = cg0;
                n1 = cw1;
                h1 = cg1;
            }
            cw0 += ndw0;
            cw1 += ndw1;
            cg0 += ndg0;
            cg1 += ndg1;
            x = pr[k - 3320];
            y = pr[k];
            m = n0 * (x >> 4) + n1 * (y >> 4);
            pr[k - 3320] = ((m >> 8) * h0) >> 6;
            pr[k] = (((x - m) >> 8) * h1) >> 6;
        }
        j += 2;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", channel_conv_joint);
#endif

/* Parked: first draft (2026-11-07), 334 of 457 lines, 80 bytes short; the butterfly stage has its structure (registers renamed: w, half, step); the window loop
   keeps fewer values on the stack than the original (5 spills at 0x18-0x2C) and the band copy's tail is not interleaved with its setup;
   decomp-permuter 2 x 20 min: d >> shift in a local (-23), the rest loop-note hacks: the frame is 16 bytes larger (one more spill); the
   prologue through the shift search follows the original (movn abs, the shift loop from 12) apart from registers */
#if 0
typedef struct {
    int v[4];
} At3Tbl4;
extern unsigned short cc[];
extern short ca[];
extern int mx[];
extern int coefpost[];
void imdct_sub(int *a, int *b, int n);
void imdct_ch(int *a, int *b, int *coef, int *tbl);
void imdct_ch2(int *a, int *b, int *coef, int *tbl);

int imdct(char *ch, int band, int gain)
{
    At3Tbl4 tbl = {{0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF}};
    int *src;
    int *w = ((At3Chan *)ch)->spec + band * 256;
    int i, max, shift, step, half, k, n;
    int *p;
    At3Band *b;
    int *s;

    if (band & 1) {
        int *q = ((At3Chan *)ch)->spec + band * 256 + 127;
        int *d;
        src = w;
        w = ((At3Chan *)ch)->spec;
        d = w;
        for (i = 0; i < 128; i++) {
            d[0] = q[128] - *src++;
            d[128] = *q--;
            d++;
        }
    } else {
        int *q;
        w = ((At3Chan *)ch)->spec + band * 256;
        q = ((At3Chan *)ch)->spec + band * 256 + 255;
        src = w;
        for (i = 127; i >= 0; i--) {
            *src = *src - *q--;
            src++;
        }
    }
    max = 0;
    for (i = 127, p = w; i >= 0; i--, p++) {
        int x = p[128];
        int y = p[0];
        int d, a;
        x += (((x >> 15) * 0x6A0A) >> 1) + (((x & 0x7FFF) * 0x6A0A) >> 16);
        d = y - x;
        y = y + x;
        a = d;
        p[128] = d;
        if (a < 0) a = -a;
        p[0] = y;
        if (max < a) max = a;
        a = y;
        if (a < 0) a = -a;
        if (max < a) max = a;
    }
    if (max == 0) return 1;
    shift = 12;
    while (shift >= 2 && max >= (1 << (29 - shift))) shift--;
    for (i = 255, p = w; i >= 0; i--, p++) *p <<= shift;
    step = 64;
    do {
        n = step * 2;
        half = step / 2;
        do {
            k = n;
            p = w;
            if (k >= 5) {
                imdct_sub(p, p + n - 4, k);
                p += step;
            } else {
                int *q = p + k;
                do {
                    p[0] = (p[0] - q[-1]) >> 1;
                    p[1] = (p[1] - q[-2]) >> 1;
                    k -= 4;
                    p += 2;
                } while (k > 0);
            }
            if (p == w + 2) {
                imdct_ch2(w + step, w, &coefpost[0x2FC + half * 4], tbl.v);
                w = p;
            } else {
                do {
                    imdct_ch(w + step, w, &coefpost[0x2FC + half * 4], tbl.v);
                    w += 4;
                } while (w < p);
            }
            half += step;
            w += step;
        } while (half < 128);
        step >>= 1;
        w -= 256;
    } while (step != 1);
    b = &((At3Band *)ch)[band];
    memcpy(b, &b->save, 0xB8);
    s = b->spec;
    {
        int sh = gain + shift;
        int sh1 = sh - 1;
        unsigned short *c = &cc[127];
        int *m = &mx[127];
        int *v = w + 254;
        for (i = 127; i >= 0; i--) {
            int t1 = v[1];
            int t0 = v[0];
            int ci = *c--;
            int mi = *m--;
            int r, d, e, lo, hi, out, ds;
            int *ov;
            r = ci * (t1 >> 16) + ((ci * ((t1 >> 1) & 0x7FFF)) >> 15);
            v -= 2;
            d = (t0 - t1) >> 1;
            e = d - r;
            d = d + r;
            lo = (e & 0x7FFF) * ca[mi + 128];
            hi = (e >> 15) * ca[mi + 128];
            ov = &s[mi + 256];
            out = 0;
            if (sh1 < 20) out = (hi >> (sh - 4)) + (lo >> (sh + 11));
            ds = d >> shift;
            s[127 - mi] = ((*ov >> 1) - a_wind[mi] * out) >> 3;
            if (sh1 < 15) out = (hi << (15 - sh1)) + (lo >> sh1);
            else out = (hi >> (sh - 16)) + (lo >> sh1);
            s[mi + 128] = out + ((((short *)ov)[1] * a_wind[mi]) >> 3);
            *ov = (d << (15 - shift)) + ds * ca[mi];
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", imdct);
__asm__(".rdata\n.align 4\nD_004FCEF0:\n.word 0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF\n.text\n"); /* imdct's At3Tbl4 initializer */
#endif

#define GAINC_STEP()                                                    \
    *p = ((*p >> 11) * subgaintable[sh & 7]) >> ((sh >> 3) + 4);        \
    p++;                                                                \
    sh += step

static int gainc_window(char *ch, int band)
{
    int *g = (int *)(ch + 0x32D4) + band * 16;
    int *p = ((At3Band *)ch)[band].spec;
    int *end = ((At3Band *)ch)[band].ov;
    unsigned int sh;
    unsigned int step;

    if (band >= 4) {
        return -1;
    }
    do {
        sh = g[1];
        if (end < ((At3Band *)ch)[band].spec + g[0] + 1) {
            return -1;
        }
        if (!(p < ((At3Band *)ch)[band].spec + g[0] + 1)) {
            return -1;
        }
        if (sh >= 16) {
            return -1;
        }
        do {
            *p = *p >> sh;
            p++;
        } while (p < ((At3Band *)ch)[band].spec + g[0] + 1);
        if (!(p < end)) {
            break;
        }
        g += 2;
        step = g[1] - sh;
        sh <<= 3;
        sh += step;
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
        GAINC_STEP();
    } while (p < end);
    return 0;
}

void iqmf(char *ch)
{
    float tmp[4][140];
    float *s;
    float *t;
    float *q;
    int i;

    q = (float *)(ch + 0x2ABC);
    s = (float *)ch;
    t = tmp[0];
    for (i = 0; i < 0x118; i += 8) {
        h_d_make(s, s + 408, t, t + 280);
        h_d_make(s + 1224, s + 816, t + 140, t + 420);
        s += 8;
        t += 4;
    }
    t = tmp[0];
    s = tmp[2];
    do {
        h_iqmf_fb1(q++, t, 0);
        t += 4;
        h_iqmf_fb1(q, s, 1);
        s += 4;
        q += 15;
    } while (q < (float *)(ch + 0x32BC));
    q = (float *)(ch + 0x2A60);
    do {
        h_iqmf_fb2(q);
        q += 4;
    } while (q < (float *)(ch + 0x3260));
}

static unsigned int get_unpack_from_msb(int n, At3Work *w)
{
    int pos = w->bitpos;
    unsigned char *p = w->stream + (pos >> 3);
    int mask = (1 << n) - 1;

    n += pos & 7;
    return (((p[0] << 8) | p[1]) >> (16 - n)) & mask;
}

static unsigned int read_unpack_from_msb(int n, At3Work *w)
{
    unsigned int v = get_unpack_from_msb(n, w);

    w->bitpos += n;
    return v;
}

static int unpack_specs(char *ch, unsigned int sel, int cb, int start, int end, int q, At3Work *w)
{
    static const int scaleDiv[64] = {
        21, 21, 21, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16,
        16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11,
        11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6,
        5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0,
    };
    int bits = huf_maxlen[cb - 1][sel];
    signed char *len = huf_lentbl[cb - 1][sel];
    signed char *prefix = huf_prefixtbl[cb - 1][sel];
    int shift;
    int *deq;
    int *p;
    int c;
    int c2;

    if (sel >= 2) {
        return -1;
    }
    if ((unsigned int)(cb - 1) >= 7) {
        return -1;
    }
    if (end == start) {
        return -1;
    }
    shift = scaleDiv[q];
    p = (int *)(ch + 0x1A40) + start;
    deq = huf_deqtbl[q - (21 - shift) * 3][cb - 1];
    end -= start;
    if (cb == 1) {
        do {
            c = prefix[get_unpack_from_msb(bits, w)];
            w->bitpos += len[c];
            *p++ += deq[c * 2] >> shift;
            if (end != 1) {
                *p++ += deq[c * 2 + 1] >> shift;
            }
            end -= 2;
        } while (end > 0);
    } else {
        do {
            c2 = prefix[get_unpack_from_msb(bits, w)];
            end--;
            w->bitpos += len[c2];
            *p++ += deq[c2] >> shift;
        } while (end > 0);
    }
    return 0;
}

static int getToneSpectrum(char *ch, int n, int nbands, At3Work *w)
{
    int mode;
    int len;
    int mask;
    int cb;
    int sel;
    int band;
    int pos;
    int cnt;
    int q;
    int start;
    int end;

    mode = read_unpack_from_msb(2, w);
    if (mode == 2) {
        return -1;
    }
    do {
        mask = get_unpack_from_msb(4, w);
        w->bitpos += nbands;
        len = read_unpack_from_msb(3, w) + 1;
        cb = read_unpack_from_msb(3, w);
        if (cb < 2) {
            return -1;
        }
        sel = mode;
        if (mode == 3) {
            sel = read_unpack_from_msb(1, w);
        }
        band = 0;
        do {
            if (mask & (8 >> band)) {
                for (pos = 0; pos < 256; pos += 64) {
                    for (cnt = read_unpack_from_msb(3, w) - 1; cnt >= 0; cnt--) {
                        q = read_unpack_from_msb(6, w);
                        if (w->qoff > 0) {
                            q = (q > w->qoff) ? q - w->qoff : 0;
                        }
                        start = read_unpack_from_msb(6, w) + (band << 8) + pos;
                        end = len + start;
                        if (end > 1024) {
                            end = 1024;
                        }
                        if (w->maxpos < end) {
                            w->maxpos = end;
                        }
                        if (unpack_specs(ch, sel, cb, start, end, q, w) == -1) {
                            return -1;
                        }
                    }
                }
            }
        } while (++band < nbands);
    } while (--n > 0);
    return 0;
}

static int getNoiseSpectrum(char *ch, int nqus, At3Work *w)
{
    int cb[32];
    int q[32];
    int sel;
    int iqt;
    int v;

    if ((unsigned int)(nqus - 1) >= 32) {
        return -1;
    }
    sel = read_unpack_from_msb(1, w);
    iqt = 0;
    do {
        cb[iqt] = read_unpack_from_msb(3, w);
    } while (++iqt < nqus);
    iqt = 0;
    do {
        if (cb[iqt]) {
            v = read_unpack_from_msb(6, w);
            q[iqt] = v;
            if (w->qoff > 0) {
                if (v > w->qoff) {
                    q[iqt] = v - w->qoff;
                } else {
                    q[iqt] = 0;
                }
            }
        }
    } while (++iqt < nqus);
    iqt = 0;
    do {
        if (cb[iqt]) {
#line 1057 "libat3dec.c"
            if (unpack_specs(ch, sel, cb[iqt], QTSTART(iqt), QTSTART(iqt+1), q[iqt], w) == -1) {
                return -1;
            }
        }
    } while (++iqt < nqus);
#line 1063 "libat3dec.c"
    if (w->maxpos < QTSTART(iqt)) {
        w->maxpos = QTSTART(iqt);
    }
    return 0;
}

/* Parked: 5 lines off: the band address of the loop is (x << 4) + x with swapped registers */
#if 0
static void f2t_nodata(At3Band *bands, int band)
{
    int i;
    int v;
    int *p;
    int *q;

    memcpy(&bands[band].hdr, &bands[band].save, sizeof(At3Blk23));
    q = &bands[band].spec[127];
    p = bands[band].ov;
    for (i = 0; i < 128; i++) {
        v = *p;
        *q-- = v >> 4;
        *p = 0;
        p[-128] = ((v >> 14) * a_wind[i]) >> 5;
        p++;
    }
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", f2t_nodata);
#endif

/* Parked: structure right; the gain loops keep &gain[0][0][1] in a register and reload 255 (ours hoists it) */
#if 0
static int unpack_mddata(At3Chan *ch, int rev, At3Work *w)
{
    int gain[4][8][2];
    int jw[4];
    int jg;
    int nbands;
    int band;
    int n;
    int nb;
    int i;
    int *gp;

    jg = 0;
    if (rev) {
        jg = read_unpack_from_msb(4, w);
        for (band = 0; band < 4; band++) {
            jw[band] = read_unpack_from_msb(2, w);
        }
        w->bitpos += 2;
    } else {
        w->bitpos += 6;
    }
    nbands = read_unpack_from_msb(2, w) + 1;
    gp = &gain[0][0][1];
    band = 0;
    do {
        n = read_unpack_from_msb(3, w);
        for (i = 0; i < n; i++) {
            gp[band * 16 + i * 2] = read_unpack_from_msb(4, w);
            gain[band][i][0] = read_unpack_from_msb(5, w) << 3;
        }
        gp[band * 16 + i * 2] = 4;
        gain[band][i][0] = 0xFF;
    } while (++band < nbands);
    for (; band < 4; band++) {
        int *lev = &gain[0][0][1];
        lev[band * 16] = 4;
        gain[band][0][0] = 0xFF;
    }
    memcpy(&ch->qmfhead, &ch->qmfsave, sizeof(At3Blk23));
    memset(ch->spec, 0, sizeof(ch->spec));
    w->maxpos = nbands << 8;
    n = read_unpack_from_msb(5, w);
    if (n && getToneSpectrum((char *)ch, n, nbands, w) == -1) {
        return -1;
    }
    if (getNoiseSpectrum((char *)ch, read_unpack_from_msb(5, w) + 1, w) == -1) {
        return -1;
    }
    nb = nbands;
    if (nbands << 8 < w->maxpos) {
        nb = (w->maxpos + 0xF7) / 256;
    }
    band = 0;
    do {
        if (imdct((char *)ch, band, gp[band * 16])) {
            f2t_nodata(ch->band, band);
        }
        if (gainc_window((char *)ch, band) == -1) {
            return -1;
        }
    } while (++band < nb);
    for (; band < 4; band++) {
        f2t_nodata(ch->band, band);
        if (gainc_window((char *)ch, band) == -1) {
            return -1;
        }
    }
    if (rev) {
        channel_conv_joint(w);
        w->joint[1] = w->joint[0];
        w->joint[0].n = nbands;
        w->joint[0].g = chwtTable[jg];
        for (band = 0; band < 4; band++) {
            w->joint[0].w[band] = chswTable[jw[band]];
        }
    }
    memcpy(ch->gain, gain, sizeof(gain));
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", unpack_mddata);
#endif

/* Parked: 11 lines off (20 before the 0xF8 pad byte in a variable, decomp-permuter): the original enters the copy loop
   with the byte the skip loop loaded (reorg), registers differ */
#if 0
static inline int _before(unsigned char *a, unsigned char *b)
{
    return a < b;
}

int at3dec_proc(At3Work *w)
{
    int off = 0;
    int i;
    int rev;
    unsigned char *src;
    unsigned char *p;
    unsigned char *d;
    unsigned char pad;

    for (i = 0; i < w->nch; i++) {
        rev = 0;
        if (w->mode == 0x12) {
            rev = i == 1;
        }
        w->bitpos = off * 8;
        if (rev) {
            src = w->stream;
            d = w->buf;
            p = src + w->frameSize + off;
            while (*--p == 0xF8 && src < p) {
            }
            while (src < p) {
                *d++ = *p--;
            }
            w->stream = w->buf;
            w->bitpos = 0;
        }
        if (unpack_mddata((At3Chan *)w->ch[i], rev, w) == -1) {
            return -1;
        }
        off += w->frameSize;
        if (w->mode != 0x12 && off * 8 < w->bitpos) {
            return -1;
        }
    }
    for (i = 0; i < w->nch; i++) {
        iqmf(w->ch[i]);
    }
    return 0;
}
#else
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/lib/libat3dec", at3dec_proc);
#endif
// SOURCE DATA: .data .rodata
