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
 * This file is the only CDA related file that a higher level library needs 
 * in order for its driver layer to compile. See also comments below by
 * "CDA Include Files".
 * It contains the necessary structure definitions and prototypes required
 * by the user.
 *
 * Compiler Directives
 * -------------------
 * _CDA_SIMULATED_OS_LAYER
 *    Used to illustrate how a "real" OS layer would work.
 *
 * _CDA_NO_INTERRUPTS
 *    When defined disables the use of interrupts. When _VRP_NO_INTERRUPTS
 *    is defined, _CDA_NO_INTERRUPTS is automatically defined for convenience.
 *
 * CDA_WIN32_NO_PRIORITY
 *    If defined (and it now is by default, Feb-02), does not raise the 
 *    priority of the interrupt thread.
 *
 ****************************************************************************
 */

#ifndef _CDA_API_H_
#define _CDA_API_H_


/* CDA Include Files
 * -----------------
 * This file is the only include file that an application would include.
 * It includes various other required include files. The only one a user may
 * need to edit is the xx_os.h one regarding specific include files (for a
 * different compiler for example).
 * The library source uses xx_all.h which pulls in all the includes.
 */

#if defined _CDA_MACOSX && !defined _CDA_MACOSX_FLAT && !defined __MWERKS__
/* Header files are located in cdalib.framework */
#import <cdalib/cda_os.h>
#import <cdalib/cda_buf.h>
#import <cdalib/cda_lib.h>
#import <cdalib/cda_err.h>
#import <cdalib/cda_pci.h>
#import <cdalib/cda_macx.h>
#else
/* Look for header files in pre-processor include path */
#include <cda_os.h>
#include <cda_buf.h>
#include <cda_lib.h>
#include <cda_err.h>
#include <cda_pci.h>
#if defined _CDA_MACOSX
#include <cda_macx.h>
#endif
#endif

#if defined _CDA_MACOSX && !defined __MWERKS__
#import <mach/mach.h>
#import <mach/mach_error.h>
#import <CoreFoundation/CoreFoundation.h>
#import <IOKit/IOKitLib.h>
#endif


/* CDA Library Revision Level (and summary history)
 * ------------------------------------------------
 * Composed of major.minor.sub-minor.  e.g. 0x012015 = v1.20 Rev.15.
 * The sub-minor should be rolled on for every interim/customer release.
 * The minor and major rev levels should be rolled on as part of a major
 * release that includes thorough testing.
 * The major rev is always rolled on if there is an API change that is not
 * backwards compatible (hence adding a function need not incur a major
 * rev roll on).
 * Note that hex coding is used - this is so that it is easy to extract
 * the three fields.
 *
CDA_SW_Revision_Level(ForBrief)
 */
#define CDA_LIB_REV_LEVEL    ((ui32) 0x40002) /* So it matches PHX_LIB_CDA_REV_REQ              */
#define CDA_LIBRARY_REV_STR  "4.0.2.0\0"      /* For win32 dll - matches PHX SDK release number */
#define CDA_LIB_FILEVERSION   4,0,2,0         /* For win32 dll - matches PHX SDK release number */

#define CDA_MAX_DEVICES         ((ui32) 64)

/* Prototype of initialisation function
 */
void CDA_Initialize();

/* Maximum "handle" number the user can use to act as a reference to the
 * device.
 */
#define CDA_MAX_USER_HANDLES         ((ui32) 1000)
#define CDA_BITS_USER_HANDLE_IN_USE  ((ui32) 0x80000000)  /* Used to flag in use */


/* CDA Structure
 * -------------
 * This structure is a used to pass information to and from the user's
 * CDA driver layer as well as general state information.
 *
struct_tCDA(ForBrief)
 */
struct tCDA
{
   /* Hardware Device Info */
   ui32 dwDeviceType;        /* PCI/ISA */
   ui32 dwDeviceAddr;        /* ISA address or PCI slot selector */
   ui32 dwBasePhysAddr;      /* Physical PCI address or ISA address - filled in by CDA library */
   void *pBaseVirtAddr;      /* Virtual address the software can use */
   ui16 wPciDeviceID;        /* PCI Device ID     */
   ui16 wPciVendorID;        /* PCI Vendor ID     */
   ui16 wPciSubVendorID;     /* PCI Sub-Vendor ID */
   ui16 wPciSubSystemID;     /* PCI Sub-System ID */
   ui16 wFunctionNumber;     /* PCI functional unit, 0.. */
   ui32 dwBaseAddrMask;      /* Base address mask - re: For PCI Memory/IO mapped */
   ui16 wPciCommandReg;      /* PCI command word to write (ORed in)      */
   ui8  bPciLatencyTimer;    /* PCI device latency timer value           */

   ui32 dwInterruptLine;     /* OS assigned interrupt line */
   ui32 dwPciConfigSpace[CDA_PCI_CONFIG_SIZE_BYTES/4];  /* Filled in by driver */

#if defined _PHX_CDA
   ui32 dwVirtualSleepTime;  /* For development using a virtual device (i.e. no physical hardware) */
#endif

#if defined _CDA_LINUX
   pthread_t threadID;       /* Thread that delivers pseudo-IRQ events */
   pthread_mutex_t DeviceMutex;
   ui32 hLdiDevice;          /* Handle to local storage */

#elif defined _CDA_WIN32
   void *hDrvDev;            /* Handle to Windows NT driver (HANDLE = void*) */ 
   HANDLE hThread;           /* Thread that delivers pseudo-IRQ events */
   ui32 uDeviceIndex;        /* Device index used by library during open */
   HANDLE hEvent;            /* Event signalled by pseudo-IRQ */
   CRITICAL_SECTION sCriticalSection;  /* For CDA_CriticalSectionBegin() */

#elif defined _CDA_DOS32 || defined _CDA_ARM7
   ui32 hLdiDevice;          /* Handle to DOS driver */

#elif defined _CDA_MACOSX && !defined __MWERKS__
   io_connect_t DevicePort;
   pthread_t threadID;
   pthread_mutex_t DeviceMutex;
   pthread_mutexattr_t DeviceMutexAttr;
   ui32 dwCount;                     /* Used by EventThread to indicate it has started */
#elif defined _CDA_VXWORKS
   struct sLocalDeviceInfo *pLDI;    /* No user/kernel separation in VxWorks */
   int hThread;                      /* Thread which delivers pseudo-IRQ events */
   SEM_ID hCdaLock;                  /* Used as a mutex */
   int iLockTimeoutTicks;            /* Maximum number of ticks to wait before failing a lock operation */
#elif defined _CDA_QNX4
   void *pStack;
   ui32 dwDeviceIndex;
   sem_t semCdaLock;
   ui8 wDummy[3];
   int pidEventThread;
   char *szEventThreadArgs[3];
   char szHandle[32];
#elif defined _CDA_QNX6
   ui32 dwDeviceIndex;
   pthread_mutex_t mutexCdaLock;
   pthread_mutex_t mutexSendRegLock;
   pthread_t EventThread;
   pthread_attr_t EventThreadAttr;
   struct sched_param EventThreadSchedParam;
   ui32 dwBusSnoopingSupported;
   char pDriverBuffer[CDA_MAX_MESSAGE];
#endif


   /* Status Information */
   ui32 dwOsType;            /* Operating system type and version      */
   ui32 dwOsMajor;           /* Operating system major version         */
   ui32 dwOsMinor;           /* Operating system minor version         */
   ui32 dwLibIdMagic;        /* Used to check valid CDA structure      */
   ui32 dwLibRevLevel;       /* Revision level of CDA library          */
   ui32 dwDriverRevLevel;    /* Revision level of CDA driver           */
   volatile ui32 dwErrCode;  /* To record last error                   */
   volatile ui32 dwOtherLibErrCode; /* To record error if from different library (e.g. driver) */

   /* Internal Management */
   ui32 fInUse;              /* Indicates if this structure is in use        */
   ui32 hDevice;             /* Handle to this instance of the CDA structure */
   ui32 hCallersHandle;      /* Handle to callers handle                     */
   void (*pFnErrorHandler)(ui32, const char*, ui32, const char*); /* Pointer to error handler function */
   void (*pFnEventHandler)(ui32, ui32, ui32, void*); /* Pointer to event handler function */
   void* pvEventContext;

#if defined _PHX_CDA
   CDA_UserLockedBuffInfo *psCdaUserLockedBufferFirst;   /* First CDA User Locked buffer instance (head) in linked list */
#endif

   /* Register structure & data list to pass to driver */
   volatile struct tCDA_RegList sRegList;
};


/*
 * CDA Magic Numbers - for logical settings
 * ----------------------------------------
 * "Magic" numbers that are unique for each mode setting - which means that
 * errors are conveniently trapped if a function is called with the wrong
 * parameter.
 * Note the error code is defined here too so that it is also unique (it is
 * used in cda_err.h). It must be 2 bytes in the most significant position 
 * in a 32 bit word.
 *
CDA_MAGICs(ForBrief)
 */
#define CDA_LIB_ID               ((ui32) 0xCD000000)
#define CDA_LIB_ID_MASK          ((ui32) 0xFF000000)
#define CDA_MAGIC_MASK           ((ui32) 0xFFFF0000)
#define CDA_MAGIC_ERROR          ((ui32) CDA_LIB_ID | ( 0 << 16)) /* MUST be "| 0"    */
#define CDA_MAGIC_REG_TYPE       ((ui32) CDA_LIB_ID | ( 1 << 16)) /* User API related (and used internally for register list) */
#define CDA_MAGIC_DEVICE         ((ui32) CDA_LIB_ID | ( 2 << 16)) /* Parameters for device select in tCDA */
#define CDA_MAGIC_OPEN           ((ui32) CDA_LIB_ID | ( 3 << 16)) /* Flags parameter to library open      */
#define CDA_MAGIC_OS_TYPE        ((ui32) CDA_LIB_ID | ( 4 << 16)) /* Operating system type                */
#define CDA_MAGIC_DEVICE_CONTROL ((ui32) CDA_LIB_ID | ( 5 << 16)) /* Our special DeviceControl function   */
#define CDA_MAGIC_MEMORY_ALLOC   ((ui32) CDA_LIB_ID | ( 6 << 16)) /* Our special MemoryAlloc function     */

/* Top level API related #defines
 * ==============================
 */
/*
CDA_dwDeviceType(ForBrief)
 *
 * dwDeviceType (struct tCDA)
 * ------------
 */
#define CDA_DEVICE_VIRTUAL   (CDA_MAGIC_DEVICE | (ui32) 0x0001)
#define CDA_DEVICE_PCI       (CDA_MAGIC_DEVICE | (ui32) 0x0002)
#define CDA_DEVICE_ISA       (CDA_MAGIC_DEVICE | (ui32) 0x0003)


/*
CDA_dwDeviceAddr(ForBrief)
 *
 * dwDeviceAddr (struct tCDA)
 * ------------
 * 1 = PCI device 1, 2 = PCI device 2, etc.
 */
#define CDA_DEVICE_AUTO      0    /* 0 = Next available PCI device */


/*
CDA_dwOsType(ForBrief)
 *
 * dwOsType (struct tCDA)
 * --------
 */
#define CDA_OS_DOS32       (CDA_MAGIC_OS_TYPE | (ui32) 0x0001)  /* DOS32 = Symantec */
#define CDA_OS_LINUX       (CDA_MAGIC_OS_TYPE | (ui32) 0x0002)
#define CDA_OS_WIN95       (CDA_MAGIC_OS_TYPE | (ui32) 0x0003)
#define CDA_OS_WIN98       (CDA_MAGIC_OS_TYPE | (ui32) 0x0004)
#define CDA_OS_WINNT       (CDA_MAGIC_OS_TYPE | (ui32) 0x0005)  /* NT4 or below */
#define CDA_OS_WIN2000     (CDA_MAGIC_OS_TYPE | (ui32) 0x0006)  /* NT 5.0 */
#define CDA_OS_ARM7        (CDA_MAGIC_OS_TYPE | (ui32) 0x0007)  /* Hopefully not Cirrus 7211/VRP specific */
#define CDA_OS_MACOSX      (CDA_MAGIC_OS_TYPE | (ui32) 0x0008)  /* MacOS X on PowerPC */
#define CDA_OS_WINME       (CDA_MAGIC_OS_TYPE | (ui32) 0x0009)
#define CDA_OS_WINXP       (CDA_MAGIC_OS_TYPE | (ui32) 0x0010)  /* NT 5.1 */
#define CDA_OS_UNKNOWN     (CDA_MAGIC_OS_TYPE | (ui32) 0x0011)


/*
CDA_DeviceControl_Parameters(ForBrief)
 *
 */
#define CDA_PCI_CONFIG_RESTORE       (CDA_MAGIC_DEVICE_CONTROL | (ui32) 0x0001)
#define CDA_EVENT_HANDLER_SIGNAL     (CDA_MAGIC_DEVICE_CONTROL | (ui32) 0x0002)
#define CDA_INTERRUPT_HANDLER_ENABLE (CDA_MAGIC_DEVICE_CONTROL | (ui32) 0x0003)
#define CDA_PHX_PORT_NAMES           (CDA_MAGIC_DEVICE_CONTROL | (ui32) 0x0004)
#define CDA_PCI_GET_UNIQUE_ID        (CDA_MAGIC_DEVICE_CONTROL | (ui32) 0x0005)
#define CDA_EVENT_PUT                (CDA_MAGIC_DEVICE_CONTROL | (ui32) 0x0006)


/*
CDA_MemoryAlloc_Parameters(ForBrief)
 *
 */
#define CDA_MEMORYALLOC_BELOW_4GB    (CDA_MAGIC_MEMORY_ALLOC | (ui32) 0x0001)
#define CDA_MEMORYALLOC_READ         (CDA_MAGIC_MEMORY_ALLOC | (ui32) 0x0002)
#define CDA_MEMORYALLOC_WRITE        (CDA_MAGIC_MEMORY_ALLOC | (ui32) 0x0004)


/*
 * General Macros
 * --------------
 * _CDA_GetAndCheckpsCDA - Macro to get card handle pointer and check all is OK.
 * _CDA_RoundUp          - Rounds up x to y alignment.
 * _CDA_MagicCheck       - Checks a magic number using the library magic mask.
 *
 * Note as the error handler is stored in the structure, if hDevice is 
 * invalid we cannot call the error handler - thus we must just return
 * CDA_ERROR_BAD_PARAM silently.
 *
LOGICAL_MACROS(ForBrief)
_CDA_GetAndCheckpsCDA(ForBrief)
_CDA_GetpsCDA(ForBrief)
_CDA_RoundUp(ForBrief)
_CDA_MagicCheck(ForBrief)
 */
#define _CDA_GetAndCheckpsCDA(hDevice, psCDA) \
   { if ( (hDevice) > CDA_MAX_DEVICES ) \
     { \
        return(CDA_ERROR_BAD_PARAM); \
     } \
     else \
     { \
        psCDA = &gsCDA_Internal[hDevice]; \
        _CDA_Assert(psCDA != NULL); \
     } \
   }

#define _CDA_GetpsCDA(hDevice, psCDA) \
   { if ( (hDevice) > CDA_MAX_DEVICES ) \
     { \
        return; \
     } \
     else \
     { \
        psCDA = &gsCDA_Internal[hDevice]; \
        _CDA_Assert(psCDA != NULL); \
     } \
   }

#define _CDA_MagicCheck(Setting, MagicNum) \
   { if ( ((Setting) & CDA_MAGIC_MASK) != MagicNum ) \
     { \
        _CDA_Proe(CDA_ERROR_BAD_PARAM, "Incorrect setting/parameter (failed 'magic number' check)"); \
     } \
   }

#define _CDA_RoundUp(x,y)   ((x + (y-1)) & ~(y-1))


/* Macros to manipulate 32 bit flags
 * ---------------------------------
_CDA_FlagMacros(ForBrief)
 *
 * Note: It's usually better to use !_IsFlagSet as _IsFlagClr in practice
 * may not work if upper bits are used as a magic number check. See 
 * CDA_WriteReg for an example.
 */
#define _CDA_FlagSet(FlagsReg, Flag)    ( (FlagsReg) |= (Flag) )
#define _CDA_FlagClr(FlagsReg, Flag)    ( (FlagsReg) &= ~(Flag) )
#define _CDA_IsFlagSet(FlagsReg, Flag)  ( ((FlagsReg) & (Flag)) == (Flag) )
/* See note above #define _CDA_IsFlagClr(FlagsReg, Flag)  ( ((FlagsReg) & (Flag)) == 0 )*/


/* Bit Mask Macros etc
 * -------------------
 * Macros to set and mask (register) bits.
 *
_CDA_BIT_MACROS(ForBrief)
_CDA_BitsMask(ForBrief)
_CDA_BitsClear(ForBrief)
_CDA_DoBitsMatch(ForBrief)
_CDA_IsBitsSet(ForBrief)
_CDA_IsBitsClr(ForBrief)
 */
#define _CDA_BitsSet(Reg, Bits)      ((Reg) |= (Bits))
#define _CDA_BitsClr(Reg, Bits)      ((Reg) &= (~(Bits)))
#define _CDA_BitsMask(Reg, Mask)     ((Reg) &= (~(Mask)))
#define _CDA_IsBitsSet(Reg, Bits)    (((Reg) & (Bits)) == (Bits))
#define _CDA_IsBitsClr(Reg, Bits)    (((Reg) & (Bits)) == 0)

/* Parallel Port Debugging Macro
 * -----------------------------
 * Used as signalling output for timing debugging.
 * Note: Only supported under DOS (for now), Colin, 8-Sep-99.
 *
_CDA_DEBUG_ParallelPort(ForBrief)
 */
#if defined _CDA_DEBUG && defined _CDA_DOS32
#define _CDA_DEBUG_ParallelPort(dwBitPos, nPolarity) CDA_DRV_ParallelPort(1, (dwBitPos), (nPolarity))
#else
#define _CDA_DEBUG_ParallelPort(dwBitPos, nPolarity)
#endif


/* Moved here to remove the "array type has incomplete element type" warning.
 * tCDA is used in cda_pro.h before it is defined above!
 */
#if defined _CDA_MACOSX && !defined _CDA_MACOSX_FLAT && !defined __MWERKS__
#import <cdalib/cda_pro.h>
#else
#include <cda_pro.h>
#endif


#endif  /* _CDA_API_H_ */

