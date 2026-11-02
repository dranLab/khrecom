#ifndef SYSTEM_RSRCDB_H
#define SYSTEM_RSRCDB_H

#include "khtypes.h"

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

enum NAME_CASE {
    NAME_CASE_SENSITIVE = 0,
    NAME_CASE_UPPER = 1,
    NAME_CASE_LOWER = 2
};

struct RsrcInfo;


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
    static void* getRsrcData(INT nRsrcType, const CHAR* pszName) { // MATCH: inlined in Form/Menu/Camp; the ref lists one getRsrcData (this overload's name unknown)
        CRsrcData* pData = getRsrc(nRsrcType, pszName, NAME_CASE_SENSITIVE);
        return pData ? getRsrcData(*pData) : NULL;
    }
    static CRsrcData* getFirstRsrc(INT nRsrcType);
    static void* getFirstData(INT nRsrcType);
    static CRsrcData* getFirstRsrc(INT nRsrcType, INT nGroupID, BOOL bFollowLink);
    static void* getFirstData(INT nRsrcType, INT nGroupID, BOOL bFollowLink);
    static CRsrcData* getFirstRsrc_Scene(INT nRsrcType);
    static void* getFirstData_Scene(INT nRsrcType);
    static CRsrcData* getFirstRsrc_Scene(INT nRsrcType, INT nGroupID);
    static void* getFirstData_Scene(INT nRsrcType, INT nGroupID);
    static BOOL isDataExist(/* parameters unknown */);
    static void initSeekRsrc(INT nRsrcType, INT nGroupID);
    static CRsrcData* seekNextRsrc();
    static CRsrcData* seekNextRsrc_Opt();
    static CRsrcData* seekNextRsrc_Scene();
    static CRsrcData* seekNextRsrc_RcOpt();
    static void* seekNextData(/* parameters unknown */);
    static void* seekNextData_Scene(/* parameters unknown */);
    static void* seekNextData_Opt(/* parameters unknown */);
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
