/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP09
 * Description : LFG Library prototype definitions.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * This file contains the prototype definitions for functions in the LFG
 * library, including the external definitions for globals (in lfg_g.c).
 *
 ****************************************************************************
 */

#ifndef _LFG_PRO_H_
#define _LFG_PRO_H_


/* External Definitions (for lfg_g.c)
 * ---------------------------------
Prototypes_For_lfg_g.c(ForBrief)
 */
extern struct tLFG gsLFG_Internal[];
#ifdef _LFG_DEBUG
extern ui8 gbDebugPortShadow;   /* For using parallel port for timing measurements */
#endif


/* External Definitions (for PLD data)
 * -----------------------------------
Prototypes_For_PLD_data(ForBrief)
 *
 * Note: No need to worry about threadsafe issues as the data is read only.
 */
extern ui8  gbPldCtrlRtData[];
extern ui32 gdwPldCtrlRtByteLength;
extern ui32 gdwPldCtrlRtChecksum;

/* Forward declarations - to quieten gcc */
struct tCDA_Buffer;


/* FUNCTION PROTOTYPES
 * -------------------
 */
#ifdef __cplusplus
extern "C" {
#endif

/* Prototypes for lfg_api.c
 * ------------------------
Prototypes_For_lfg_api.c(ForBrief)
 */
ui32 LFG_EXPORT_FN LFG_EventHandlerInstall(ui32 hCard, void (*pFnHandler)(ui32, ui32, ui32, void*), void* pv);
void LFG_EventHandler( ui32 hCdaDevice, ui32 dwPhysEvent, ui32 dwData, void* pv);
ui32 LFG_EXPORT_FN LFG_Init(struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_Close(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_Open(ui32 *phCard, struct tLFG *psUsrLFG, void (*pFnErrHandler)(ui32, const char*, ui32, const char*) );
ui32 LFG_EXPORT_FN LFG_SetAndGet(ui32 hCard, struct tLFG *psUsrLFG, ui32 dwBitsOptions);
ui32 LFG_EXPORT_FN LFG_GetPointer(ui32 hCard, ui32 dwPtrType, void **ppPtr);
ui32 LFG_EXPORT_FN LFG_PrintReg(ui32 hCard, ui32 dwAddress);
#if defined _LFG_WIN64 || defined _LFG_WIN32 || defined _LFG_MACOSX
ui32 LFG_EXPORT_FN LFG_MemoryAllocate(ui32 hCard, ui32 dwSizeBytes, void **pUserVirtualAddress);
ui32 LFG_EXPORT_FN LFG_MemoryFree(ui32 hCard, void *UserVirtualAddress);
#endif
	
	
/* Prototypes for lfg_risc.c
 * -------------------------
Prototypes_For_lfg_risc.c(ForBrief)
 */
ui32 LFG_EXPORT_FN LFG_AcquisitionStart(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_AcquisitionStop(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_BufferCreate(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_BufferDestroy(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_RiscCodeGenerate(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_RiscCodeAdd(ui32 hCard, ui32 dwRiscCode);
ui32 LFG_EXPORT_FN LFG_RiscCodeFindStart(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_RiscCodeSpaceCreate(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_RiscCodeSpaceDestroy(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_RiscPhysToLog(ui32 hCard);
void LFG_EXPORT_FN LFG_Free32(ui32 hCdaDevice, void * VirtualAddress);
void LFG_EXPORT_PFN LFG_Malloc32(ui32 hCdaDevice, ui32 dwSizeBytes);

/* Prototypes for lfg_mode.c
 * -------------------------
Prototypes_For_lfg_mode.c(ForBrief)
 */
ui32 LFG_EXPORT_FN LFG_VideoModeSet(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_VideoModeMapToClosest(ui32 hCard, struct tLFG *psUsrLFG);


/* Prototypes for lfg_lib.c
 * -----------------------
Prototypes_For_lfg_lib.c(ForBrief)
 */
void LFG_EXPORT_FN LFG_ErrHandlerDefault(ui32 hCard, const char *pszFnName, ui32 dwErrCode, const char *pszDescString);
ui32 LFG_EXPORT_FN LFG_ErrHandlerInstall(ui32 hCard, void (*pFnHandler)(ui32, const char*, ui32, const char*));
void LFG_EXPORT_FN LFG_ErrProcess(ui32 hCard, const char *pszFnName, ui32 dwErrCode, const char *pszDescString);
ui32 LFG_EXPORT_FN LFG_MapLogicalToPhysical(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_MapPhysicalToLogical(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_CalculateBytesPerLine(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_PldConfigure(ui32 hCard, ui32 dwWhichPld);
ui32 LFG_EXPORT_FN LFG_SetRegInterruptMask(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegBrightnessControl(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegContrastControl(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegColourControl(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegHueShift (ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegColorBars(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegCoring(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegEndian(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegGammaRemoval(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegGpio(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegLumaNotch(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegPixelFormat(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegHorzActive(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegVertActive(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegHorzScale(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegVertScale(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegRange(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegTimingControl(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegTriggerMode(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegVideoDecodeStd(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetRegVideoSrc(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetSsResetFieldCount(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_SetSsSwResetChip(ui32 hCard, struct tLFG *psUsrLFG);
ui32 LFG_EXPORT_FN LFG_VidStdAutoDetect(ui32 hCard, struct tLFG *psUsrLFG);


/* Prototypes for lfg_drv.c
 * ------------------------
Prototypes_For_lfg_drv.c(ForBrief)
 */
ui32 LFG_EXPORT_FN LFG_DRV_Close(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_DRV_Open(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_DRV_PldConfigure(ui32 hCard, struct tLFG_PldInfo *psPldInfo);
ui32 LFG_EXPORT_FN LFG_DRV_RegWrite_ui8(ui32 hCard, ui32 dwAddrOffset, ui8 bData);
ui32 LFG_EXPORT_FN LFG_DRV_RegWrite_ui32(ui32 hCard, ui32 dwAddrOffset, ui32 dwData);
ui32 LFG_EXPORT_FN LFG_DRV_RegRead_ui8(ui32 hCard, ui32 dwAddrOffset, ui8 *pbData);
ui32 LFG_EXPORT_FN LFG_DRV_RegRead_ui32(ui32 hCard, ui32 dwAddrOffset, ui32 *pdwData);
ui32 LFG_EXPORT_FN LFG_DRV_RegFlush(ui32 hCard);
ui32 LFG_EXPORT_FN LFG_DRV_Delay(ui32 hCard, ui32 dwMilliSecs);
ui32 LFG_EXPORT_FN LFG_DRV_GetTimeStamp_ms(ui32 hCard, ui32 *pdwTimeStamp_ms);
ui32 LFG_EXPORT_FN LFG_DRV_Timeout(ui32 hCard, ui32 dwTimeout, ui32 *pfTimedOut);

/* Prototypes for lfg_tmg.c
 * ------------------------
Prototypes_For_lfg_tmg.c(ForBrief)
 */
ui32 LFG_EXPORT_FN LFG_TMG_ImageCreate( ui32 *phImage);
ui32 LFG_EXPORT_FN LFG_TMG_ImageDestroy( ui32 hImage);
ui32 LFG_EXPORT_FN LFG_TMG_ImageSet(ui32 hCard, struct tLFG *psUsrLFG, ui32 hImage, i32 nImageNum);

#ifdef __cplusplus
};
#endif

#endif /* _LFG_PRO_H_ */
