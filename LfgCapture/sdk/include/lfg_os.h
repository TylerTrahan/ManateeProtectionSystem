/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP09
 * Description : All definitions that vary with operating system environment.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * This file contains sections for code for each of the operating system
 * environments & driver layer that it supports (e.g. Win32 & CDA driver).
 *
 ****************************************************************************
 */

#ifndef _LFG_OS_H_
#define _LFG_OS_H_


/* Definitions used for each OS type below
 * ---------------------------------------
 */
#define LFG_DEBUG_STRING_LEN 160

#ifdef _LFG_DEBUG
#define _CDA_DEBUG
#endif

#if defined _LFG_WIN64
#if !defined _LFG_WIN32
#define _LFG_WIN32
#endif
#endif

#ifdef _LFG_WIN32  /*======================================================*/

/*
_LFG_WIN32(ForBrief)
-------------------
*/
#if !defined _CDA_WIN32
#define _CDA_WIN32
#endif

/* 1. Include Files
 * ----------------
 */
#include <windows.h>     /* windows.h must be included first */
#ifdef _WINDOWS          /* This is an MS Windows definition -    */
#include <windowsx.h>    /* not defined for console mode programs */
#include <ddraw.h>       /* For Windows NT (4) Direct Draw API    */
#endif   /* _WINDOWS */

#include <conio.h>
#include <io.h>
#include <stdarg.h>      /* Must be before stdio.h for MS Windows */
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
#define _LFG_Malloc(h, x) LFG_Malloc32((h), (x))
#define _LFG_Free(h, x)   LFG_Free32((h), (x))


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
#define LFG_EXPORT_FN   __declspec(dllexport)
#define LFG_EXPORT_PFN  __declspec(dllexport)*


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#ifdef _LFG_DEBUG  /* Only support under debug, so that Windows 95 works in
                    * release mode (although obviously not in debug mode).
                    */
  /* LVR: IsDebuggerPresent is defined only #if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
   * otherwise #define IsDebuggerPresent() 1 */
#ifdef __cplusplus
extern "C" WINBASEAPI BOOL WINAPI IsDebuggerPresent(VOID); /* kernel32.lib requires NT4 or Win98 */
#define _LFG_ErrPrint(szMessage, szTitle) \
  {IsDebuggerPresent() ? (OutputDebugString( szMessage), IDOK) : ::MessageBox(NULL,szMessage,szTitle,MB_OK);}
#else
extern WINBASEAPI BOOL WINAPI IsDebuggerPresent(VOID); /* kernel32.lib requires NT4 or Win98 */
#define _LFG_ErrPrint(szMessage, szTitle) \
  {IsDebuggerPresent() ? (OutputDebugString( szMessage), IDOK) : MessageBox(NULL,szMessage,szTitle,MB_OK);}
#endif
#else   /* Not LFG debug */
#define _LFG_ErrPrint(szMessage, szTitle) 
#endif


/* 6. Assert Macros
 * ----------------
 * The Microsoft ANSI C "assert" is not compiled in when NDEBUG is selected,
 * which is selected by default on the project makefiles for release builds.
 */
#define _LFG_Assert assert

#ifdef _CONSOLE  /* Win32 automatic definition */

/* 7. Debug Macros
 * ---------------
 */
#ifdef _LFG_DEBUG

/* _LFG_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("\nLFG_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _LFG_DebugString: Outputs a string as part of a debug string.
   ---------------- */
#define _LFG_DebugString(pszMessage, pszParameter) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _LFG_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _LFG_DebugPopup(pszMessage, pszTitle) {::MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#else
#define _LFG_DebugPopup(pszMessage, pszTitle) {MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#endif

#else   /* no debugging - _LFG_DEBUG not defined */

#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2)
#define _LFG_DebugString(pszMessage, pszParameter)
#define _LFG_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _LFG_DEBUG" */

#else   /* #else part of "#ifdef _CONSOLE"   */

#ifdef _LFG_DEBUG

/* _LFG_Debug: Outputs a long word as part of a debug string.
   --------- */
#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _LFG_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _LFG_DebugString(pszMessage, pszParameter) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _LFG_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _LFG_DebugPopup(pszMessage, pszTitle) {::MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#else
#define _LFG_DebugPopup(pszMessage, pszTitle) {MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#endif

#else   /* no debugging - _LFG_DEBUG not defined */

#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2)
#define _LFG_DebugString(pszMessage, pszParameter)
#define _LFG_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _LFG_DEBUG" */

#endif  /* #else part of "#ifdef _CONSOLE"  */

ui32 LFG_EXPORT_FN LFG_LibraryLoad(void);
ui32 LFG_EXPORT_FN LFG_LibraryUnload(void);
ui32 LFG_EXPORT_FN LFG_CriticalSectionBegin(void);
ui32 LFG_EXPORT_FN LFG_CriticalSectionEnd(void);

#elif defined _LFG_DOS32 /*================================================*/

/*
_LFG_DOS32(ForBrief)
--------------------
*/
#if !defined _CDA_DOS32
#define _CDA_DOS32
#endif

#ifndef _DOS32
#define _DOS32
#endif
#ifdef __WATCOMC__
#define _FG_GRAPHICS       /* Watcom command length problems */
#define _LFG_DEBUG
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
#define _LFG_Malloc(h, x) malloc(x)
#define _LFG_Free(h, x)   free(x)


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
#ifdef __GNUC__
#define LFG_EXPORT_FN
#define LFG_EXPORT_PFN *
#else
#define LFG_EXPORT_FN   __cdecl
#define LFG_EXPORT_PFN  __cdecl*
#endif


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _LFG_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _LFG_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _LFG_DEBUG

/* _LFG_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("LFG_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _LFG_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _LFG_DebugString(pszMessage, pszParameter) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  wsprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  OutputDebugString( _szWork ); }

/* _LFG_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _LFG_DebugPopup(pszMessage, pszTitle) {::MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#else
#define _LFG_DebugPopup(pszMessage, pszTitle) {MessageBox(NULL,pszMessage,pszTitle,MB_OK);}
#endif

#else   /* no debugging - _LFG_DEBUG not defined */

#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2)
#define _LFG_DebugString(pszMessage, pszParameter)
#define _LFG_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _LFG_DEBUG" */

#elif defined _LFG_LINUX /*================================================*/

/*
_LFG_LINUX(ForBrief)
--------------------
*/
#if !defined _CDA_LINUX
#define _CDA_LINUX
#endif

/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* must be before stdio.h for MS Windows */
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
#define _LFG_Malloc(h, x) malloc(x)
#define _LFG_Free(h, x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
typedef int             i32;
typedef unsigned int   ui32;
typedef int             m16;
typedef unsigned int   mu16;
typedef int             m32;
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
#define LFG_EXPORT_FN
#define LFG_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _LFG_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _LFG_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _LFG_DEBUG

/* _LFG_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("LFG_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _LFG_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _LFG_DebugString(pszMessage, pszParameter) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( stderr, _szWork ); }

/* _LFG_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _LFG_DebugPopup(pszMessage, pszTitle)
#else
#define _LFG_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _LFG_DEBUG not defined */

#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2)
#define _LFG_DebugString(pszMessage, pszParameter)
#define _LFG_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _LFG_DEBUG" */

#elif defined _LFG_MACOSX /*================================================*/

/*
_LFG_MACOSX(ForBrief)
--------------------
*/
#if !defined _CDA_MACOSX
#define _CDA_MACOSX
#endif

/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* must be before stdio.h for MS Windows */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#if !defined __MWERKS__
#import <CoreFoundation/CoreFoundation.h>
#endif

/* 2. Malloc/free macros
 * ---------------------
 */
#define _LFG_Malloc(h, x) LFG_Malloc32((h), (x))
#define _LFG_Free(h, x)   LFG_Free32((h), (x))


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
#define LFG_EXPORT_FN
#define LFG_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _LFG_ErrPrint(szMessage, szTitle) {CDA_DRV_ErrPrint(szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _LFG_Assert(c) assert((c))


/* 7. Debug Macros
 * ---------------
 */
#ifdef _LFG_DEBUG

/* _LFG_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("LFG_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _LFG_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _LFG_DebugString(pszMessage, pszParameter) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( stderr, _szWork ); }

/* _LFG_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _LFG_DebugPopup(pszMessage, pszTitle)
#else
#define _LFG_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _LFG_DEBUG not defined */

#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2)
#define _LFG_DebugString(pszMessage, pszParameter)
#define _LFG_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _LFG_DEBUG" */

ui32 LFG_EXPORT_FN LFG_CriticalSectionBegin(void);
ui32 LFG_EXPORT_FN LFG_CriticalSectionEnd(void);

#elif defined _LFG_VXWORKS /*================================================*/

/*
_LFG_VXWORKS(ForBrief)
--------------------
*/
#if !defined _CDA_VXWORKS
#define _CDA_VXWORKS
#endif

/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* must be before stdio.h for MS Windows */
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
#define _LFG_Malloc(h, x) malloc(x)
#define _LFG_Free(h, x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
typedef int             i32;
typedef unsigned int   ui32;
typedef int             m16;
typedef unsigned int   mu16;
typedef int             m32;
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
#define LFG_EXPORT_FN
#define LFG_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _LFG_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _LFG_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _LFG_DEBUG

/* _LFG_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("LFG_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _LFG_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _LFG_DebugString(pszMessage, pszParameter) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( stderr, _szWork ); }

/* _LFG_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _LFG_DebugPopup(pszMessage, pszTitle)
#else
#define _LFG_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _LFG_DEBUG not defined */

#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2)
#define _LFG_DebugString(pszMessage, pszParameter)
#define _LFG_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _LFG_DEBUG" */

#elif defined _LFG_QNX4 /*================================================*/

/*
_LFG_QNX4(ForBrief)
--------------------
*/
#if !defined _CDA_QNX4
#define _CDA_QNX4
#endif

/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* must be before stdio.h for MS Windows */
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
#define _LFG_Malloc(h, x) malloc(x)
#define _LFG_Free(h, x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
typedef int             i32;
typedef unsigned long  ui32;
typedef int             m16;
typedef unsigned long  mu16;
typedef int             m32;
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
#define LFG_EXPORT_FN
#define LFG_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _LFG_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _LFG_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _LFG_DEBUG

/* _LFG_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("LFG_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _LFG_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _LFG_DebugString(pszMessage, pszParameter) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( stderr, _szWork ); }

/* _LFG_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _LFG_DebugPopup(pszMessage, pszTitle)
#else
#define _LFG_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _LFG_DEBUG not defined */

#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2)
#define _LFG_DebugString(pszMessage, pszParameter)
#define _LFG_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _LFG_DEBUG" */

#elif defined _LFG_QNX6 /*================================================*/

/*
_LFG_QNX6(ForBrief)
--------------------
*/
#if !defined _CDA_QNX6
#define _CDA_QNX6
#endif

/* 1. Include Files
 * ----------------
 */
#include <stdarg.h>     /* must be before stdio.h for MS Windows */
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
#define _LFG_Malloc(h, x) malloc(x)
#define _LFG_Free(h, x)   free(x)


/* 3. General type definitions
 * ---------------------------
 */
#ifndef _ASL_TYPEDEFS
typedef char             i8;
typedef unsigned char   ui8;
typedef short           i16;
typedef unsigned short ui16;
typedef int             i32;
typedef unsigned int   ui32;
typedef int             m16;
typedef unsigned int   mu16;
typedef int             m32;
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
#define LFG_EXPORT_FN
#define LFG_EXPORT_PFN *


/* 5. Error handler macro - used by default error handler
 * ------------------------------------------------------
 */
#define _LFG_ErrPrint(szMessage, szTitle) {printf("\nERROR - %s: %s\n", szTitle, szMessage);}


/* 6. Assert Macros
 * ----------------
 */
#define _LFG_Assert assert


/* 7. Debug Macros
 * ---------------
 */
#ifdef _LFG_DEBUG

/* _LFG_Debug: Outputs a long word as part of a debug string.
   ---------- */
#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2) \
  { printf("LFG_Debug (%s) - %s: 0x%02lx (%02ld) : 0x%02lx (%02ld)\n", szFnName, pszMessage, (ui32) dwParameter1, (ui32) dwParameter1, (ui32) dwParameter2, (ui32) dwParameter2); }

/* _LFG_DebugString: Outputs a string as part of a debug string.
   --------------- */
#define _LFG_DebugString(pszMessage, pszParameter) { \
  char _szWork[LFG_DEBUG_STRING_LEN]; char _szParamString[LFG_DEBUG_STRING_LEN]; strcpy(_szWork, szFnName); \
  strcat(_szWork, " - "); strcat(_szWork, pszMessage); \
  sprintf(_szParamString, " %s\r", (ui32) pszParameter); \
  strcat(_szWork, _szParamString); \
  printf( stderr, _szWork ); }

/* _LFG_DebugPopup: Outputs a debug string in a popup.
   -------------- */
#ifdef __cplusplus
#define _LFG_DebugPopup(pszMessage, pszTitle)
#else
#define _LFG_DebugPopup(pszMessage, pszTitle)
#endif

#else   /* no debugging - _LFG_DEBUG not defined */

#define _LFG_Debug(pszMessage, dwParameter1, dwParameter2)
#define _LFG_DebugString(pszMessage, pszParameter)
#define _LFG_DebugPopup(pszMessage, pszTitle)

#endif  /* #else part of "#ifdef _LFG_DEBUG" */

#else

#error You have a missing target environment directive (e.g. _LFG_WIN32)

#endif

#endif /* _LFG_OS_H_ */

