#ifndef SYSTEM_RSRCDB_H
#define SYSTEM_RSRCDB_H

#include "khtypes.h"
#include "System/Task.h"
#include "System/RsrcData.h"

class CBuf;
class CRsrcData;
class CRsrcList;
class CRsrcANN;
class CMotData;
struct TexInfo;
struct MODELDATA;
struct ANIMDATA;
struct EFFECT_DATA;
struct EFFECT_SE;
struct TEXANM_DATA;
struct MEP_DATA;

typedef char SBYTE;

#ifdef RSRCDATA_H_FULL
struct PackedRsrcHeader {
    void* _pBody;
    char _szName[22];
    BYTE _cType;
    BYTE _cPram;
    INT _cSize;

    static void coodinateVersion(void* pHead);
};

class CRsrcList : public CSLCListT<CRsrcData> {
public:
    CRsrcList(ISLCManagerT<CRsrcData>& inManager) : CSLCListT<CRsrcData>(inManager) {}
    virtual ~CRsrcList() {
        for (SLContainerT<CRsrcData>* __p = getHead(); __p; __p = __p->getNext()) {
            if (__p->getValue()) {
                delete __p->getValue();
                __p->setValue(NULL);
            }
        }
        removeAll();
    }
};

class CRsrcVSV : public CRsrcData {
private:
    UINT m_nStartSect; /* 0x48 */
    UINT m_nSectSize;  /* 0x4c */
    UINT m_nBgmID;     /* 0x50 */

    void _setupVSV();

public:
    CRsrcVSV(const RsrcInfo& info, UINT nStartSect, UINT nSectSize) : CRsrcData(info) {
        m_nStartSect = nStartSect;
        m_nSectSize = nSectSize;
        _setupVSV();
    }
};

class CRsrcIMZ : public CRsrcData {
public:
    CRsrcIMZ(const RsrcInfo& info, CBuf& data) : CRsrcData(info, data) {}
};

#endif /* RSRCDATA_H_FULL */

class CRsrcDB {
public:
    static void initialize();
    static void finalize();
    static INT addRsrc(const RsrcInfo& info, CBuf& data, CRsrcData** ppOutRsrc);
    static INT addRsrc_NoCheck(const RsrcInfo& info, CBuf& data, CRsrcData** ppOutRsrc);
    static INT addPackedRsrc(const RsrcInfo& info, CBuf& data, CRsrcData* pParent, BOOL bFragment);
    static INT addPackedRsrcIMZ(const RsrcInfo& info, CBuf& data, CRsrcData* pParent);
    static void addRsrcTag(CRsrcData& data, INT nGroupID, BOOL bSceneManaged);
    static void removeRsrc(INT nID);
    static void removeRsrcForce(INT nID);
    static void removeRsrcWithType(INT nID, INT nRsrc);
    static void removeRsrcWithTypeForce(INT nID, INT nRsrc);
    static void removeGroup(INT nGroupID, INT nMemberID);
    static void removeGroupForce(INT nGroupID, INT nMemberID);
    static void removeRsrc(CRsrcData& rsrc);
    static void removeSceneAttachedData(UINT nNum);
    static void addStreamInfo(const CHAR* pszFile, INT nGroupID, UINT nStartSect, UINT nSectSize, INT nRsrcType);
    static void addRsrcType(const CHAR* pszExt, INT nID);
    static INT getRsrcType(const CHAR* pszExt);
    static UINT getRsrcAlignmentSize(INT nRsrcType);
    static BOOL isSceneAttachMode();
    static UINT getSceneAttachNum();
    static void setSceneAttachMode(UINT nNum);
    static void clearSceneAttachMode();
    static void resetRsrc(INT nRsrcType, INT nArg);
    static CRsrcList& getRsrcList(UINT nRsrcType);
    static CRsrcData* getRsrc(const CHAR* pszName, NAME_CASE nc);
    static CRsrcData* getRsrc(INT nRsrcType, const CHAR* pszName, NAME_CASE nc);
    static CRsrcData* getRsrc(HANDLE hRsrc);
    static CRsrcData* getRsrcWithHash(INT nRsrcType, INT nNameHash);
    static CRsrcData* getRsrcWithIndex(INT nRsrcType, INT nIndex, BOOL bOnlyCurrentScene);
    static void* getRsrcData(const CRsrcData& data);
    static CRsrcData* getFirstRsrc(INT nRsrcType);
    static void* getFirstData(INT nRsrcType);
    static CRsrcData* getFirstRsrc(INT nRsrcType, INT nGroupID, BOOL bFollowLink);
    static void* getFirstData(INT nRsrcType, INT nGroupID, BOOL bFollowLink);
    static CRsrcData* getFirstRsrc_Scene(INT nRsrcType);
    static void* getFirstData_Scene(INT nRsrcType);
    static CRsrcData* getFirstRsrc_Scene(INT nRsrcType, INT nGroupID);
    static void* getFirstData_Scene(INT nRsrcType, INT nGroupID);
    static void initSeekRsrc(INT nRsrcType, INT nGroupID);
    static CRsrcData* seekNextRsrc();
    static CRsrcData* seekNextRsrc_Opt();
    static CRsrcData* seekNextRsrc_Scene();
    static CRsrcData* seekNextRsrc_RcOpt();
    static INT checkGroupID(INT nGroupOfs, INT nGroupID);
    static UINT getRsrcGrpSize(INT nGroupID, INT nMemberID);
    static UINT getRsrcGrpSizeOnMemory(INT nGroupID, INT nMemberID);
    static BOOL hasRsrcGroup(INT nGroupID, INT nMemberID);
    static CRsrcList& getScrollTexList();
    static TexInfo* getRsrcTex(const CHAR* pszName, NAME_CASE nc);
    static MODELDATA** getRsrcMDL(const CHAR* pszName, NAME_CASE nc);
    static MODELDATA** getRsrcMDL(INT nGroup);
    static MODELDATA** getRsrcSMD(const CHAR* pszName);
    static MODELDATA** getRsrcSMD(INT nGroup);
    static MODELDATA* getRsrcAMF(const CHAR* pszName, NAME_CASE nc);
    static ANIMDATA** getRsrcANM(const CHAR* pszName, NAME_CASE nc);
    static BYTE* getRsrcSPD(const CHAR* pszName);
    static BYTE* getRsrcSQD(const CHAR* pszName);
    static BYTE* getRsrcLAD(const CHAR* pszName);
    static CBuf* getRsrcBIN(const CHAR* pszName, NAME_CASE nc);
    static CBuf* getRsrcABC(INT nAtelNo);
    static CBuf* getRsrcMCD(INT nMapRsrcGrpID);
    static BYTE* getRsrcOCD(INT nGroup);
    static EFFECT_DATA* getRsrcDPX(const CHAR* pszName);
    static SBYTE* getRsrcDPD(const CHAR* pszName);
    static TEXANM_DATA* getRsrcTXA(INT nGroup);
    static EFFECT_SE* getRsrcESE(const CHAR* pszName);
    static BYTE* getRsrcEET(const CHAR* pszName);
    static void* getRsrcCAM(const CHAR* pszName);
    static MEP_DATA* getRsrcMEP(INT nGroup);
    static BYTE* getRsrcPTD(INT nPtdNo);
    static BYTE* getRsrcPTD(const CHAR* pszName);
    static CHAR* getRsrcPTDName(INT nPtdNo);
    static BYTE* getRsrcSES(INT nGroup);
    static BYTE* getRsrcMMP(INT nGroup, INT nPtdNo);
    static BYTE* getRsrcMMP(INT nGroup, const CHAR* pszName);
    static CHAR* getRsrcMMPName(INT nGroup, INT nPtdNo);
    static BYTE* getRsrcCTD(INT nGroup, INT nPtdNo);
    static BYTE* getRsrcCTD(INT nGroup, const CHAR* pszName);
    static CHAR* getRsrcCTDName(INT nGroup, INT nPtdNo);
    static INT getModelCount(const CHAR* pszName, NAME_CASE nc);
    static INT getModelCount(INT nGroup);
    static INT getSModelCount(const CHAR* pszName);
    static INT getSModelCount(INT nGroup);
    static INT getAnimeCount(const CHAR* pszName, NAME_CASE nc);
    static INT getAnimeCount(INT nGroup);
    static INT getMotionCount(const CHAR* pszName, NAME_CASE nc);
    static CMotData* getMotionData(const CHAR* pszName, INT nMotNo, NAME_CASE nc);
    static CRsrcANN* getRsrcANN(INT nGroup);
    static CRsrcANN* getRsrcANN(const CHAR* pszName, NAME_CASE nc);
    static INT getMotionIdxFromMotName(const CHAR* pszName, const CHAR* pszMotName, NAME_CASE nc);
};

#endif /* SYSTEM_RSRCDB_H */
