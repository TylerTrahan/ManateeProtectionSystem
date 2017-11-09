/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP04
 * Description : All definitions that vary with operating system environment.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * This file contains sections for code for each of the operating system
 * environments & drivers that it supports (e.g. WinNT & Win95 etc).
 *
 ****************************************************************************
 */

#ifndef _CDA_OS_H_
#define _CDA_OS_H_

/* Definitions used for each OS type below
 * ---------------------------------------
 */
#define CDA_DEBUG_STRING_LEN 128

#if defined _VRP_ARM7
#if !defined _CDA_ARM7
#define _CDA_ARM7
#endif
#endif

#if defined _VRP_DEBUG
#if !defined _CDA_DEBUG
#define _CDA_DEBUG
#endif
#endif

#if defined _CDA_WINNT || defined _CDA_WIN95 || defined _CDA_WIN64
#define _CDA_WIN32
#endif


/* Standard Host Include Files - Note dependent on operating system
 * ----------------------------------------------------------------
 */
#ifdef _CDA_WIN32  /*======================================================*/

/*
_CDA_WIN32(ForBrief)
--------------------
*/

/* 1. Include Files
 * ----------------
 */
#include <windows.h>     /* windows.h must be included first */
#ifdef _WINDOWS          /* This is an MS Windows definition -    */
#include <windowsx.h>    /* not defined for console mode programs */
#endif   /* _WINDOWS */

#include <conio.h>
#include <io.h>
#include <stdarg.h>     /* must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <memory.h>
#include <assert.h>


/* 2. Malloc/free macros
 * ---------------------
 */
#define _CDA_Malloc(x) malloc(x)
#define _CDA_Free(x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
typedef long            i32;
typedef unsigned long  ui32;
typedef INT64           i64;   
typedef UINT64         ui64;   
typedef long            m16;
typedef unsigned long  mu16;
typedef long            m32;
typedef unsigned long  mu32;
#define _ASL_TYPEDEFS
#endif

#ifndef TRUE
#define TRUE     (1==1)
#define FALSE    (!TRUE)
#endif


/* 4. Library export and calling convention definitions
 * ----------------------------------------------------
 */
#if defined _PHX_CDA
  #define CDA_EXPORT_FN
  #define CDA_EXPORT_PFN
#else
  #define CDA_EXPORT_FN   __declspec(dllexport)
  #define CDA_EXPORT_PFN  __declspec(dllexport)*
#endif


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#ifdef _CDA_DEBUG  /* Only support under debug, so that Windows 95 works in
                    * release mode (although obviously not in debug mode).
                    */
  /* LVR: IsDebuggerPresent is defined only #if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
   * otherwise #define IsDebuggerPresent() 1 */
#ifdef __cplusplus
extern "C" WINBASEAPI BOOL WINAPI IsDebuggerPresent(VOID); /* kernel32.lib requires NT4 or Win98 */
#define _CDA_ErrPrint(szMessage, szTitle) \
  {IsDebuggerPresent() ? (OutputDebugString( szMessage), IDOK) : ::MessageBox(NULL,szMessage,szTitle,MB_OK);}
#else
extern WINBASEAPI BOOL WINAPI IsDebuggerPresent(VOID); /* kernel32.lib requires NT4 or Win98 */
#define _CDA_ErrPrint(szMessage, szTitle) \
  {IsDebuggerPresent() ? (OutputDebugString( szMessage), IDOK) : MessageBox(NULL,szMessage,szTitle,MB_OK);}
#endif
#else   /* Not CDA debug */
#define _CDA_ErrPrint(szMessage, szTitle) 
#endif


/* 6. Assert Macros
 * ----------------
 * The Microsoft ANSI C "assert" is not compiled in when NDEBUG is selected,
 * which is selected by default on the project makefiles for release builds.
 */
#define _CDA_Assert assert

#ifdef _CONSOLE   /* Win32 automatic definition */

/* 7. Debug Macros
 * ---------------
 */
#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("\nCDA_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   ---------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle) {::MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#else
#define _CDA_DebugPopup(pszMessage, pszTitle) {MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */

#else   /* #else part of "#ifdef _CONSOLE"   */

#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
 * ----------
 * Note that dwParameter2 is not used.
 */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " 0x%lx (%ld)\n", (ui32) dwParameter, (ui32) dwParameter); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   ---------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   --------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle) {::MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#else
#define _CDA_DebugPopup(pszMessage, pszTitle) {MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */

#endif  /* #else part of "#ifdef _CONSOLE"  */


#elif defined _CDA_DOS32 /*=================================================*/

/*
_CDA_DOS32(ForBrief)
--------------------
*/

#ifdef __WATCOMC__
#define _FG_GRAPHICS       /* Watcom command length problems */
#define _CDA_DEBUG
#endif


/* 1. Include Files
 * ----------------
 */
#include <conio.h>
#include <io.h>
#include <stdarg.h>     /* must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <memory.h>
#include <assert.h>
#include <bios.h>
#include <dos.h>


/* 2. Malloc/free macros
 * ---------------------
 */
#define _CDA_Malloc(x) malloc(x)
#define _CDA_Free(x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
typedef long            i32;
typedef unsigned long  ui32;
typedef long            m16;
typedef unsigned long  mu16;
typedef long            m32;
typedef unsigned long  mu32;
#define _ASL_TYPEDEFS
#endif

#ifndef TRUE
#define TRUE     (1==1)
#define FALSE    (!TRUE)
#endif


/* 4. Library export and calling convention definitions
 * ----------------------------------------------------
 */
/* Define __cdecl, as GNUC doesn't define it */
#if defined __GNUC__
 #define __cdecl
#endif

#if defined _PHX_CDA
  #define CDA_EXPORT_FN
  #define CDA_EXPORT_PFN
#else
  #define CDA_EXPORT_FN   __cdecl
  #define CDA_EXPORT_PFN  __cdecl*
#endif

/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _CDA_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _CDA_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("CDA_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle) {::MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#else
#define _CDA_DebugPopup(pszMessage, pszTitle) {MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */


#elif defined _CDA_ARM7 /*=================================================*/


/*
_CDA_ARM7(ForBrief)
-------------------
*/

/* NOTE: For ARM we conveniently define these low level macros here.
 */
#define _DRV_Write_ui8( dwAddr, bData ) \
         *(ui8*)(dwAddr) = (bData)
#define _DRV_Write_ui16( dwAddr, wData ) \
         *(ui16*)(dwAddr) = (wData)
#define _DRV_Write_ui32( dwAddr, dwData ) \
         *(ui32*)(dwAddr) = (dwData)
#define _DRV_Read_ui8( dwAddr ) \
         *(volatile ui8*)(dwAddr)
#define _DRV_Read_ui16( dwAddr ) \
         *(volatile ui16*)(dwAddr)
#define _DRV_Read_ui32( dwAddr ) \
         *(volatile ui32*)(dwAddr)


/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* Must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>


/* 2. Malloc/free macros
 * ---------------------
 */
#define _CDA_Malloc(x) malloc(x)
#define _CDA_Free(x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
/* Note: int is 32 bit (as is long) but long generates a compiler warning */
typedef int             i32;
typedef unsigned int   ui32;
typedef long            m16;
typedef unsigned long  mu16;
typedef long            m32;
typedef unsigned long  mu32;
#define _ASL_TYPEDEFS
#endif

#ifndef TRUE
#define TRUE     (1==1)
#define FALSE    (!TRUE)
#endif


/* 4. Library export and calling convention definitions
 * ----------------------------------------------------
 */
#define CDA_EXPORT_FN
#define CDA_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _CDA_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _CDA_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("CDA_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle) 
#else
#define _CDA_DebugPopup(pszMessage, pszTitle) 
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */

#elif defined _CDA_LINUX /*=================================================*/


/*
_CDA_LINUX(ForBrief)
-------------------
*/


/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* Must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>

/* 2. Malloc/free macros
 * ---------------------
 */
#define _CDA_Malloc(x) malloc(x)
#define _CDA_Free(x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
/* Note: int is 32 bit (as is long) but long generates a compiler warning */
typedef int             i32;
typedef unsigned int   ui32;
#if defined _LP64
typedef unsigned long ui64;
#else
typedef unsigned long long ui64;
#endif /* _LP64 */
typedef long            m16;
typedef unsigned long  mu16;
typedef long            m32;
typedef unsigned long  mu32;
#define _ASL_TYPEDEFS
#endif

#ifndef TRUE
#define TRUE     (1==1)
#define FALSE    (!TRUE)
#endif


/* 4. Library export and calling convention definitions
 * ----------------------------------------------------
 */
#if defined _PHX_CDA
  #define CDA_EXPORT_FN  __attribute__ ((visibility("hidden")))
  #define CDA_EXPORT_PFN __attribute__ ((visibility("hidden")))
#else
  #define CDA_EXPORT_FN
  #define CDA_EXPORT_PFN *
#endif

/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _CDA_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _CDA_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("CDA_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle)
#else
#define _CDA_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */


#elif defined _CDA_MACOSX /*=================================================*/

/*
_CDA_MACOSX(ForBrief)
-------------------
*/

/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* Must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#if !defined __MWERKS__
#include <pthread.h>
#import <Carbon/Carbon.h>
#else
#include <Carbon.h>
#endif
/* 2. Malloc/free macros
 * ---------------------
 */
#define _CDA_Malloc(x) malloc(x)
#define _CDA_Free(x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef SInt8            i8;
typedef UInt8           ui8;
typedef SInt16          i16;
typedef UInt16         ui16;
typedef SInt32          i32;
typedef UInt32         ui32;
typedef SInt64          i64;
typedef UInt64         ui64;
typedef SInt16          m16;
typedef UInt16         mu16;
typedef SInt32          m32;
typedef UInt32         mu32;
#define _ASL_TYPEDEFS
#endif

#ifndef TRUE
#define TRUE     (1==1)
#define FALSE    (!TRUE)
#endif


/* 4. Library export and calling convention definitions
 * ----------------------------------------------------
 */
#define CDA_EXPORT_FN 
#define CDA_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
extern void CDA_DRV_ErrPrint(const char *, const char *);
#if defined _CDA_DEBUG
#define _CDA_ErrPrint(szMessage, szTitle) {CDA_DRV_ErrPrint(szTitle, szMessage);}
#else
#define _CDA_ErrPrint(szMessage, szTitle)
#endif

/* 6. Assert Macros
 * ----------------
 */
#define _CDA_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("CDA_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle)
#else
#define _CDA_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */

#elif defined _CDA_VXWORKS /*=================================================*/


/*
_CDA_VXWORKS(ForBrief)
-------------------
*/


/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* Must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
/*
 * System Include Files
 * --------------------
 */
#include <taskLib.h>
#include <vmLib.h>
#include <cacheLib.h>
#include <intLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <vwModNum.h>
#include <errnoLib.h>
#include <sigLib.h>
#include <wdLib.h>
#include <timers.h>

/* 2. Malloc/free macros
 * ---------------------
 */
void *CDA_DRV_Malloc(size_t);
void  CDA_DRV_Free(void*);
#define _CDA_Malloc(x) CDA_DRV_Malloc(x)
#define _CDA_Free(x)   CDA_DRV_Free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
/* Note: int is 32 bit (as is long) but long generates a compiler warning */
typedef int             i32;
typedef unsigned int   ui32;
typedef unsigned long long ui64;
typedef int            m16;
typedef unsigned int   mu16;
typedef int            m32;
typedef unsigned int   mu32;
#define _ASL_TYPEDEFS
#endif

#ifndef TRUE
#define TRUE     (1==1)
#define FALSE    (!TRUE)
#endif


/* 4. Library export and calling convention definitions
 * ----------------------------------------------------
 */
#define CDA_EXPORT_FN
#define CDA_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _CDA_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _CDA_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("CDA_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle)
#else
#define _CDA_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */

#elif defined _CDA_QNX4 /*=================================================*/

/*
_CDA_QNX4(ForBrief)
-------------------
*/


/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* Must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <process.h>
#include <semaphore.h>
#include <sys/qnx_glob.h>

/* 2. Malloc/free macros
 * ---------------------
 */
#define _CDA_Malloc(x) malloc(x)
#define _CDA_Free(x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
/* Note: int is 32 bit (as is long) but long generates a compiler warning */
typedef int             i32;
typedef unsigned long  ui32;
typedef long            m16;
typedef unsigned long  mu16;
typedef long            m32;
typedef unsigned long  mu32;
#define _ASL_TYPEDEFS
#endif

#ifndef TRUE
#define TRUE     (1==1)
#define FALSE    (!TRUE)
#endif


/* 4. Library export and calling convention definitions
 * ----------------------------------------------------
 */
#define CDA_EXPORT_FN
#define CDA_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _CDA_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _CDA_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("CDA_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle)
#else
#define _CDA_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */


#elif defined _CDA_QNX6 /*=================================================*/

/*
_CDA_QNX6(ForBrief)
-------------------
*/


/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* Must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <process.h>
#include <semaphore.h>
#include <pthread.h>

#define CDA_MAX_MESSAGE 4096

/* 2. Malloc/free macros
 * ---------------------
 */
#define _CDA_Malloc(x) malloc(x)
#if !defined _CLR_QNX6_CHECK_CALLBACK
#define _CDA_Free(x)   free(x)
#else
extern volatile int g_clr_callback_flag; /* CLR - for QNX callback debugging. */
#define _CDA_Free(x) do { if (g_clr_callback_flag > 0) printf("Free callback\n");  free((x)); }while(0)
#endif

/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
/* Note: int is 32 bit (as is long) but long generates a compiler warning */
typedef int             i32;
typedef unsigned int   ui32;
typedef int             m16;
typedef unsigned int   mu16;
typedef int             m32;
typedef unsigned int   mu32;
typedef unsigned long long ui64;
#define _ASL_TYPEDEFS
#endif

#ifndef TRUE
#define TRUE     (1==1)
#define FALSE    (!TRUE)
#endif


/* 4. Library export and calling convention definitions
 * ----------------------------------------------------
 */
#define CDA_EXPORT_FN
#define CDA_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _CDA_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _CDA_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _CDA_DEBUG

/* _CDA_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("CDA_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _CDA_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _CDA_DebugString(pszMessage, pszParameter) { \
  char _szWork[CDA_DEBUG_STRING_LEN]; char _szParamString[CDA_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( _szWork ); }

/* _CDA_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _CDA_DebugPopup(pszMessage, pszTitle)
#else
#define _CDA_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _CDA_DEBUG not defined */

#define _CDA_Debug(pszMessage, dwParameter1, dwParameter2)
#define _CDA_DebugString(pszMessage, pszParameter)
#define _CDA_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _CDA_DEBUG" */


#else

#error You have a missing target environment directive (e.g. _CDA_WINNT)

#endif

#endif /* _CDA_OS_H_ */

