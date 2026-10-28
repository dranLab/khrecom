#define TASK_H_NO_SLC_OVERFLOW_MSG
#define GPRIM_H_PATH "System/Graphic/GPrim.h"
#include "System/Graphic/Gprim.h"
#include "System/Graphic/2d/2DMan.h"
#include "Game/Technic.h"
#include "Form/Menu/TechHelp.h"

struct TexInfo;

enum NAME_CASE {
    NAME_CASE_SENSITIVE = 0,
    NAME_CASE_UPPER = 1,
    NAME_CASE_LOWER = 2
};

namespace D2 {
class C2DMan {
public:
    static HANDLE create(const char* pName, UINT nIdx);
    static void destroy(HANDLE h);
    static void setLayer(INT nLayer);
    static BOOL isEnd(HANDLE h);
};
}

class CRsrcDB {
public:
    static void removeGroup(INT nGroup, INT nID);
    static TexInfo* getRsrcTex(const char* pName, NAME_CASE eCase);
};

class CRcf {
public:
    static void reqLoadRCFFront(INT nGroup, INT nID, INT nFlag, unsigned char* pBuf, INT nSize);
    static BOOL isCompleteLoadRCF(INT nGroup, INT nID);
};

#include "System/Sound/SoundMan.h"
#line 42

BOOL CTechHelp::m_bEnd = 0;

CTechHelp::CTechHelp(CTask* p, USHORT nTeckKind) : CTask(p, 0x4000) {
    unsigned char techRsrcTbl[101][2] = {
        {0x04, 0x06}, {0x05, 0x00}, {0x06, 0x07}, {0x07, 0x08}, {0x08, 0x03}, {0x09, 0x01}, {0x0A, 0x02},
        {0x0B, 0x04}, {0x0C, 0x05}, {0x0D, 0x09}, {0x0E, 0x0B}, {0x0F, 0x0D}, {0x10, 0x0F}, {0x11, 0x11},
        {0x12, 0x13}, {0x13, 0x15}, {0x14, 0x0A}, {0x15, 0x0C}, {0x16, 0x0E}, {0x17, 0x10}, {0x18, 0x12},
        {0x19, 0x14}, {0x1A, 0x16}, {0x1B, 0x40}, {0x1C, 0x41}, {0x1D, 0x42}, {0x1E, 0x3F}, {0x1F, 0x3E},
        {0x20, 0x46}, {0x21, 0x47}, {0x22, 0x4B}, {0x23, 0x45}, {0x24, 0x4D}, {0x25, 0x43}, {0x26, 0x48},
        {0x27, 0x44}, {0x28, 0x4C}, {0x29, 0x50}, {0x2A, 0x51}, {0x2B, 0x3B}, {0x2C, 0x3A}, {0x2D, 0x4F},
        {0x2E, 0x3D}, {0x2F, 0x3C}, {0x30, 0x4E}, {0x31, 0x4A}, {0x32, 0x52}, {0x33, 0x49}, {0x34, 0x17},
        {0x35, 0x18}, {0x36, 0x25}, {0x37, 0x26}, {0x38, 0x22}, {0x39, 0x23}, {0x3A, 0x24}, {0x3B, 0x1D},
        {0x3C, 0x1E}, {0x3D, 0x19}, {0x3E, 0x1A}, {0x3F, 0x1B}, {0x40, 0x1C}, {0x41, 0x1F}, {0x42, 0x21},
        {0x43, 0x20}, {0x44, 0x27}, {0x45, 0x28}, {0x46, 0x29}, {0x47, 0x2B}, {0x48, 0x2C}, {0x49, 0x2A},
        {0x4A, 0x2D}, {0x4B, 0x2E}, {0x4C, 0x2F}, {0x4D, 0x30}, {0x4E, 0x31}, {0x4F, 0x32}, {0x50, 0x33},
        {0x51, 0x34}, {0x52, 0x35}, {0x53, 0x36}, {0x54, 0x37}, {0x55, 0x38}, {0x56, 0x39}, {0x5F, 0x56},
        {0x60, 0x57}, {0x61, 0x58}, {0x62, 0x59}, {0x63, 0x5A}, {0x64, 0x5B}, {0x65, 0x5C}, {0x66, 0x5D},
        {0x67, 0x5F}, {0x68, 0x61}, {0x69, 0x5E}, {0x6A, 0x60}, {0x6B, 0x62}, {0x5A, 0x53}, {0x5B, 0x54},
        {0x5C, 0x55}, {0x6D, 0x63}, {0xFF, 0x00},
    };
    INT i;

    m_nStep = 0;
    m_bEnd = FALSE;
    m_hHelpLay = 0;
    for (i = 0; techRsrcTbl[i][0] != 0xFF; i++) {
        if (techRsrcTbl[i][0] == nTeckKind) {
            m_iRsecID = techRsrcTbl[i][1] + 300;
            break;
        }
    }
}

CTechHelp::~CTechHelp() {
    /* The original stores m_bEnd before loading m_hHelpLay (no reordering): volatile reproduces it. */
    *(volatile BOOL*)&m_bEnd = TRUE;
    D2::C2DMan::destroy(m_hHelpLay);
    if (m_iRsecID) {
        CRsrcDB::removeGroup(2, m_iRsecID);
    }
}

void CTechHelp::main() {
    static LPCTSTR pLadName = "help_stock.lad";
    static LPCTSTR pSeqName = "help_stock:00";

    switch (m_nStep) {
    case 0:
        if (!CRsrcDB::getRsrcTex(pSeqName, NAME_CASE_SENSITIVE)) {
            CRcf::reqLoadRCFFront(2, m_iRsecID, 0x100, 0, 0);
        }
        m_nStep = 1;
        break;
    case 1:
        if (CRcf::isCompleteLoadRCF(2, m_iRsecID)) {
            D2::C2DMan::setLayer(0x17);
            m_hHelpLay = D2::C2DMan::create(pLadName, 0);
            CSound::playSE(3, -1, 0x40);
            m_nStep = 2;
        }
        break;
    case 2:
        if (D2::C2DMan::isEnd(m_hHelpLay)) {
            D2::C2DMan::destroy(m_hHelpLay);
            D2::C2DMan::setLayer(0x17);
            m_hHelpLay = D2::C2DMan::create(pLadName, 1);
            m_nStep = 3;
        }
        break;
    case 3:
        if (isTrgOK() || isTrgCANCEL()) {
            D2::C2DMan::destroy(m_hHelpLay);
            D2::C2DMan::setLayer(0x17);
            m_hHelpLay = D2::C2DMan::create(pLadName, 2);
            CSound::playSE(4, -1, 0x40);
            m_nStep = 4;
        }
        break;
    case 4:
        if (D2::C2DMan::isEnd(m_hHelpLay)) {
            D2::C2DMan::destroy(m_hHelpLay);
            m_hHelpLay = 0;
            m_nStep = 5;
        }
        break;
    case 5:
    default:
        out();
        break;
    }
}

BOOL CTechHelp::isEnd() {
    return m_bEnd;
}
// SOURCE DATA: .data .rodata
