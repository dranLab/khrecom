#ifndef SYSTEM_GRAPHIC_WORLDENV_H
#define SYSTEM_GRAPHIC_WORLDENV_H

#include "khtypes.h"

class CEffectLight;
class CCamera;
class CBillboardHelper;
union FVECTOR;
union IVECTOR;
union FMATRIX;

class CWorldEnv {
public:
    static void initialize();
    static void finalize();
    static void mainProc();
    static BOOL pushCamera(CCamera& inCamera);
    static BOOL popCamera(CCamera& inCamera);
    static void removeAllCamera();
    static CCamera* getActiveCamera();
    static CCamera* getOldCamera();
    static void setFogParam(FLOAT fNear, FLOAT fFar, FLOAT fMax, FLOAT fMin);
    static FVECTOR& getFogParam();
    static FLOAT getFogNear();
    static FLOAT getFogFar();
    static void setFogColor(INT r, INT g, INT b);
    static IVECTOR& getFogColor();
    static FLOAT getDispFar();
    static void setDispFar(FLOAT far);
    static void setScreenZ(FLOAT z);
    static FLOAT getScreenZ();
    static void setDefScreenZ(FLOAT z);
    static FLOAT getDefScreenZ();
    static void getViewVector(FVECTOR& vecOut);
    static const FVECTOR& getViewVector();
    static FMATRIX& getWorldViewMatrix();
    static FMATRIX& getViewScreenMatrix();
    static FMATRIX& getWorldScreenMatrix();
    static FMATRIX& getViewClipMatrix();
    static FMATRIX& getWorldClipMatrix();
    static TIMEQ getWorldScreenTimestamp();
    static FVECTOR& getEyePos();
    static const FVECTOR& getMikePos();
    static void readyMike(BOOL bReady, BOOL bSetup);
    static BOOL isReadyMike();
    static void setSunPos(const FVECTOR& vecInPos);
    static FVECTOR& getSunPos();
    static void getSunPosScr(FVECTOR& vecOut);
    static BOOL sphereVisible(FVECTOR& inCenter, FLOAT inRadius);
    static BOOL ellipsoidVisible(FVECTOR& inCenter, FVECTOR& inR, FVECTOR& inS, FVECTOR& inT);
    static BOOL cylinderVisible(FVECTOR& inP0, FVECTOR& inP1, FLOAT inRadius);
    static CEffectLight* getBlankEffLight();
    static CEffectLight* getEffLight(UINT nIndex);
    static void initSeekActiveEffLight();
    static CEffectLight* seekNextActiveEffLight();
    static void resetEffLight();
    static CBillboardHelper& getBBHelper();
    static void getBillboardMatrix(FMATRIX& matOut, const FVECTOR& vecPos, INT nFlag);
};

void applySignWorldViewMatrix(FMATRIX& m0, const FMATRIX& m1);
void getViewportMatrix(FMATRIX& matViewport, float cx, float cy, float width, float height, float maxz, float minz);
void getPerspectiveMatrix(FMATRIX& m, float scrz, float ax, float ay, float width, float height, float r_near,
                          float r_far);
void getPerspectiveMatrix(FMATRIX& m, float fovy, float aspect, float r_near, float r_far);
void getOrthoMatrix(FMATRIX& m, float left, float right, float bottom, float top, float r_near, float r_far);

#endif /* SYSTEM_GRAPHIC_WORLDENV_H */
