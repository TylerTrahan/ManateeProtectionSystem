/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP04
 * Description : CDA API include file.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * CDA prototype definitions.
 *
 ****************************************************************************
 */

#ifndef _CDA_PRO_H_
#define _CDA_PRO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* External Definitions (for g_cda.c)
 * ----------------------------------
Prototypes_For_g_cda.c(ForBrief)
 */
extern struct tCDA gsCDA_Internal[];
extern ui32 ghUserTohDevice[];


/* Prototype Definitions
 * ---------------------
 * User API functions.
Prototypes_for_cda_api.c(ForBrief)
 */
ui32 CDA_EXPORT_FN CDA_Close(ui32 hDevice);
ui32 CDA_EXPORT_FN CDA_Delay_ms(ui32 hDevice, ui32 dwMilliSecs);
ui32 CDA_EXPORT_FN CDA_EventHandlerInstall( ui32 hDevice, void (*pFnHandler)( ui32, ui32, ui32, void*), void*);
ui32 CDA_EXPORT_FN CDA_Init(struct tCDA *psCDA);
ui32 CDA_EXPORT_FN CDA_GetTimeStamp(ui32 hDevice, ui32 *pdwTimeStamp_ms);
ui32 CDA_EXPORT_FN CDA_Open(ui32 *phDevice, struct tCDA *psTempCDA, struct tCDA **ppsCDA, i32 nNumBytesToAllocate, void (*pFnErrHandler)(ui32, const char*, ui32, const char*) );
ui32 CDA_EXPORT_FN CDA_MemoryAlloc(ui32 hDevice, void **ppBuffer, ui32 dwSizeInBytes, ui32 dwAlignment, ui32 dwFlags );
ui32 CDA_EXPORT_FN CDA_MemoryFree (ui32 hDevice, void **ppBuffer );
ui32 CDA_EXPORT_FN CDA_MemoryLock  (ui32 hDevice, struct tCDA_Buffer *psCdaBuffer);
ui32 CDA_EXPORT_FN CDA_MemoryLockEx(ui32 hDevice, struct tCDA_Buffer *psCdaBuffer, ui32 dwDma);
ui32 CDA_EXPORT_FN CDA_MemoryUnlock(ui32 hDevice, struct tCDA_Buffer *psCdaBuffer);
ui32 CDA_EXPORT_FN CDA_MemoryUserLock  (ui32 hDevice, void *pvAddress, ui64 qwSizeBytes);
ui32 CDA_EXPORT_FN CDA_MemoryUserUnlock(ui32 hDevice, void *pvAddress, ui64 qwSizeBytes);
ui32 CDA_EXPORT_FN CDA_MemoryFlush (ui32 hDevice, struct tCDA_Buffer *psCdaBuffer, ui32 dwDma);
ui32 CDA_EXPORT_FN CDA_RegFlush(ui32 hDevice);
ui32 CDA_EXPORT_FN CDA_RegRead(ui32 hDevice, ui32 dwBitsRegType, ui32 dwAddress, volatile ui32 *pdwData);
ui32 CDA_EXPORT_FN CDA_RegWrite(ui32 hDevice, ui32 dwBitsRegType, ui32 dwAddress, ui32 dwData);
ui32 CDA_EXPORT_FN CDA_RegWriteCached(ui32 hDevice, ui32 dwBitsRegType, ui32 dwAddress, ui32 dwData);
ui32 CDA_EXPORT_FN CDA_Timeout(ui32 hDevice, ui32 dwTimeout, ui32 *pfTimedOut);
ui32 CDA_EXPORT_FN CDA_DeviceControl(ui32 hDevice, ui32 dwCommand, void *pvData);
#if defined _CDA_VXWORKS
ui32 CDA_EXPORT_FN CDA_DRV_Init(void); /* The "driver" needs to be manually initialised by the application */
ui32 CDA_EXPORT_FN CDA_DRV_CacheFlush(ui32 hDevice, void *pBuffer, ui32 dwSize);
#endif

/*
Prototypes_for_cda_lib.c(ForBrief)
*/
void CDA_EXPORT_FN CDA_ErrHandlerDefault(ui32 hDevice, const char *pszFnName, ui32 dwErrCode, const char *pszDescString);
ui32 CDA_EXPORT_FN CDA_ErrHandlerInstall(ui32 hDevice, void (*pFnHandler)(ui32, const char*, ui32, const char*));
void CDA_EXPORT_FN CDA_ErrProcess(ui32 hDevice, const char *pszFnName, ui32 dwErrCode, const char *pszDescString);


/*
Prototypes_for_cda_<os>.c(ForBrief)
*/
ui32 CDA_EXPORT_FN  CDA_CriticalSectionBegin(ui32 hDevice);
ui32 CDA_EXPORT_FN  CDA_CriticalSectionEnd(ui32 hDevice);
ui32 CDA_EXPORT_FN  CDA_DRV_Close(ui32 hDevice);
ui32 CDA_EXPORT_FN  CDA_DRV_Delay_ms(ui32 hDevice, ui32 dwMilliSecs);
ui32 CDA_EXPORT_FN  CDA_DRV_DeviceControl(ui32 hDevice, ui32 dwCommand, void *pvData);
ui32 CDA_EXPORT_FN  CDA_DRV_GetTimeStamp(ui32 hDevice, ui32 *pdwTimeStamp_ms);
ui32 CDA_EXPORT_FN  CDA_DRV_Initialisation();
ui32 CDA_EXPORT_FN  CDA_DRV_Open(ui32 hDevice);
ui32 CDA_EXPORT_FN  CDA_DRV_MemoryAlloc (ui32 hDevice, void **ppBuffer, ui32 dwSizeInBytes );
ui32 CDA_EXPORT_FN  CDA_DRV_MemoryLock  (ui32 hDevice, struct tCDA_Buffer *psCdaBuffer);
ui32 CDA_EXPORT_FN  CDA_DRV_MemoryLockEx(ui32 hDevice, struct tCDA_Buffer *psCdaBuffer, ui32 dwDma);
ui32 CDA_EXPORT_FN  CDA_DRV_MemoryUnlock(ui32 hDevice, struct tCDA_Buffer *psCdaBuffer);
ui32 CDA_EXPORT_FN  CDA_DRV_MemoryFlush (ui32 hDevice, struct tCDA_Buffer *psCdaBuffer, ui32 dwDma);
ui32 CDA_EXPORT_FN  CDA_DRV_ParallelPort(ui32 hDevice, ui32 dwBitPos, i32 nPolarity);
ui32 CDA_EXPORT_FN  CDA_DRV_RegRead(ui32 hDevice, ui32 dwBitsRegType, ui32 dwAddress, volatile ui32 *pdwData);
ui32 CDA_EXPORT_FN  CDA_DRV_RegWrite(ui32 hDevice);
ui32 CDA_EXPORT_FN  CDA_DRV_SendRegList(ui32 hDevice);
ui32 CDA_EXPORT_FN  CDA_DRV_Malloc32(ui32 hDevice, ui32 dwSizeBytes, void* *ppVirtualAddress);
ui32 CDA_EXPORT_FN  CDA_DRV_Free32(ui32 hDevice, void* pVirtualAddress);

#ifdef __cplusplus
};
#endif

#endif  /* _CDA_PRO_H_ */

