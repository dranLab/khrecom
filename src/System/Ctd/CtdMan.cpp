#include "System/Graphic/Gprim.h"
#include "System/Ctd/CtdMan.h"
#include "System/SJoint.h"
#include "System/Debug.h"
#include "System/HeapMan.h"
extern "C" int sprintf(char*, const char*, ...);




class CCtd : public CSJointT<CCtd> {
public:
    INT m_id;
    INT m_nBlock;
    INT m_nText;
    CHAR** m_ppText;
    CtdBlock* m_pCtdBlock;
};

class CCtdList : public CSJointListT<CCtd> {
public:
    ~CCtdList() {
        for (CCtd *__p = getHead(), *__pn = __p ? __p->getNext() : NULL; __p != NULL;
             __p = __pn, __pn = __pn ? __pn->getNext() : NULL) {
            delete __p;
        }
        resetParam();
    }
};

/* Accessing m_pCtdList through a reference keeps it ordered after stores through other pointers, as in the
 * original. */
template <class T> static inline T& _ref(T& v) { return v; }

CCtdMan* CCtdMan::m_pInstance = NULL;
CCtdList* CCtdMan::m_pCtdList = NULL;

CCtdMan::CCtdMan() {}

CCtdMan::~CCtdMan() {
    delete _ref(m_pCtdList);
    m_pCtdList = NULL;
}

void CCtdMan::setup() {
    if (m_pCtdList == NULL) {
#line 37
        m_pCtdList = new (__FILE__ "(37)") CCtdList;
    }
}

void CCtdMan::dispose() {}

void CCtdMan::initializeData(void* pDtHead) {
    CtdHeader* pHead = (CtdHeader*)pDtHead;
    if (*(UINT*)pHead->sig == 0x44544340) {
#line 57
        CCtd* pCtd = new (__FILE__ "(57)") CCtd;
        CtdBlock* pCtdBlock = (CtdBlock*)(pHead + 1);
        pCtd->m_nBlock = pHead->nBlock;
        pCtd->m_nText = pHead->nText;
        pCtd->m_pCtdBlock = pCtdBlock;
        CHAR** ppText0 = (CHAR**)((UINT)pDtHead + pHead->textAddrArea);
        pCtd->m_ppText = ppText0;
        CHAR** ppText = ppText0;
        CHAR* pText = (CHAR*)(pHead->textDataArea + (UINT)pDtHead);
        INT i;
        for (i = 0; i < pCtd->m_nText; i++, ppText++) {
            *ppText = pText + (UINT)*ppText;
        }
        INT idx = 0;
        for (i = 0; i < pCtd->m_nBlock; i++, pCtdBlock++) {
            pCtdBlock->ppText = ppText0 + idx;
            idx += pCtdBlock->nText;
        }
        pCtd->m_id = idx = pCtd->m_pCtdBlock->id & 0xFFFF0000;
        _ref(m_pCtdList)->pushBack(*pCtd);
        TRACE("CTD\x83\x66\x81\x5B\x83\x5E:%02d-%03d-%1d \x82\xF0\x93\x6F\x98\x5E\n", idx >> 26, (idx >> 19) & 0x7F, (idx >> 16) & 0x7);
    }
}

void CCtdMan::disposeData(void* pDtHead) {
    INT idx = ((CtdBlock*)((CtdHeader*)pDtHead + 1))->id & 0xFFFF0000;
    INT listSize = m_pCtdList->getListSize();
    if (listSize != 0) {
#line 103
        CCtd** pCtdList = (CCtd**)com_malloc_d(listSize * sizeof(CCtd*), __FILE__ "(103)", RET_ADDR());
        INT i = 0;
        for (CCtd *pCtd = m_pCtdList->getHead(), *__pn = pCtd ? pCtd->getNext() : NULL; pCtd != NULL;
             pCtd = __pn, __pn = __pn ? __pn->getNext() : NULL) {
            pCtdList[i++] = pCtd;
        }
        for (i = listSize - 1; i >= 0; i--) {
            CCtd* pCtd = pCtdList[i];
            if (pCtd->m_id == idx) {
                m_pCtdList->remove(*pCtd);
                TRACE("CTD\x83\x66\x81\x5B\x83\x5E:%02d-%03d-%1d \x82\xF0\x8D\xED\x8F\x9C\n", idx >> 26,
                      (idx >> 19) & 0x7F, (idx >> 16) & 0x7);
                delete pCtd;
                break;
            }
        }
        if (pCtdList != NULL) com_free(pCtdList);
    }
}

CHAR* CCtdMan::_getText(UINT ctdID) {
    INT idf = ctdID & 0xFFFF0000;
    UINT idb = ctdID & 0xFFFFFF00;
    UINT idt = ctdID & 0xFF;
    for (CCtd *pCtd = m_pCtdList->getHead(), *__pn = pCtd ? pCtd->getNext() : NULL; pCtd != NULL;
         pCtd = __pn, __pn = __pn ? __pn->getNext() : NULL) {
        if (pCtd->m_id == idf) {
            CtdBlock* pCtdBlock = pCtd->m_pCtdBlock;
            for (INT j = 0; j < pCtd->m_nBlock; j++, pCtdBlock++) {
                if (pCtdBlock->id == idb && idt < pCtdBlock->nText) {
                    return pCtdBlock->ppText[idt];
                }
            }
        }
    }
    return NULL;
}

CtdBlock* CCtdMan::_getCtdBlock(UINT ctdID, CHAR** pOutText) {
    INT idf = ctdID & 0xFFFF0000;
    UINT idb = ctdID & 0xFFFFFF00;
    for (CCtd *pCtd = m_pCtdList->getHead(), *__pn = pCtd ? pCtd->getNext() : NULL; pCtd != NULL;
         pCtd = __pn, __pn = __pn ? __pn->getNext() : NULL) {
        if (pCtd->m_id == idf) {
            CtdBlock* pCtdBlock = pCtd->m_pCtdBlock;
            for (INT j = 0; j < pCtd->m_nBlock; j++, pCtdBlock++) {
                if (pCtdBlock->id == idb) {
                    if (pOutText != NULL) {
                        INT idt = ctdID & 0xFF;
                        if (idt >= pCtdBlock->nText) {
                            *pOutText = NULL;
                        } else {
                            *pOutText = pCtdBlock->ppText[idt];
                        }
                    }
                    return pCtdBlock;
                }
            }
        }
    }
    return NULL;
}

INT CCtdMan::_sprintf(LPSTR inBuf, UINT inBufSize, UINT inSrcCtdID, UINT (&ctdID)[4]) {
    return _sprintf(inBuf, inBufSize, _getText(inSrcCtdID), ctdID);
}

INT CCtdMan::_sprintf(LPSTR inBuf, UINT inBufSize, const CHAR* inSrcStr, UINT (&ctdID)[4]) {
    return 0;
}

INT CCtdMan::_sprintf(LPSTR inBuf, UINT inBufSize, const CHAR* inSrcStr, const CHAR* (&pWord)[4], CHAR** outBuf) {
    if (inSrcStr == NULL) return 0;
    INT fgCtrl = FALSE;
    const CHAR* pPercent = strchr(inSrcStr, '%');
    LPSTR pDstText = NULL;
    INT nWordCnt = 0;
    if (pPercent != NULL) {
        INT nCtrlCordCnt = 1;
        const CHAR* pText = pPercent + 1;
        INT fgEnd = FALSE;
        const CHAR** ppWord = pWord;
        const CHAR* pSrcText = inSrcStr;
        INT fgCtrlCode = 0;
        do {
            const CHAR* pW0;
            INT lenWord;
            char wordBuf[128];
            switch (*pText) {
            case 'S':
            case 'a':
            case 's':
                if (nCtrlCordCnt == 0) break;
                nCtrlCordCnt++;
                if (*pText == 'S') {
                    fgCtrlCode |= 1;
                } else if (*pText == 'a') {
                    fgCtrlCode |= 2;
                }
                pW0 = *ppWord;
                if ((UINT)pW0 & 0x80000000) {
                    fgCtrl = TRUE;
                    fgEnd = TRUE;
                    break;
                }
                lenWord = strlen(pW0);
                {
                    LPSTR pW1 = strchr(pW0, '\n');
                    if (pW1 != NULL) {
                        pW1++;
                        LPSTR pW2 = strchr(pW1, '\n');
                        if (pW2 != NULL) {
                            INT lenW;
                            pW2++;
                            if ((fgCtrlCode & 3) == 0) {
                                lenW = pW1 - pW0 - 1;
                                strncpy(wordBuf, pW0, lenW);
                            } else if (fgCtrlCode & 1) {
                                lenW = pW2 - pW1 - 1;
                                strncpy(wordBuf, pW1, lenW);
                            } else {
                                lenW = lenWord - (pW2 - pW0);
                                strncpy(wordBuf, pW2, lenW);
                            }
                            wordBuf[lenW] = '\0';
                            if (fgCtrlCode < 0) {
                                if (wordBuf[0] >= 'a') {
                                    if (wordBuf[0] < '{') wordBuf[0] -= 0x20;
                                }
                            }
                            pW0 = wordBuf;
                            lenWord = strlen(wordBuf);
                        }
                    }
                }
            replace:
                {
                    INT lenText = strlen(pSrcText);
                    INT lenText0 = pText - pSrcText - nCtrlCordCnt + 1;
                    INT lenText1 = lenText - lenText0 - nCtrlCordCnt;
                    lenWord = strlen(pW0);
                    INT strSize = lenText0 + lenWord + lenText1 + 1;
#line 334
                    LPSTR pDstTmp = (LPSTR)com_malloc_d(strSize, __FILE__ "(334)", (UINT)-1);
                    pDstTmp[strSize - 1] = '\0';
                    strncpy(pDstTmp, pSrcText, lenText0);
                    strncpy(pDstTmp + lenText0, pW0, lenWord);
                    pDstText = pDstTmp;
                    strncpy(pDstTmp + lenText0 + lenWord, pSrcText + lenText0 + nCtrlCordCnt, lenText1);
                    if (nWordCnt != 0 && pSrcText != NULL) com_free((void*)pSrcText);
                    pSrcText = pDstTmp;
                    pText = pSrcText + lenText0 + lenWord - 1;
                    nWordCnt++;
                    fgCtrl = TRUE;
                    nCtrlCordCnt = 0;
                    if (nWordCnt >= 4) fgEnd = TRUE;
                    ppWord++;
                }
                break;
            case 'x':
                if (nCtrlCordCnt == 0) break;
                nCtrlCordCnt++;
                if (!((UINT)*ppWord & 0x80000000)) {
                    fgCtrl = TRUE;
                    fgEnd = TRUE;
                    break;
                }
                ::sprintf(wordBuf, "%d", (UINT)*ppWord & 0x7FFFFFFF);
                pW0 = wordBuf;
                goto replace;
            case '%':
                nCtrlCordCnt = 1;
                fgCtrlCode = 0;
                break;
            case '#':
                if (nCtrlCordCnt != 0) {
                    nCtrlCordCnt++;
                    fgCtrlCode |= 0x80000000;
                }
                break;
            case '\0':
                fgEnd = TRUE;
            default:
                nCtrlCordCnt = 0;
                break;
            }
            pText++;
        } while (!fgEnd);
    }
    {
        LPSTR pText = fgCtrl ? pDstText : (LPSTR)inSrcStr;
        if (pText == NULL) return 0;
        INT num = strlen(pText);
        if (outBuf == NULL) {
            if (strlen(pText) < inBufSize) {
                strcpy(inBuf, pText);
            } else {
                num = 0;
            }
            if (fgCtrl && pText != NULL) com_free(pText);
        } else {
            *outBuf = pText;
        }
        return num;
    }
}

// SOURCE DATA: .data .rodata
