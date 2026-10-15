#ifndef SYSTEM_SOUND_DRIVER_SDDRV_EE_H
#define SYSTEM_SOUND_DRIVER_SDDRV_EE_H

#include "khtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void TsPrintN(char *args, ...);
void TsSdDebugPrint();
void TsSdSetWdMain(int flag, u_int addr, int wd_num, int wd_id, int size);
void TsSdSetWd(int flag, u_int addr);
void TsSdSetWd2(int flag, u_int addr, int wd_id);
void TsSdSetWdClear(int wd_num);
int TsSdSetWdCheck(int wd_num);
void TsSdSetWdClearAddr(int addr);
int TsSdSetWdCheckAddr(int addr);
int TsSdSetWdCheckArea(int wd_id);
int TsSdGetWdOpenArea(int wd_id);
void TsSdSetSebMain(int flag, u_int addr, int seb_num, int seb_id, int size);
void TsSdSetSeb(int flag, u_int addr);
void TsSdSetSeb2(int flag, u_int addr, int seb_id);
void TsSdSetSebClear(int seb_num);
int TsSdSetSebCheck(int seb_num);
void TsSdSetSebClearAddr(int addr);
int TsSdSetSebCheckAddr(int addr);
int TsSdGetSebOpenArea(int seb_id);
int TsSdGetSebID(u_int seb_num, int check);
int TsSdGetSebNum(u_int addr);
int TsSdGetSepNum(int addr);
u_int TsSdGetSepNum2(int addr);
int TsSdSetSepParam(int sep_num, int check);
void TsSdSetSep(int flag, u_int addr);
void TsSdSetSepClear(int addr);
void TsSdSetSepClearAll();
int TsSdSetSepCheck(int addr);
void TsSdSetRevDepth(int depth);
void TsSdResetRevDepth();
void TsSdSebPlaySI2(int seb_id, int num, int vol, int pan, int se_id);
void TsSdSebStopSI2(int seb_id, int num, int se_id);
void TsSdSebStartSI2(int seb_id, int num, int se_id);
void TsSdSebPauseSI2(int seb_id, int num, int se_id);
void TsSdSebSetVPPSI2(int seb_id, int num, int vol, int pan, int pitch, int se_id);
void TsSdSebSetVPSI2(int seb_id, int num, int vol, int pan, int se_id);
void TsSdSebSetVolSI2(int seb_id, int num, int vol, int se_id);
void TsSdSebSetPanSI2(int seb_id, int num, int pan, int se_id);
void TsSdSebFadeVPSI2(int seb_id, int num, int vol, int pan, int step, int flag, int turn, int se_id);
void TsSdSebFadeVolSI2(int seb_id, int num, int vol, int step, int flag, int se_id);
void TsSdSebFadePanSI2(int seb_id, int num, int pan, int step, int turn, int se_id);
int TsSdSebPlayCheckSI2(int seb_id, int num, int se_id);
void TsSdSebPlaySI(int seb_id, int num, int vol, int pan);
void TsSdSebStopSI(int seb_id, int num);
void TsSdSebStartSI(int seb_id, int num);
void TsSdSebPauseSI(int seb_id, int num);
void TsSdSebSetVPSI(int seb_id, int num, int vol, int pan);
void TsSdSebSetVolSI(int seb_id, int num, int vol);
void TsSdSebSetPanSI(int seb_id, int num, int pan);
void TsSdSebFadeVPSI(int seb_id, int num, int vol, int pan, int step, int flag, int turn);
void TsSdSebFadeVolSI(int seb_id, int num, int vol, int step, int flag);
void TsSdSebFadePanSI(int seb_id, int num, int pan, int step, int turn);
int TsSdSebPlayCheckSI(int seb_id, int num);
void TsSdSebPlaySN(int seb_num, int num, int vol, int pan, int se_id);
void TsSdSebStopSN(int seb_num, int num, int se_id);
void TsSdSebStartSN(int seb_num, int num, int se_id);
void TsSdSebPauseSN(int seb_num, int num, int se_id);
void TsSdSebSetVPSN(int seb_num, int num, int vol, int pan, int se_id);
void TsSdSebFadeVPSN(int seb_num, int num, int vol, int pan, int step, int flag, int turn, int se_id);
void TsSdSebFadeVolSN(int seb_num, int num, int vol, int step, int flag, int se_id);
int TsSdSebPlayCheckSN(int seb_num, int num, int se_id);
int TsSdSebTimeCheck(int *buf, int num);
void TsSdSeStopAll();
int TsSdSePlayCheck2(int num);
int TsSdSePlayCheck(int num);
void TsSdSepPlayDBG(int addr, int vol, int pan, int se_id, int mtrack, int dtime);
void TsSdSepPlay(int addr, int vol, int pan);
void TsSdSepPlay2(int addr, int vol, int pan, int se_id);
void TsSdSepStop(int addr);
void TsSdSepStop2(int addr, int se_id);
void TsSdSepStart(int addr);
void TsSdSepPause(int addr);
void TsSdSepSetVPP(int addr, int vol, int pan, int pitch);
void TsSdSepSetVPP2(int addr, int vol, int pan, int pitch, int se_id);
void TsSdSepSetVP(int addr, int vol, int pan);
void TsSdSepSetVol(int addr, int vol);
void TsSdSepSetPan(int addr, int pan);
int TsSdSepGetTime(int addr);
void TsSdSepFadeVP(int addr, int vol, int pan, int step, int flag, int turn);
void TsSdSepFadeVol(int addr, int vol, int step, int flag);
void TsSdSepFadeVol2(int addr, int vol, int step, int flag, int se_id);
void TsSdSepFadePan(int addr, int pan, int step, int turn);
int TsSdSepPlayCheck(int addr);
int TsSdSepPlayCheck2(int addr, int se_id);
int TsSdSepPlayCheckAddr();
void TsSdSepClearAddr();
void TsSdStrPlayMusicAddr(u_int addr, int play_sect);
void TsSdStrPlayMusic(int sect, int play_sect, char *filename, int id);
void TsSdStrPlayMusicCrossFade(int sect, int play_sect, char *filename, int id, u_int addr, int read_sect, int fade_step);
void TsSdStrPlayVoice(u_int addr);
void TsSdStrStop(int type, int id);
void TsSdStrFadeVol(int type, int id, int vol, int step, int flag);
void TsSdStrFadePan(int type, int id, int pan, int step, int turn);
void TsSdStrVolSet(int type, int id, int vol);
void TsSdStrVolSet2(int type, int id, int vol);
void TsSdStrVolReset(int type, int id);
void TsSdStrVoiceSetPan(u_int addr, int pan);
void TsSdStrPause(int type, int id);
void TsSdStrStart(int type, int id);
int TsSdStrPlayMusicCheck(int id);
int TsSdStrPlayVoiceCheck(int addr);
int TsSdStrPlayVoiceCheckAll();
int TsSdStrGetStatus(int type, int id);
int TsSdStrGetStatusAddr(int type, int addr);
int TsSdStrGetMarker(int type, int id);
int TsSdStrGetCount(int type, int id);
int TsSdStrGetCountAddr(int type, int addr);
int TsSdStrGetTime(int addr);
int ConvLittleEndian(int a);
void TsSdVsbPlay(u_int addr, int vsb_id, int num, int vol, int pan);
void TsSdVsbStop(int vsb_id);
void TsSdVsbSetVP(int vsb_id, int num, int vol, int pan);
void TsSdVsbPause(int vsb_id, int num);
void TsSdVsbStart(int vsb_id, int num);
void TsSdMpegTransVoice(u_int m_addr, u_int s_addr, u_int size);
void TsSdMpegTransBlock(u_int m_addr, u_int size, u_int start_addr);
u_int TsSdMpegTransBlockStop();
u_int TsSdMpegTransBlockStatus(int channel, int flag);
void TsSdMpegSetVolume(int vol);
int TsSdMpegGetAddrIOP();
void TsSdStopAll(int flag);
void TsSdFadeOutAll(int step);
int TsSdFadeOutAllCheck();
void TsSdCheckDriverGetParam();
int TsSdCheckDriverSetParam();
int TsSdCheckDriverSetParam2();
void TsSdCheckDriver();
void TsSdSetModeOutput(int type);
void TsSdSetMasterVol2_StrMusic(int mvol);
void TsSdSetMasterVol2_StrVoice(int mvol);
void TsSdSetMasterVol2_SeqSE(int mvol);
void TsSdSetMasterVol2_SeqVoice(int mvol);
int TsSdGetModeOutput();
int TsSdGetMasterVol2_StrMusic();
int TsSdGetMasterVol2_StrVoice();
int TsSdGetMasterVol2_SeqSE();
int TsSdGetMasterVol2_SeqVoice();
void TsInitParam();
void TsInitData();
void TsInitSif();
void TsSdInitDriver();

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_SOUND_DRIVER_SDDRV_EE_H
