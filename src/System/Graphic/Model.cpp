#include "khtypes.h"
#include "System/Graphic/Model.h"
#include "include_asm.h"

class CShader;

CShader* createDummyShader() { return NULL; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __6CModelPC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_6CModel);
void CModel::resetWorldScreenMatrix() { m_pWSMatCache = NULL; }
void CModel::resetMicroProgram() { m_pMpg = NULL; }
void CModel::resetSendCtrl(INT nMode) { m_iSendFlg &= ~nMode; }
void CModel::setSendCtrl(INT nMode) { m_iSendFlg |= nMode; }
u_long128* CModel::getMicroProg() {
    if (m_pShader) return *(u_long128**)((BYTE*)m_pShader + 4);
    return NULL;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", searchShaderDesc__FUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", releaseData__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelP5CTaskPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelR7CCoordsPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBoneIndex__C9MODELDATAPCc);
INT CModel::getBoneIndex(const CHAR* pName) const {
    INT n = -1;
    if (m_pData) n = m_pData->getBoneIndex(pName);
    return n;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", AMFfromMDL__FPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", AMFfromMDL__Fii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelP5CTaskPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelR7CCoordsPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelP5CTaski);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelR7CCoordsi);
void CModel::setModelData(const MODELDATA* pData) {
    releaseData();
    m_pData = pData;
    link(m_pTask);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelP5CTask);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelR7CCoords);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getCrd__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getCoordNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setLocal__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getTextureIndex__C6CModelPCc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeTexture__6CModelPCcT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeTextureSub__6CModeliPC7TexInfo);
void CModel::setShadowSize(INT nSize) {
    if (m_Mode == 0x10 && m_pFadeModel) *(INT*)((BYTE*)m_pFadeModel + 0x28) = nSize;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getSemiZ__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getOldSemiZ__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getWorldBBoxScr__6CModelR7IVECTORPC7FMATRIX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getApplyMatrixBBox__FR7FVECTORT0RC7FMATRIXRC7FVECTORT3);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcWorldBBox__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getWorldBBox__6CModelR7FVECTORT1);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", checkBSphere__FR6CCoordRC7FVECTORf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", checkBSphere__FR6CCoordPC4BBOX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", checkBBox__FRC7FMATRIXPC4BBOX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", resultCheckBBox__FR7FVECTORT0);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", resultCheckBBox__FR6CCoordPC4BBOX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", checkBBox__6CModel);
void CModel::setColorScale(const FVECTOR& vColor) { getColorScale() = vColor; }
void CModel::setColorScale(FLOAT r, FLOAT g, FLOAT b, FLOAT a) { getColorScale().set(r, g, b, a); }
void CModel::setAlphaScale(FLOAT inScale) { m_fAlphaScale = inScale; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", linksub__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", addTexture__6CModelPCci);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setShadowMapModel__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", attachSemiModel__6CModelR10CSemiModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", sendTexture__6CModelPUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getTexParam__6CModelPUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispUpdate__6CModelf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispCheck__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", disp__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", disp__6CModelf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispImpl__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", disp0__6CModeliUli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _disp0_GetData__6CModelRUii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", disp0_SetMicroCode__6CModelPUx);
void CModel::_disp0_SetTexture(INT nFlag, u_long* pTag) {
    if (!(nFlag & 6)) sendTexture(pTag);
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _disp0_SetPacket__6CModelUiPA3_CiiUliPUl);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__10CSemiModelP6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", initSemiZModel__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", releaseSemiZModel__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__10CSemiModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcZ__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getPacketSize__C10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiTrans__10CSemiModelG5ALPHA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispDirect__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__11CSemiZModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__10CFadeModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getPacketSize__C10CFadeModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__8CModelBG);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcZ__5CBBox);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", makePkt__5CBBox);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", linkTexAnm__6CModelP7CTexAnmi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setupUVScroll__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", updateUVScroll__6CModelf);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setUVScrollOffset__6CModeliff);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getExtendClut__6CModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getChangeClut__6CModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", enablePointLight__C9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", enableLight__C9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isCharacter__9MODELDATAUi);
BOOL MODELDATA::isMap(UINT mode) {
    switch (mode) {
    case 0:
    case 0x200:
    case 0x40ee:
        return TRUE;
    }
    return FALSE;
}
BOOL MODELDATA::isSky(UINT mode) { return mode == 0x24; }
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isCharacterAndMap__9MODELDATAUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isNormal__9MODELDATAUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isColorI__9MODELDATAUi);
BOOL MODELDATA::isColorF(UINT mode) {
    switch (mode) {
    case 0x406:
    case 0x4009:
    case 0x400b:
    case 0x4109:
        return TRUE;
    }
    return FALSE;
}
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isTexture__9MODELDATAUi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getModelAttachData__3MadPPCQ23Mad17MODEL_ATTACH_DATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __static_initialization_and_destruction_0_00133620);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", reset__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", disp__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", relink__10CSemiModelP6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiSort__10CSemiModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getParentModel__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setOldZ__10CSemiModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getOldZ__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setDefaultSemiTrans__10CSemiModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", replaceParentModel__10CSemiModelP6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setPacketSize__10CSemiModelUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_10CFadeModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __10CFadeModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_11CSemiZModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __11CSemiZModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__11CSemiZModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcZ__11CSemiZModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelP5CTaskRC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelP5CTaskPC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelR7CCoordsRC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", link__6CModelR7CCoordsPC9MODELDATA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelP5CTaskR9MODELDATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelP5CTaskP9MODELDATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelR7CCoordsR9MODELDATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", assign__6CModelR7CCoordsP9MODELDATAi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiTrans__6CModelG5ALPHA);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getSemiModel__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiSort__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setLightUnit__6CModelR10CLightUnit);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setLightUnit__6CModelP10CLightUnit);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getCoords__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getPacketSize__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isDataEmpty__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isSemiDataEmpty__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeTexture__6CModeliPC7TexInfo);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBoneName__C6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getParentBoneName__C6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getMode__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBBoxFlag__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setBBoxFlag__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBBox__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getModelData__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getTextureInfo__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setTextureInfo__6CModeliPC11TexInfoStub);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", updateTextureInfo__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getTextureNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getTransTextureNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", fade2Path__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isFade2Path__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", notSemi__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isNotSemi__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispOff__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isDispOff__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isDisp__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setBeforeDirect__6CModelPCUxi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", clearBeforeDirect__6CModelPCUxi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getBeforeDirectNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setAfterDirect__6CModelPCUxi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", clearAfterDirect__6CModelPCUxi);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getAfterDirectNum__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getOTZ__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setOTZ__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getColorScale__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getAlphaScale__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", dispWireframe__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isDispWireframe__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setWorldScreenMatrix__6CModelPC7FMATRIX);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", defaultShaderOption__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setShaderOption__6CModelUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", resetShaderOption__6CModelUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__6CModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isClampRepeat__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setClampRepeat__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isShadowMap__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setShadowMap__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isAutoEffSemiSort__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setAutoEffSemiSort__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isRootZSort__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setRootZSort__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isShadow__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isMap__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", createSemiModel__6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isManagedSemiModel__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", manageSemiModel__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isAvailableSemiModel__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", availableSemiModel__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setDispFlag__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isOTZFlag__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setOTZFlag__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", updateOTZ__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isUVScroll__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setUVScroll__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isWBBox__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setWBBox__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", isSemiSortFlag__C6CModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setSemiSortFlag__6CModeli);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_8CModelBG);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __8CModelBGUs);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", disp__8CModelBG);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", setLayerFG__8CModelBGUc);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_5CBBox);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", __5CBBox);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", getPacketSize__C5CBBox);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_12CShadowModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__12CShadowModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcZ__12CShadowModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _$_15CShadowMapModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", changeLayerExt__15CShadowMapModelii);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", calcZ__15CShadowMapModel);
INCLUDE_ASM("asm/SLUS_217.99/nonmatchings/System/Graphic/Model", _GLOBAL_$I$c__KHCOM_MainApp_System_Graphic_Model.cppI3aaaa);
