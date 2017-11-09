/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP09
 * Description : High level include file for LFG structures / API etc.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * This file is the only include file a user (or higher level library) needs 
 * to include in their application in order to use the LFG library.
 * The main definition in this file is the LFG structure (top level) and the 
 * LFG Logical structure. The LFG Logical structure defines the logical view
 * of the hardware and is included in the LFG (Frame Grabber Info) structure 
 * which contains, as well as this logical view (as set by the user), a 
 * physical view (a structure representing the actual hardware registers).
 *
 * Note this file is theoretically independent of the actual hardware 
 * implementation. (The hardware registers are defined in lfg_hw.h.)
 *
 * Each mode or setting has an associated magic number with it - this is very
 * useful in trapping accidental user errors such as setting the ...
 *
 * Note that any "single shot" registers (or boolean flags etc) are reset 
 * by the library to reflect the same action as the hardware.
 *
 * Compiler Directives
 * -------------------
 * _LFG_VIRGIN_LFG  - If defined allows the DOS compiled code to ignore the 
 *                    the subvendor subsystem IDs and open the board.
 *
 * _LFG_GENERIC_878 - Allows a generic 878 card to work.
 * 
 * _LFG_CDA_NOW     - Forces CDA non-cached in release builds.
 *                    (_LFG_DEBUG still forces non-cached as well).
 *
 * _LFG_CM04        - Configures GPIO22 & 23 as inputs for CM04, 15-Dec-08.
 *
 ****************************************************************************
 */

#ifndef _LFG_API_H_
#define _LFG_API_H_

#define _LFG_CM04 1


/* LFG Include Files
 * -----------------
 * This file is the only include file that an application would include.
 * It includes various other required include files. The only one a user may
 * need to edit is the xx_os.h one regarding specific include files (for a
 * different compiler for example).
 * The library source uses xx_all.h which pulls in all the includes.
 */
#if defined _LFG_MACOSX && !defined _LFG_MACOSX_FLAT && !defined __MWERKS__
#import <lfglib/lfg_os.h>     /* Needed for operating system defines    */
#import <lfglib/lfg_hw.h>     /* Needed for physical register structure */
#import <lfglib/lfg_err.h>    /* Needed for error interpretation        */
#import <lfglib/lfg_shrd.h>
#import <cdalib/cda_buf.h>
#import <cdalib/cda_api.h>    /* Needed so user can view PCI info if required */
#import <cdalib/cda_pci.h>    /* Same as above for cda_api.h */
#else
#include <lfg_os.h>     /* Needed for operating system defines    */
#include <lfg_hw.h>     /* Needed for physical register structure */
#include <lfg_err.h>    /* Needed for error interpretation        */
#include <lfg_shrd.h>
#include <cda_buf.h>
#include <cda_api.h>    /* Needed so user can view PCI info if required */
#include <cda_pci.h>    /* Same as above for cda_api.h */
#endif

/* LFG Library Revision Level (see also "File History" above)
 * ----------------------------------------------------------
 * Composed of major.minor.sub-minor.  e.g. 0x12015 = v1.20 Rev.15.
 * The sub-minor should be rolled on for every interim/customer release.
 * The minor and major rev levels should be rolled on as part of a major
 * release that includes thorough testing.
 * Note that hex coding is used - this is so that it is easy to extract
 * the three fields.
 *
LFG_Library_Revision_Level(ForBrief)
 */
#define LFG_LIBRARY_REV_LEVEL ((ui32) 0x20001)
#define LFG_LIB_REV_LEVEL      LFG_LIBRARY_REV_LEVEL 
#define LFG_LIBRARY_REV_STR    "2.0.1.0\0"
#define LFG_LIB_FILEVERSION     2,0,1,0 

#define LFG_MAX_CARDS         ((ui32) 64)

#ifdef _LFG_DOS32
#define LFG_DOSX86_PAGE_SIZE  ((ui32) 4096)  /* Used for RISC array size */
#define LFG_MAX_RISC_ADDRESS  (((LFG_DOSX86_PAGE_SIZE*2)/4)-1)   /* 4 bytes per 32 bit instruction */
#elif !defined _LFG_WIN32 && !defined _LFG_LINUX && !defined _LFG_MACOSX && !defined _LFG_VXWORKS && !defined _LFG_QNX4 && !defined _LFG_QNX6
#error OS dependent page size definition required (probably!).
#endif

/* LFG Logical Structure
 * ---------------------
 * This is the basic low level logical structure that reflects the register
 * settings.  It is used within the internal LFG structure.
 *
struct_tLFG_Logical(ForBrief)
 */
struct tLFG_Logical
{
   ui32 fForceRewrite;   /* Used to force re-write of all registers from logical structure */
   ui32 fUpdateRiscCode; /* Single shot register, used primarily internal to the library   */

  /* Logical view of hardware (logical representation of HW registers)
   * -----------------------------------------------------------------
   */
   ui32 fPolledDrivenCallback; /* So that non-interrupt driven callbacks may be used       */
   ui32 dwEvents;         /* Callback events - ORable bit flags */

   ui32 dwVideoSrc;       /* Video source via mux                    */
   ui32 dwVideoMode;      /* High level video acquisition mode       */

   ui32 f50Hz;            /* TRUE for 50Hz video, FALSE for 60Hz     */
   ui32 dwVideoDecodeStd; /* Colour PAL/NTSC / mono                  */
   ui32 dwVideoDecodeStdActual; /* As above, but cannot include AUTO */
   i32  nHorzStart;       /* In non-scaled pixels pixels */
   i32  nVertStart;       /* In non-scaled pixels pixels */
   float fsHorzScaling;   /* 1.0 = full res, 0.5 = Subx2 etc */
   float fsVertScaling;   /* 1.0 = full res, 0.5 = Subx2 etc */
   ui32 fVertFieldAlign;  /* For subx2,4 50/60Hz to stop vertical field jitter     */
   ui32 fReInterlace;     /* Used by RISC code to re-interlace or not  */
   i32  nImageWidth;      /* In pixels (output/scaled pixels) */
   i32  nImageHeight;     /* In pixels (output/scaled pixels) */
   ui32 dwFieldsToAcquire;/* Selects acquisition of field 1, 2 or both */
   ui32 nBytesPerPixel;   /* Set by LFG_SetRegPixelFormat()            */
   i32  nBytesPerLine;

   ui32 dwPixelFormat;    /* Auto, Y8, YUV422 etc                      */
   ui32 dwPixelFormatActual; /* As above, but will not contain AUTO    */

   ui32 fColorBars;       /* TRUE enables color bars                 */
   ui32 fLumaNotchOn;     /* Luma notch filter at colour sub-carrier */
   ui32 fRemoveGamma;     /* Removes Gamma correction that may be applied at the camera */
   ui32 fFullRange;       /* Selects 0..255, rather than 16..253     */
   ui32 dwCoring;         /* Maps luma values below 0, 8, 16 or 32 to 0 (or 16 if not full range!) */

   ui8  bBrightness;      /* 0..255, 128 = default */
   ui8  bContrast;        /* 0..255, 128 = default */
   ui8  bColor;           /* 0..255, 128 = default */
   ui8  bHueShift;        /* For NTSC only, 0..255 represents +90 to -90 degrees resultant colour shift */

   ui32 dwTriggerMode;    /* Level, edge etc */
   ui32 dwEndian;         /* 16 bit word or byte swap or both      */

   ui32 fLed1_On;         /* TRUE puts LED1 (in the rear panel) on */
   ui32 fI2cExtEnable;    /* Enable external I2C access */
   ui32 fEeWriteEnable;   /* Allow write accesses to the on-board EEPROM */
   ui32 dwIo0;            /* Input, output and level */
   ui32 dwIo1;            /* Input, output and level */
   ui32 dwIo2;            /* Input, output and level */

   i32  nOneEveryNImages; /* For temporal sub-sampling - e.g. 1 every 10 */
   i32  nNumImages;       /* For sequences/swinging buffers etc */

   i32 fSsSwResetChip;  /* Single shot - does a chip software reset - note */
                        /* only resets video decoder and scaler registers! */
   i32 fSsResetFieldCount; /* Single shot - resets field count register "dwFieldCount" */


  /* Logical Status Settings
   * -----------------------
   * Derived from reading hardware status registers using 
   * LFG_SetAndGet(...LFG_READ)
   */
   ui32 fVideoPresent;       /* TRUE indicates video present on the selected input */
   ui32 fHLock;              /* TRUE indicates achieved horizonal video lock */

   ui32 fIo0_InHi;           /* TRUE indicates I/O 0 is high */
   ui32 fIo1_InHi;           /* etc ... */
   ui32 fIo2_InHi;

   ui32 dwCurrentField;      /* Current field being decoded */
   ui32 fFieldTransition;    /* Set to TRUE when dwCurrentField changes  */

   ui32 dwFieldCount;           /* Current count of fields acquired (0..255) */
   ui32 fFieldCountTransition;  /* Set to TRUE when dwFieldCount changes     */

   /* Internal management */
   ui32 dwLastField;         /* Internal used to determine fFieldTransition */
   ui32 dwLastFieldCount;    /* Internal used to determine fFieldCountTransition */
};


/* LFG Info Structure
 * ------------------
 * This structure is a representation of the state of the software and 
 * hardware.
 * It is split into three main sections:
 * 1. Overall status and control information that is not directly represented
 *    by the hardware.  This includes the error handler etc.
 * 2. A shadow representing the logical state of the hardware. This logical
 *    shadow is tested against to save time if there is no mode change.
 * 3. A shadow representing the physical hardware registers. These get 
 *    updated as a result of testing and updating the logical shadow.
 *
 * See also the notes for LFG_Open.
 *
struct_tLFG(ForBrief)
 */
struct tLFG
{
  /* Overall software library variables (not directly related to HW settings)
   * ------------------------------------------------------------------------
   */
   /* Hardware Info */
   ui32 dwDeviceType;        /* PCI or ISA */
   ui32 dwDeviceAddr;        /* PCI device number/auto or ISA base address, 0 = auto, else 1..N for slot number */
   ui32 wPciDeviceID;        /* PCI Device ID */
   ui32 wPciVendorID;        /* PCI Vendor ID */
   ui32 wPciSubVendorID;     /* PCI subsystem vendor ID */
   ui32 wPciSubSystemID;     /* PCI subsystem ID */
   ui32 wFunctionNumber;     /* PCI functional unit */
   ui32 wPciCommandReg;      /* Base address mask - re: Memory/IO mapped */
   ui8  bPciLatencyTimer;    /* PCI device latency timer value           */
   ui32 dwBaseAddrMask;      /* PCI command word to write (ORed in)      */

   ui32 hCard;               /* Back pointer needed by event handler   */
   ui32 hCdaDevice;          /* Handle to CDA driver                   */
   struct tCDA *psCDA;       /* Provide access to CDA driver structure */

   /* Status Information */
   ui32 dwLibIdMagic;        /* Used to check valid structure */
   ui32 dwLibraryRevLevel;   /* Software library revision level - see lfg_api.h */
   ui32 dwDriverRevLevel;    /* Driver/lower library revision level   */
   ui32 dwErrCode;           /* To record last error                  */
   ui32 dwOtherLibErrCode;   /* To record error if from different library (e.g. driver) */

   /* Internal management */
   ui32 fInUse;              /* Indicates if this structure is in use */
   ui32 dwMode;              /* Overall mode - selects PLD type       */
   ui32 dwPldType;           /* PLD to download - governed by dwMode  */
   void (*pFnErrorHandler)(ui32, const char*, ui32, const char*); /* Pointer to error handler function */
   void (*pFnUserEventHandler)(ui32, ui32, ui32, void*); /* Pointer to user event handler function */
   void* pvUserEventContext;
   i32  fFirstRiscI;         /* Used so that the first (start up) RISCI interrupt is ignored */
   i32  nImageCount;         /* Used with callback and nOneEveryNImages to implement temporal sub-sampling */

   struct tCDA_Buffer sCdaBufferVid;   /* Buffer structure for DMA'ed video data */

   struct tCDA_Buffer sCdaBufferRisc;  /* Buffer structure used for RISC code space */
   ui32 dwRiscFirstPageIndex;  /* 0 or 1 indicating offset to first complete page (containing Risc "working" code start) */
   ui32 dwRiscLastPageIndex;   /* Index into last used page for RISC code */
   ui32 dwRiscPageIndex;       /* Roving/current page index            */
   ui32 dwRiscBytesSoFarThisPage; /* Bytes used in current memory page */
   i32  nRiscStartOffset;      /* Offset, in bytes, from start of CDA Risc code buffer to first instruction (start of "working" code space) */
   ui32 *pdwRiscStartLog;      /* Logical pointer to start of "working" Risc code space */
   ui32 *pdwRiscStartLog2nd;   /* Logical pointer to start of "2nd" image */
   ui32 *pdwRiscCode;          /* Roving logical pointer to Risc code (used during Risc code generation) */
   ui32 dwRiscStartPhys;       /* Physical address of Risc code start    */
   ui32 fLive;                 /* TRUE indicates acquisition in progress */
   ui32 fTriggerThisImage;     /* Use to record a trigger within last DMA'ed image */

  /* Shadow of logical representation for internal representation
   * ------------------------------------------------------------
   * The user will have his own PHD structure. See LFG_Open for details
   * about the Logical-Physical architecture.
   */
   struct tLFG_Logical sLog;

  /* Shadow of physical hardware registers
   * -------------------------------------
   * Note the actual values/variables are in the hardware.
   */
   struct tLFG_PhysicalRegs sReg;
}; 


/*
 * LFG Magic Numbers - for logical settings
 * ---------------------------------------
 * "Magic" numbers that are unique for each mode setting - which means that
 * errors are conveniently trapped if a function is called with the wrong
 * parameter.  This is a bit like enums but more efficient.
 *
 * Note the error code is defined here too so that it is also unique (it is
 * used in lfg_err.h). It must be 2 bytes in the most significant position in
 * a 32 bit word.
 *
 * The comments after the magic numbers below explain in which structure the
 * magic numbers are in.
 *
LFG_MAGICs(ForBrief)
 */
#define LFG_LIB_ID              ((ui32) 0x1F000000) /* 1F is like LF - get it? */
#define LFG_LIB_ID_MASK         ((ui32) 0xFF000000)
#define LFG_MAGIC_MASK          ((ui32) 0xFFFF0000)
#define LFG_MAGIC_ERROR         ((ui32) LFG_LIB_ID | ( 0 << 16)) /* MUST be "| 0" */
#define LFG_MAGIC_DRIVER_MSG    ((ui32) LFG_LIB_ID | ( 1 << 16)) /* Driver communication */
#define LFG_MAGIC_DEVICE        ((ui32) LFG_LIB_ID | ( 2 << 16)) /* Parameters for device select in tLFG */
#define LFG_MAGIC_OPEN          ((ui32) LFG_LIB_ID | ( 3 << 16)) /* Flags parameter to library open command */
#define LFG_MAGIC_SET_AND_GET   ((ui32) LFG_LIB_ID | ( 4 << 16)) /* struct tLFG */
#define LFG_MAGIC_MODE          ((ui32) LFG_LIB_ID | ( 5 << 16)) /* struct tLFG */
#define LFG_MAGIC_PLD_TYPE      ((ui32) LFG_LIB_ID | ( 6 << 16)) /* struct tLFG */
#define LFG_MAGIC_PLD_ALGORITHM ((ui32) LFG_LIB_ID | ( 7 << 16)) /* struct tLFG_PldInfo */
#define LFG_MAGIC_SETANDGET_OPTIONS ((ui32) LFG_LIB_ID | ( 8 << 16)) /* LFG_SetAndGet() */
#define LFG_MAGIC_VMODE         ((ui32) LFG_LIB_ID | ( 9 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_VID_STD       ((ui32) LFG_LIB_ID | (10 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_VID_SRC       ((ui32) LFG_LIB_ID | (11 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_FIELDS        ((ui32) LFG_LIB_ID | (12 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_PIXEL_FORMAT  ((ui32) LFG_LIB_ID | (13 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_GET_PTR_OPTIONS ((ui32) LFG_LIB_ID | (14 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_CORING        ((ui32) LFG_LIB_ID | (15 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_TRIGGER       ((ui32) LFG_LIB_ID | (16 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_ENDIAN        ((ui32) LFG_LIB_ID | (17 << 16)) /* struct tLFG_Logical */
#define LFG_MAGIC_IO            ((ui32) LFG_LIB_ID | (18 << 16)) /* struct tLFG_Logical */


/* Top level API related #defines
 * ==============================
 */
/*
LFG_dwDeviceType(ForBrief)
 *
 * dwDeviceType (struct tLFG)
 * ------------
 * See LFG_Open() in lfg_api.c for description of open options.
 */
#define LFG_DEVICE_PCI       (LFG_MAGIC_DEVICE | (ui32) 0x0001)
#define LFG_DEVICE_ISA       (LFG_MAGIC_DEVICE | (ui32) 0x0002)


/*
LFG_SetAndGet_Options(ForBrief)
 *
 * The LFG_SetAndGet options are bitwise ORable.
 */
#define LFG_WRITE             (LFG_MAGIC_SETANDGET_OPTIONS | (ui32) 0x0001)
#define LFG_WRITE_REG_ONLY    (LFG_MAGIC_SETANDGET_OPTIONS | (ui32) 0x0002)
#define LFG_READ              (LFG_MAGIC_SETANDGET_OPTIONS | (ui32) 0x0004)

/*
LFG_GetPointer(ForBrief)
 */
#define LFG_PTR_LFG_STRUCTURE (LFG_MAGIC_GET_PTR_OPTIONS | (ui32) 1)


/* Definitions/options for overall LFG structure
 * ---------------------------------------------
Definitions_For_struct_tLFG(ForBrief)
 */

/* dwMode (struct tLFG)
 * ------
 * Overall mode that controls which PLD is downloaded.
 */
#define LFG_MODE_CTRL_RUNTIME  (LFG_MAGIC_MODE | (ui32) 1)
#define LFG_MODE_CTRL_TEST1    (LFG_MAGIC_MODE | (ui32) 2)

/* dwPldType (struct tLFG)
 * ---------
 */
#define LFG_PLD_CTRL_RUNTIME   (LFG_MAGIC_PLD_TYPE | (ui32) 1)
#define LFG_PLD_CTRL_TEST1     (LFG_MAGIC_PLD_TYPE | (ui32) 2)



/* Logical definitions/options for each logical structure member
 * -------------------------------------------------------------
LOGICAL_DEFINITIONS_For_struct_tLFG_Logical(ForBrief)
 */

/* dwEvents
 * --------
 * Note bitwise ORable.
 */
#define LFG_EVENT_NEW_IMAGE          ((ui32) 0x0001)
#define LFG_EVENT_TRIGGER            ((ui32) 0x0002)
#define LFG_EVENT_TRIGGER_THIS_IMAGE ((ui32) 0x0004)
#define LFG_EVENT_VSYNC              ((ui32) 0x0008)

/* These are special software generated ones from the status register - 
 * END_SEQUENCE refers to the condition when NEW_IMAGE is set, but the
 * image just DMA'ed is the last one in the sLog.nNumImages sequence.
 */
#define LFG_EVENT_END_SEQUENCE ((ui32) 0x1000)


/* dwVideoSrc (struct tLFG_Logical)
 * ----------
 * CM = Composite/Mono input.
 * YC = S-Video input (luma switched, chroma input fixed).
 */
#define LFG_VID_SRC_CM0  (LFG_MAGIC_VID_SRC | (ui32) 1)
#define LFG_VID_SRC_CM1  (LFG_MAGIC_VID_SRC | (ui32) 2)
#define LFG_VID_SRC_CM2  (LFG_MAGIC_VID_SRC | (ui32) 3)
#define LFG_VID_SRC_CM3  (LFG_MAGIC_VID_SRC | (ui32) 4)
#define LFG_VID_SRC_YC0  (LFG_MAGIC_VID_SRC | (ui32) 5)
#define LFG_VID_SRC_YC1  (LFG_MAGIC_VID_SRC | (ui32) 6)
#define LFG_VID_SRC_YC2  (LFG_MAGIC_VID_SRC | (ui32) 7)
#define LFG_VID_SRC_YC3  (LFG_MAGIC_VID_SRC | (ui32) 8)


/* dwVideoMode  (struct tLFG_Logical)
 * -----------
 */
#define VMODE_USER      ((ui32) 0x0001)  /* 0 for direct mode */
#define VMODE_AUTO      ((ui32) 0x0002)  /* 0 for direct mode */
#define VMODE_50HZ      ((ui32) 0x0004)
#define VMODE_60HZ      ((ui32) 0x0008)
#define VMODE_MONO      ((ui32) 0x0010)  /* 0 for colour      */
#define VMODE_PAL       ((ui32) 0x0020)
#define VMODE_NTSC      ((ui32) 0x0040)
#define VMODE_SECAM     ((ui32) 0x0080)  /* Refers to 4.406MHz subcarrier - see lfg_mode.c to change to 4.425MHz */
#define VMODE_F1        ((ui32) 0x0100)
#define VMODE_F2        ((ui32) 0x0200)
#define VMODE_F12       ((ui32) VMODE_F1 | VMODE_F2)
#define VMODE_REINT     ((ui32) 0x0400)  /* 0 for no re-interlacing */
#define VMODE_BITS_SIZE_MASK ((ui32) 0xF000)
#define VMODE_768x576   ((ui32) 0x1000)
#define VMODE_768x288   ((ui32) 0x2000)
#define VMODE_384x288   ((ui32) 0x3000)
#define VMODE_192x144   ((ui32) 0x4000)
#define VMODE_640x480   ((ui32) 0x5000)
#define VMODE_640x240   ((ui32) 0x6000)
#define VMODE_320x240   ((ui32) 0x7000)
#define VMODE_160x120   ((ui32) 0x8000)


#define LFG_VMODE_USER          (LFG_MAGIC_VMODE | VMODE_USER )
#define LFG_VMODE_AUTO          (LFG_MAGIC_VMODE | VMODE_AUTO )
#define LFG_50_MONO_768x576_F12 (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_768x576 | VMODE_F12 | VMODE_REINT)
#define LFG_50_MONO_640x480_F12 (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_640x480 | VMODE_F12 | VMODE_REINT)
#define LFG_50_MONO_768x288_F1  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_768x288 | VMODE_F1 )
#define LFG_50_MONO_768x288_F2  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_768x288 | VMODE_F2 )
#define LFG_50_MONO_768x288_F12 (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_768x288 | VMODE_F12)
#define LFG_50_MONO_384x288_F1  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_384x288 | VMODE_F1 )
#define LFG_50_MONO_320x240_F1  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_320x240 | VMODE_F1 )
#define LFG_50_MONO_384x288_F2  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_384x288 | VMODE_F2 )
#define LFG_50_MONO_384x288_F12 (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_384x288 | VMODE_F12)
#define LFG_50_MONO_192x144_F1  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_192x144 | VMODE_F1 )
#define LFG_50_MONO_160x120_F1  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_160x120 | VMODE_F1 )
#define LFG_50_MONO_192x144_F2  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_192x144 | VMODE_F2 )
#define LFG_50_MONO_192x144_F12 (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_MONO | VMODE_192x144 | VMODE_F12)

#define LFG_50_PAL_768x576_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_768x576 | VMODE_F12 | VMODE_REINT)
#define LFG_50_PAL_640x480_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_640x480 | VMODE_F12 | VMODE_REINT)
#define LFG_50_PAL_768x288_F1   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_768x288 | VMODE_F1 )
#define LFG_50_PAL_768x288_F2   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_768x288 | VMODE_F2 )
#define LFG_50_PAL_768x288_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_768x288 | VMODE_F12)
#define LFG_50_PAL_384x288_F1   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_384x288 | VMODE_F1 )
#define LFG_50_PAL_384x288_F2   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_384x288 | VMODE_F2 )
#define LFG_50_PAL_384x288_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_384x288 | VMODE_F12)
#define LFG_50_PAL_192x144_F1   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_192x144 | VMODE_F1 )
#define LFG_50_PAL_192x144_F2   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_192x144 | VMODE_F2 )
#define LFG_50_PAL_192x144_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_PAL  | VMODE_192x144 | VMODE_F12)

#define LFG_60_MONO_640x480_F12 (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_640x480 | VMODE_F12 | VMODE_REINT)
#define LFG_60_MONO_640x240_F1  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_640x240 | VMODE_F1 )
#define LFG_60_MONO_640x240_F2  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_640x240 | VMODE_F2 )
#define LFG_60_MONO_640x240_F12 (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_640x240 | VMODE_F12)
#define LFG_60_MONO_320x240_F1  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_320x240 | VMODE_F1 )
#define LFG_60_MONO_320x240_F2  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_320x240 | VMODE_F2 )
#define LFG_60_MONO_320x240_F12 (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_320x240 | VMODE_F12)
#define LFG_60_MONO_160x120_F1  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_160x120 | VMODE_F1 )
#define LFG_60_MONO_160x120_F2  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_160x120 | VMODE_F2 )
#define LFG_60_MONO_160x120_F12 (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_MONO | VMODE_160x120 | VMODE_F12)

#define LFG_60_NTSC_640x480_F12 (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_640x480 | VMODE_F12 | VMODE_REINT)
#define LFG_60_NTSC_640x240_F1  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_640x240 | VMODE_F1 )
#define LFG_60_NTSC_640x240_F2  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_640x240 | VMODE_F2 )
#define LFG_60_NTSC_640x240_F12 (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_640x240 | VMODE_F12)
#define LFG_60_NTSC_320x240_F1  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_320x240 | VMODE_F1 )
#define LFG_60_NTSC_320x240_F2  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_320x240 | VMODE_F2 )
#define LFG_60_NTSC_320x240_F12 (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_320x240 | VMODE_F12)
#define LFG_60_NTSC_160x120_F1  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_160x120 | VMODE_F1 )
#define LFG_60_NTSC_160x120_F2  (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_160x120 | VMODE_F2 )
#define LFG_60_NTSC_160x120_F12 (LFG_MAGIC_VMODE | VMODE_60HZ | VMODE_NTSC | VMODE_160x120 | VMODE_F12)

#define LFG_50_SECAM_768x576_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_768x576 | VMODE_F12 | VMODE_REINT)
#define LFG_50_SECAM_640x480_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_640x480 | VMODE_F12 | VMODE_REINT)
#define LFG_50_SECAM_768x288_F1   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_768x288 | VMODE_F1 )
#define LFG_50_SECAM_768x288_F2   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_768x288 | VMODE_F2 )
#define LFG_50_SECAM_768x288_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_768x288 | VMODE_F12)
#define LFG_50_SECAM_384x288_F1   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_384x288 | VMODE_F1 )
#define LFG_50_SECAM_384x288_F2   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_384x288 | VMODE_F2 )
#define LFG_50_SECAM_384x288_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_384x288 | VMODE_F12)
#define LFG_50_SECAM_192x144_F1   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_192x144 | VMODE_F1 )
#define LFG_50_SECAM_192x144_F2   (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_192x144 | VMODE_F2 )
#define LFG_50_SECAM_192x144_F12  (LFG_MAGIC_VMODE | VMODE_50HZ | VMODE_SECAM  | VMODE_192x144 | VMODE_F12)


/* dwVideoDecodeStd (struct tLFG_Logical)
 * ----------------
 */
#define LFG_VID_STD_AUTO       (LFG_MAGIC_VID_STD | (ui32) 1)
#define LFG_VID_STD_NO_VIDEO   (LFG_MAGIC_VID_STD | (ui32) 2)
#define LFG_VID_STD_NTSC       (LFG_MAGIC_VID_STD | (ui32) 3)  /* "-M" USA and many others */
#define LFG_VID_STD_NTSC_J     (LFG_MAGIC_VID_STD | (ui32) 4)  /* Japan (No Pedestal, IRE=0) */
#define LFG_VID_STD_PAL        (LFG_MAGIC_VID_STD | (ui32) 5)  /* UK, Ireland, S.Africa */
#define LFG_VID_STD_PAL_M      (LFG_MAGIC_VID_STD | (ui32) 6)  /* Brazil, 60Hz */
#define LFG_VID_STD_PAL_NC     (LFG_MAGIC_VID_STD | (ui32) 7)  /* Argentina */
#define LFG_VID_STD_PAL_N      (LFG_MAGIC_VID_STD | (ui32) 8)  /* Paraguay, Uruguay */
#define LFG_VID_STD_SECAM_4406 (LFG_MAGIC_VID_STD | (ui32) 9)  /* Eastern Europe, France, Middle East */
#define LFG_VID_STD_SECAM_4250 (LFG_MAGIC_VID_STD | (ui32)10)  /* Eastern Europe, France, Middle East */
#define LFG_VID_STD_MONO       (LFG_MAGIC_VID_STD | (ui32)11)

/* nVertStart, nHorzStart  (struct tLFG_Logical)
 * ----------------------
 */
#define LFG_50HZ_HORZ_START ((i32) 154)
#define LFG_50HZ_VERT_START ((i32) 32)
#define LFG_60HZ_HORZ_START ((i32) 114)
#define LFG_60HZ_VERT_START ((i32) 28)

#define LFG_50HZ_LINES_PER_FRAME ((i32) 625)  /* Used for bounds checking */
#define LFG_60HZ_LINES_PER_FRAME ((i32) 525)
 
/* dwFieldsToAcquire  (struct tLFG_Logical)
 * -----------------
 */
#define LFG_FIELDS_F1          (LFG_MAGIC_FIELDS | (ui32) 1)
#define LFG_FIELDS_F2          (LFG_MAGIC_FIELDS | (ui32) 2)
#define LFG_FIELDS_BOTH        (LFG_MAGIC_FIELDS | (ui32) 3)

/* nImageWidth, nImageHeight  (struct tLFG_Logical)
 * -------------------------
 */
#define LFG_50HZ_X1_WIDTH  ((i32) 768)
#define LFG_50HZ_X1_HEIGHT ((i32) 576)
#define LFG_60HZ_X1_WIDTH  ((i32) 640)
#define LFG_60HZ_X1_HEIGHT ((i32) 480)

/* dwPixelFormat  (struct tLFG_Logical)
 * -------------
 * Auto selects Y8 or YUV422 according to dwVideoDecodeStd (colour/mono).
 * _D = Error diffusion/dithered.
 */
#define LFG_PIXEL_FORMAT_AUTO    (LFG_MAGIC_PIXEL_FORMAT | (ui32)  1)
#define LFG_PIXEL_FORMAT_Y8      (LFG_MAGIC_PIXEL_FORMAT | (ui32)  2)
#define LFG_PIXEL_FORMAT_YUV422  (LFG_MAGIC_PIXEL_FORMAT | (ui32)  3)
#define LFG_PIXEL_FORMAT_RGB8_D  (LFG_MAGIC_PIXEL_FORMAT | (ui32)  4)
#define LFG_PIXEL_FORMAT_RGB15   (LFG_MAGIC_PIXEL_FORMAT | (ui32)  5)
#define LFG_PIXEL_FORMAT_RGB15_D (LFG_MAGIC_PIXEL_FORMAT | (ui32)  6)
#define LFG_PIXEL_FORMAT_RGB16   (LFG_MAGIC_PIXEL_FORMAT | (ui32)  7)
#define LFG_PIXEL_FORMAT_RGB16_D (LFG_MAGIC_PIXEL_FORMAT | (ui32)  8)
#define LFG_PIXEL_FORMAT_RGB24   (LFG_MAGIC_PIXEL_FORMAT | (ui32)  9)
#define LFG_PIXEL_FORMAT_RGB32   (LFG_MAGIC_PIXEL_FORMAT | (ui32) 10)

/* dwCoring  (struct tLFG_Logical)
 * --------
 */
#define LFG_CORING_0    (LFG_MAGIC_CORING | (ui32) 1)
#define LFG_CORING_8    (LFG_MAGIC_CORING | (ui32) 2)
#define LFG_CORING_16   (LFG_MAGIC_CORING | (ui32) 3)
#define LFG_CORING_32   (LFG_MAGIC_CORING | (ui32) 4)

/* dwTriggerMode  (struct tLFG_Logical)
 * -------------
 */
#define LFG_TRIGGER_RISING_EDGE  (LFG_MAGIC_TRIGGER | (ui32) 1)
#define LFG_TRIGGER_FALLING_EDGE (LFG_MAGIC_TRIGGER | (ui32) 2)
#define LFG_TRIGGER_LEVEL_HIGH   (LFG_MAGIC_TRIGGER | (ui32) 3)
#define LFG_TRIGGER_LEVEL_LOW    (LFG_MAGIC_TRIGGER | (ui32) 4)

/* dwEndian  (struct tLFG_Logical)
 * --------
 */
#define LFG_ENDIAN_NO_SWAP    (LFG_MAGIC_ENDIAN | (ui32) 1)
#define LFG_ENDIAN_BYTE_SWAP  (LFG_MAGIC_ENDIAN | (ui32) 2)
#define LFG_ENDIAN_WORD_SWAP  (LFG_MAGIC_ENDIAN | (ui32) 3)
#define LFG_ENDIAN_BW_SWAP    (LFG_MAGIC_ENDIAN | (ui32) 4)

/* dwIo0, dwIo1, dwIo2  (struct tLFG_Logical)
 * -------------------
 */
#define LFG_IO_INPUT  (LFG_MAGIC_IO | (ui32) 1)
#define LFG_IO_OUT_HI (LFG_MAGIC_IO | (ui32) 2)
#define LFG_IO_OUT_LO (LFG_MAGIC_IO | (ui32) 3)

/* dwCurrentField  (struct tLFG_Logical - status settings)
 * ----------------
 */
#define LFG_FIELD_1  ((ui32) 1)
#define LFG_FIELD_2  ((ui32) 2)


/*
 * General Macros
 * --------------
 * _LFG_GetAndCheckpsLFG - Macro to get card handle pointer and check all is OK.
 * _LFG_RoundUp          - Rounds up x to y alignment.
 * _LFG_MagicCheck       - Checks a magic number using the library magic mask.
 *
 * Note as the error handler is stored in the structure, if hDevice is 
 * invalid we cannot call the error handler - thus we must just return
 * LFG_ERROR_BAD_PARAM silently.
 *
LOGICAL_MACROS(ForBrief)
_LFG_GetAndCheckpsLFG(ForBrief)
_LFG_RoundUp(ForBrief)
_LFG_MagicCheck(ForBrief)
 */
#define _LFG_GetAndCheckpsLFG(hCard, psLFG) \
   { if ( (hCard) > LFG_MAX_CARDS ) \
     { \
        return(LFG_ERROR_BAD_PARAM); \
     } \
     else \
     { \
        psLFG = &gsLFG_Internal[hCard]; \
        _LFG_Assert(psLFG != NULL); \
        if ( !psLFG->fInUse ) \
        { \
           return(LFG_ERROR_BAD_PARAM); \
        } \
     } \
   }

#define _LFG_MagicCheck(Setting, MagicNum) \
   { if ( ((Setting) & LFG_MAGIC_MASK) != (MagicNum) ) \
     { \
        _LFG_Proe(LFG_ERROR_BAD_PARAM, "Incorrect setting/parameter (failed 'magic number' check)"); \
     } \
   }

#define _LFG_RoundUp(x,y)   ((x + (y-1)) & ~(y-1))


/* Macros to manipulate 32 bit flags
 * ---------------------------------
_LFG_FlagMacros(ForBrief)
*/
#define _LFG_FlagSet(FlagsReg, Flag)    ( (FlagsReg) |= (Flag) )
#define _LFG_FlagClr(FlagsReg, Flag)    ( (FlagsReg) &= ~(Flag) )
#define _LFG_IsFlagSet(FlagsReg, Flag)  ( ((FlagsReg) & (Flag)) == (Flag) )
#define _LFG_IsFlagClr(FlagsReg, Flag)  ( ((FlagsReg) & (Flag)) == 0 )


/*
 * Parallel Port Debugging Macro
 * -----------------------------
 * Used as signalling output for timing debugging. Note not supported under
 * Win32 or Watcom (in fact only works under Symantec DOS32).
 *
_LFG_DEBUG_ParallelPort(ForBrief)
 */
#if defined _LFG_DEBUG && !defined __WATCOMC__ && !defined _LFG_WIN32
#define _LFG_DEBUG_ParallelPort(dwBitPos, nPolarity)                /*lint -save -e774 */ \
  { if ((nPolarity) == 1) gbDebugPortShadow |= (1 << (dwBitPos));  /*lint -restore */ \
    else gbDebugPortShadow &= ~(1 << (dwBitPos)); \
        _outp( 0x378, (char) gbDebugPortShadow ); } \
    /* NOTE: Need simple CDA function to do this type of thing -
     * _CDA_Roe( CDA_RegWrite(hCard / CDA_NO_HANDLE, CDA_8BIT | CDA_NOW, 0x378, gbDebugPortShadow) ); }
     */
#else
#define _LFG_DEBUG_ParallelPort(dwBitPos, nPolarity)
#endif

/* Moved to end of file for gcc4 - Rich - 24-Oct-2006 */
#if defined _LFG_MACOSX && !defined __MWERKS__ && !defined _LFG_MACOSX_FLAT
#import <lfglib/lfg_pro.h>    /* Needed for prototypes                  */
#else
#include <lfg_pro.h>    /* Needed for prototypes                  */
#endif

#endif  /* _LFG_API_H_ */

