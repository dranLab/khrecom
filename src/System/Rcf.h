#ifndef SYSTEM_RCF_H
#define SYSTEM_RCF_H

#include "khtypes.h"
#include "System/SJoint.h"

struct RcfKey {
    SHORT _nGroup;
    SHORT _nMember;
};

struct RcfSceneInfo : public RcfKey, public CSJointT<RcfSceneInfo> {
    char _szTitle[128];
    USHORT _nAtelCount;
    SHORT _nRcfMapIdx;
};

typedef CSJointListT<RcfSceneInfo> RcfSceneInfoList;

struct RcfMapInfo : public RcfKey {
    char _szTitle[128];
    RcfSceneInfoList _SceneList;
};

class CRcf {
public:
    static void initialize();
    static void finalize();
    static BOOL isTerminated();
    static void mainProc();
    static void reqLoadRCF(INT nGroup, INT nMember, INT nType, BYTE* pBuff, INT nLinkOwner);
    static void reqLoadRCFFront(INT nGroup, INT nMember, INT nType, BYTE* pBuff, INT nLinkOwner);
    static void reqLoadPlayer(INT nID);
    static void reqLoadScene(INT nArea, INT nScene);
    static void reqLoadStage(INT nArea, INT nScene);
    static void reqLoadEnemy(INT nInEnemyID);
    static void reqLoadNpc(INT nInNpcID);
    static void reqLoadStageMap(INT nArea, INT nScene);
    static void cancelLoadRCF(INT nGroup, INT nMember);
    static BOOL isCompleteLoadRCF(INT nGroup, INT nMember);
    static BOOL isCompleteLoadEnemy(INT nInEnemyID);
    static BOOL isCompleteLoadNpc(INT nInNpcID);
    static void setLocale(INT nID);
    static INT getLocale();
    static INT getMapID(SHORT nArea, SHORT nScene);
    static SHORT getBattleScene(SHORT nArea, SHORT nScene);
    static INT getMemberOption(ULONG nGroup);
    static int isSceneManagedMode();
    static void initRsrcFileSize();
    static INT getRsrcFileSize(ULONG nID);
    static INT getCdStatus();
    static void prepareLoadCategoryInfo();
    static INT loadNextCategoryInfo(INT nCategory, CHAR* pOutLabel, INT nLabelMax, INT* pOutMemberID);
    static RcfSceneInfo* getSceneInfo(SHORT nIndex);
    static RcfSceneInfo* getSceneInfo(SHORT nArea, SHORT nScene);
    static SHORT getSceneInfoIndex(SHORT nArea, SHORT nScene);
    static SHORT getSceneInfoCount();
    static RcfMapInfo* getMapInfo(SHORT nIndex);
    static RcfMapInfo* getMapInfo(SHORT nGroup, SHORT nMember);
    static SHORT getMapInfoIndex(SHORT nGroup, SHORT nMember);
    static SHORT getMapInfoCount();

private:
    static void _threadReadProc(void* p);
    static BOOL _launchReadThread(ULONG nData, INT nType);
    static INT _loadRCFAlias();
};

#endif /* SYSTEM_RCF_H */
