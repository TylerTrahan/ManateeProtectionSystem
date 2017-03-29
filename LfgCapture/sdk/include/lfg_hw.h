/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP09
 * Description : Hardware register structure and definitions.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * This file contains the hardware definitions - including a structure 
 * representing all hardware registers that is used as a shadow and actual
 * register addresses (offset from base address).
 *
 ****************************************************************************
 */

#ifndef _LFG_HW_H_
#define _LFG_HW_H_


#if !defined _LFG_MACOSX_KERNEL
/* NOTE: This file should ideally be two files. The device driver required 
 * some of the definitions later on in this file (register definitions), 
 * specifically for the interrupt service routine. However, inclusion of all 
 * the CDA structure definitions causes problems with kernel name space 
 * pollution under MacOS X.  Hence, conditional compilation is used to chop 
 * the file in two, CLR, 27-Sep-01.
 */

/* LFG Hardware Register Structure
 * -------------------------------
 * These registers mirror the hardware registers of the board and are used
 * by the logical-physical architecture.
 *
 * Types of Register
 * -----------------
 * - dwDstatus is basically used as read only although the lower 2 bits may 
 *   be written to.
 * - dwTest is read only.
 * - dwCapCtl starts and stops the acquisition (amongst a few other things).
 * - dwIntStatus and dwIntMask are the interrupt status and mask registers.
 * - dwRiscCounter and dwRiscStartAddr control the target DMA areas etc.
 * - dwGpioDmaCtrl starts the DMA (amongst various other things).
 * - dwI2C and dwfI2CDataToSend control the I2C port.
 * - All other registers and registers setup in advance of acquisition.
 *
 * Note the PCI configuration registers are dealt with within the CDA driver
 * and set up as part of the call to LFG_DRV_Open().
 *
 * Note: The readback registers do not need to be volatile as the only
 * separate interrupt driven function is LFG_EventHandler() that does not
 * write into the structure.
 *
struct_tLFG_PhysicalRegs(ForBrief)
 */
struct tLFG_PhysicalRegs
{
   ui8 bDstatus;      /* Status (8 bits)                    */
   ui8 bIform;        /* Input Format (8 bits)              */
   ui8 bTdec;         /* Temporal Decimation (8 bits)       */

   ui8 bEcrop;        /* MSB Cropping - even field (8 bits) */
   ui8 bOcrop;        /* MSB Cropping - odd field (8 bits)  */

   ui8 bEVdelayLo;    /* Vertical Delay  - even - lo byte (8 bits) */
   ui8 bOVdelayLo;    /* Vertical Delay  - odd  - lo byte (8 bits) */
   ui8 bEVactiveLo;   /* Vertical Active - even - lo byte (8 bits) */
   ui8 bOVactiveLo;   /* Vertical Active - odd  - lo byte (8 bits) */

   ui8 bEHdelayLo;    /* Horizontal Delay  - even - lo byte (8 bits) */
   ui8 bOHdelayLo;    /* Horizontal Delay  - odd  - lo byte (8 bits) */
   ui8 bEHactiveLo;   /* Horizontal Active - even - lo byte (8 bits) */
   ui8 bOHactiveLo;   /* Horizontal Active - odd  - lo byte (8 bits) */

   ui8 bEHscaleHi;    /* Horizontal Scaling - even - hi byte (8 bits) */
   ui8 bOHscaleHi;    /* Horizontal Scaling - odd  - hi byte (8 bits) */
   ui8 bEHscaleLo;    /* Horizontal Scaling - even - lo byte (8 bits) */
   ui8 bOHscaleLo;    /* Horizontal Scaling - odd  - lo byte (8 bits) */

   ui8 bBright;       /* Brightness (8 bits) */

   ui8 bEcontrol;     /* Misc Control - even field (8 bits) */
   ui8 bOcontrol;     /* Misc Control - odd  field (8 bits) */

   ui8 bContrastLo;   /* Luma gain/contrast - lo byte (8 bits)           */
   ui8 bSatULo;       /* Saturation (chroma gain) - U - Lo byte (8 bits) */
   ui8 bSatVLo;       /* Saturation (chroma gain) - V - Lo byte (8 bits) */
   ui8 bHue;          /* Hue Control (8 bits)            */
   ui8 bESCloop;      /* SC Loop Control - even (8 bits) */
   ui8 bOSCloop;      /* SC Loop Control - odd  (8 bits) */

   ui8 bWhiteCrushUp; /* AGC for proportional sync level (8 bits) */

   ui8 bOform;        /* Output Format (8 bits) */

   ui8 bEVscaleHi;    /* Vertical scaling - even - hi (8 bits) */
   ui8 bOVscaleHi;    /* Vertical scaling - odd  - hi (8 bits) */
   ui8 bEVscaleLo;    /* Vertical scaling - even - lo (8 bits) */
   ui8 bOVscaleLo;    /* Vertical scaling - odd  - lo (8 bits) */

   ui8 bTest;         /* Test register (8 bits) */

   ui8 bAdelay;       /* AGC Delay  (8 bits)  */
   ui8 bBdelay;       /* Burst Delay (8 bits) */

   ui8 bADCinterface; /* ADC Interface  (8 bits) */

   ui8 bEVTC;         /* Video Timing Control - even (8 bits) */
   ui8 bOVTC;         /* Video Timing Control - odd  (8 bits) */

   ui8 bSreset;       /* Software reset (8 bits) */
   ui8 bWhiteCrushDn; /* AGC for proportional sync level */

   ui8 bTimGenLoadByte;  /* (8 bits) */
   ui8 bTimGenCtrl;      /* (8 bits) "TGCTRL" */

   ui8 bVtotalLo;     /* For specifying non-525/625 line input video (8 bits) */
   ui8 bVtotalHi;     /* (8 bits) */

   ui8 bColorFmt;     /* Colour Format  (8 bits) */
   ui8 bColorCtl;     /* Colour Control (8 bits) */
   ui8 bCapCtl;       /* Capture Control (8 bits) */

   ui8 bVBIpackSize;  /* VBI Packet Size  (8 bits) */
   ui8 bVBIpackDel;   /* VBI Packet Delay (8 bits) */

   ui8 bFieldCount;   /* Count of field transitions when capture bit set (8 bits) */

   ui8 bPllRefMultLo; /* PLL Reference Multiplier - Lo byte (8 bits) */
   ui8 bPllRefMultHi; /* PLL Reference Multiplier - Hi byte (8 bits) */
   ui8 bPllXci;       /* PLL pre/post divide & input range (8 bits)  */

   ui8 bDvsiFormat;   /* Digital Video Signal Interface Format (8 bits) */

   ui32 dwIntStatus;    /* Interrupt Status (32 bits) */
   ui32 dwIntMask;      /* Interrupt Mask   (24 bits) */

   ui32 dwRiscStartAddr;  /* RISC program start address (32 bits) */
   ui32 dwRiscCount;      /* RISC counter (RO) (32 bits)          */

   ui32 dwGpioDmaCtrl;    /* GPOI and DMA Control (16 bits) */
   ui32 dwGpioOutEn;      /* GPIO Output Enable Control (24 bits) */
   ui32 dwGpioData;       /* GPIO Data (24 bits) */

   ui32 dwI2C;            /* I2C Data and Control (32 bits) */
   ui32 dwfI2CDataToSend; /* Flag to indicate that there is data to send (NOTE: needed?) */
};


/*
 * Macros to set and mask (register) bits.
 * Note that the macros can also be used for logical settings - for example,
 * the IsBitsSet macro may be used for logical testing of user API bitwise 
 * settings.
 *
_LFG_REGISTER_MACROS(ForBrief)
_LFG_BitsSet(ForBrief)
_LFG_BitsMask(ForBrief)
_LFG_BitsTransferAndMask(ForBrief)
_LFG_BitsClear(ForBrief)
_LFG_DoBitsMatch(ForBrief)
_LFG_IsBitsSet(ForBrief)
_LFG_IsBitsClr(ForBrief)
_LFG_RegWriteAndUpdate_ui8(ForBrief)
_LFG_RegWriteAndUpdate_ui32(ForBrief)
 */
/* 
 * The ~ operator causes a truncation error/warning if Reg refers to a type which 
 * is less bits than int. Compiler warning and error generation must be set accordingly. 
 * This most commonly happens with char operations, but may also happen with short.
 * It may be better to have a BitsClr8 and a BitsMask8 for byte operations.
 */
#define _LFG_BitsSet(Reg, Bits)         ((Reg) |= (Bits))
#define _LFG_BitsClr(Reg, Bits)         ((Reg) &= (~(Bits)))
#define _LFG_BitsMask(Reg, Mask)        ((Reg) &= (~(Mask)))
#define _LFG_DoBitsMatch(Reg, Bits)     (((Reg) & (Bits)) == (Bits))
#define _LFG_IsBitsSet(Reg, Bits)       (((Reg) & (Bits)) == (Bits))
#define _LFG_IsBitsClr(Reg, Bits)       (((Reg) & (Bits)) == 0)
#if defined _LFG_MACOSX
/* 
 * See comment above - MacOS X compiler generates an error, therefore
 * use specific 8 bit definitions for these macros.
 */
#define _LFG_BitsClr8(Reg, Bits)        ((Reg) &= (ui8)(~(Bits)))
#define _LFG_BitsMask8(Reg, Mask)       ((Reg) &= (ui8)(~(Mask)))
#endif

/* Extracts a "Mask"ed bit pattern into Target1 and Target2 - used for status readback in SetAndGet */
#define _LFG_BitsTransferAndMask(Source, Target1, Target2, Mask) \
      { (Target1) &=~(Mask); (Target1) |= ((Source) & (Mask)); (Target2) = (Target1); }

/* Compares dwOldData to dwNewData and if there is a difference it updates
 * dwOldData and writes the value into the register at address dwAddress.
 * dwNewData is the User's structure, dwOldData is the Internal one.
 */
#define _LFG_RegWriteAndUpdate_ui32(hCard, dwAddress, dwNewData, dwOldData) \
         ( ((dwNewData) != (dwOldData)) ? \
           LFG_DRV_RegWrite_ui32((hCard), (dwAddress), (dwOldData=dwNewData)) : LFG_OK )

/* Compares bOldData to bNewData and if there is a difference it updates
 * bOldData and writes the value into the register at address dwAddress.
 * bNewData is the User's structure, bOldData is the Internal one.
 */
#define _LFG_RegWriteAndUpdate_ui8(hCard, dwAddress, bNewData, bOldData) \
         ( ((bNewData) != (bOldData)) ? \
           LFG_DRV_RegWrite_ui8((hCard), (dwAddress), (bOldData=bNewData)) : LFG_OK )

#ifdef _OLD_METHOD_NEVER
#define _LFG_RegUpdateAndClearFlag(hCard, dwAddress, dwData) \
         ((((dwData) & LFG_REG_CHANGED) == LFG_REG_CHANGED) ? \
         LFG_DRV_RegWrite((hCard), (dwAddress), (ui32) ((dwData) &= (~LFG_REG_CHANGED))) : LFG_OK )
#endif

#endif  /* NOTE: End of _LFG_MACOSX_KERNEL (See notes at top) */

/* NOTE: These definitions should only be defined here and not repeated
 * in the drivers.  Currently only MacOS X does this.  At some point the
 * driver duplicated definitions ought to be removed, CLR/HCP, 27-Sep-01.
 */
#if defined _LFG_MACOSX_KERNEL || defined _LFG_VXWORKS || defined _LFG_QNX4 || defined _LFG_QNX6
#define LFG_PCI_DEVICEID_AUDIO 0x0878
#define LFG_PCI_DEVICEID_VIDEO 0x036E
#define LFG_PCI_VENDORID       0x109E
#define LFG_PCI_SUBSYSTEMID    0x8781
#define LFG_PCI_SUBVENDORID    0x4153
#endif


/* PLD Programming Information 
 * ---------------------------
 * See also further down under "Register Addresses".
 * Programming delays in milliseconds.
 */
#define LFG_PLD_DELAY_PROG_REQ_LOW   3
#define LFG_PLD_DELAY_DATA_START     3
#define LFG_PLD_DELAY_STATUS_VALID   3


/* Register Addresses
 * ------------------
Register_Addresses(ForBrief)
 */
#define LFG_REG_ADDR_DSTATUS       ((ui32) 0x000)   /* bDstatus       */
#define LFG_REG_ADDR_IFORM         ((ui32) 0x004)   /* bIform         */
#define LFG_REG_ADDR_TDEC          ((ui32) 0x008)   /* bTdec          */
#define LFG_REG_ADDR_E_CROP        ((ui32) 0x00C)   /* bEcrop         */
#define LFG_REG_ADDR_O_CROP        ((ui32) 0x08C)   /* bOcrop         */
#define LFG_REG_ADDR_E_VDELAY_LO   ((ui32) 0x010)   /* bEVdelayLo     */
#define LFG_REG_ADDR_O_VDELAY_LO   ((ui32) 0x090)   /* bOVdelayLo     */
#define LFG_REG_ADDR_E_VACTIVE_LO  ((ui32) 0x014)   /* bEVactiveLo    */
#define LFG_REG_ADDR_O_VACTIVE_LO  ((ui32) 0x094)   /* bOVactiveLo    */
#define LFG_REG_ADDR_E_HDELAY_LO   ((ui32) 0x018)   /* bEHdelayLo     */
#define LFG_REG_ADDR_O_HDELAY_LO   ((ui32) 0x098)   /* bOHdelayLo     */
#define LFG_REG_ADDR_E_HACTIVE_LO  ((ui32) 0x01C)   /* bEHactiveLo    */
#define LFG_REG_ADDR_O_HACTIVE_LO  ((ui32) 0x09C)   /* bOHactiveLo    */
#define LFG_REG_ADDR_E_HSCALE_HI   ((ui32) 0x020)   /* bEHscaleHi     */
#define LFG_REG_ADDR_O_HSCALE_HI   ((ui32) 0x0A0)   /* bOHscaleHi     */
#define LFG_REG_ADDR_E_HSCALE_LO   ((ui32) 0x024)   /* bEHscaleLo     */
#define LFG_REG_ADDR_O_HSCALE_LO   ((ui32) 0x0A4)   /* bOHscaleLo     */
#define LFG_REG_ADDR_BRIGHT        ((ui32) 0x028)   /* bBright        */
#define LFG_REG_ADDR_E_CONTROL     ((ui32) 0x02C)   /* bEControl      */
#define LFG_REG_ADDR_O_CONTROL     ((ui32) 0x0AC)   /* bOControl      */
#define LFG_REG_ADDR_CONTRAST_LO   ((ui32) 0x030)   /* bContrastLo    */
#define LFG_REG_ADDR_SAT_U_LO      ((ui32) 0x034)   /* bSatULo        */
#define LFG_REG_ADDR_SAT_V_LO      ((ui32) 0x038)   /* bSatVLo        */
#define LFG_REG_ADDR_HUE           ((ui32) 0x03c)   /* bHue           */
#define LFG_REG_ADDR_E_SCLOOP      ((ui32) 0x040)   /* bESCLoop       */
#define LFG_REG_ADDR_O_SCLOOP      ((ui32) 0x0C0)   /* bOSCLoop       */
#define LFG_REG_ADDR_WC_UP         ((ui32) 0x044)   /* bWhiteCrushUp  */
#define LFG_REG_ADDR_OFORM         ((ui32) 0x048)   /* bOFrom         */
#define LFG_REG_ADDR_E_VSCALE_HI   ((ui32) 0x04C)   /* bEVscaleHi     */
#define LFG_REG_ADDR_O_VSCALE_HI   ((ui32) 0x0CC)   /* bOVscaleHi     */
#define LFG_REG_ADDR_E_VSCALE_LO   ((ui32) 0x050)   /* bEVscaleLo     */
#define LFG_REG_ADDR_O_VSCALE_LO   ((ui32) 0x0D0)   /* bOVscaleLo     */
#define LFG_REG_ADDR_TEST          ((ui32) 0x054)   /* bTest          */
#define LFG_REG_ADDR_ADELAY        ((ui32) 0x060)   /* bAdelay        */
#define LFG_REG_ADDR_BDELAY        ((ui32) 0x064)   /* bBdelay        */
#define LFG_REG_ADDR_ADC           ((ui32) 0x068)   /* bADCinterface  */
#define LFG_REG_ADDR_E_VTC         ((ui32) 0x06C)   /* bEVTC          */
#define LFG_REG_ADDR_O_VTC         ((ui32) 0x0EC)   /* bOVTC          */
#define LFG_REG_ADDR_SRESET        ((ui32) 0x07C)   /* bSreset        */
#define LFG_REG_ADDR_WC_DN         ((ui32) 0x078)   /* bWhiteCrushDn  */
#define LFG_REG_ADDR_TGLB          ((ui32) 0x080)   /* bTimGenLoadByte */
#define LFG_REG_ADDR_TGCTRL        ((ui32) 0x084)   /* bTimGenCtrl     */
#define LFG_REG_ADDR_VTOTAL_LO     ((ui32) 0x0B0)   /* bVtotalLo      */
#define LFG_REG_ADDR_VTOTAL_HI     ((ui32) 0x0B4)   /* bVtotalHi      */
#define LFG_REG_ADDR_COLOR_FMT     ((ui32) 0x0D4)   /* bColorFmt      */
#define LFG_REG_ADDR_COLOR_CTL     ((ui32) 0x0D8)   /* bColorCtl      */
#define LFG_REG_ADDR_CAP_CTL       ((ui32) 0x0DC)   /* bCapCtl        */
#define LFG_REG_ADDR_VBI_PACK_SIZE ((ui32) 0x0E0)   /* bVBIpackSize   */
#define LFG_REG_ADDR_VBI_PACK_DEL  ((ui32) 0x0E4)   /* bVBIpackDel    */
#define LFG_REG_ADDR_FCAP          ((ui32) 0x0E8)   /* bFieldCount    */
#define LFG_REG_ADDR_PLL_F_LO      ((ui32) 0x0F0)   /* bPllRefMultLo  */
#define LFG_REG_ADDR_PLL_F_HI      ((ui32) 0x0F4)   /* bPllRefMultHi  */
#define LFG_REG_ADDR_PLL_XCI       ((ui32) 0x0F8)   /* bPllXci        */
#define LFG_REG_ADDR_DVSIF         ((ui32) 0x0FC)   /* bDvsiFormat    */
#define LFG_REG_ADDR_INT_STATUS    ((ui32) 0x100)   /* dwIntStatus     */
#define LFG_REG_ADDR_INT_MASK      ((ui32) 0x104)   /* dwIntMask       */
#define LFG_REG_ADDR_RISC_COUNT    ((ui32) 0x120)   /* dwRiscCounter   */
#define LFG_REG_ADDR_RISC_STRT_ADD ((ui32) 0x114)   /* dwRiscStartAddr */
#define LFG_REG_ADDR_GPIO_DMA_CTL  ((ui32) 0x10C)   /* dwdwGpioDmaCtrl */
#define LFG_REG_ADDR_GPIO_OUT_EN   ((ui32) 0x118)   /* dwGpioOutEn     */
#define LFG_REG_ADDR_GPIO_DATA     ((ui32) 0x200)   /* dwGpioData      */
#define LFG_REG_ADDR_I2C           ((ui32) 0x110)   /* dwI2C           */



/* REGISTER BIT DEFINITIONS
 * ------------------------
Register_Bit_Definitions(ForBrief)
 *
 * Note use of _MODE, _STATUS, _ON and _OFF as part of naming convention.
 * (As well as pre-pended _HMASK, and _HBITS.)
 */
/* TODO: See vc_hw.h for examples */

/* bDstatus
 * --------
 */
#define LFG_HBITS_DSTATUS_VIDEO_PRESENT  ((ui8) 0x80)
#define LFG_HBITS_DSTATUS_HLOC           ((ui8) 0x40)
#define LFG_HBITS_DSTATUS_FIELD2_EVEN    ((ui8) 0x20)
#define LFG_HBITS_DSTATUS_NUML           ((ui8) 0x10)
#define LFG_HBITS_DSTATUS_PLL_NOT_LOCKED ((ui8) 0x04)  /* "PLOCK" */
#define LFG_HBITS_DSTATUS_LOF            ((ui8) 0x02)
#define LFG_HBITS_DSTATUS_COF            ((ui8) 0x01)

/* bIform
 * ------
 */
#define LFG_HMASK_IFORM_MUXSEL     LFG_HBITS_IFORM_MUXSEL_11
#define LFG_HBITS_IFORM_MUXSEL_00  ((ui8) 0x00)
#define LFG_HBITS_IFORM_MUXSEL_01  ((ui8) 0x20)
#define LFG_HBITS_IFORM_MUXSEL_10  ((ui8) 0x40)
#define LFG_HBITS_IFORM_MUXSEL_11  ((ui8) 0x60)

#define LFG_HMASK_IFORM_FORMAT           LFG_HBITS_IFORM_FORMAT_PAL_NC
#define LFG_HBITS_IFORM_FORMAT_AUTO      ((ui8) 0x00)
#define LFG_HBITS_IFORM_FORMAT_NTSC_M    ((ui8) 0x01)
#define LFG_HBITS_IFORM_FORMAT_NTSC_J    ((ui8) 0x02)
#define LFG_HBITS_IFORM_FORMAT_PAL_BDGHI ((ui8) 0x03)
#define LFG_HBITS_IFORM_FORMAT_PAL_M     ((ui8) 0x04)
#define LFG_HBITS_IFORM_FORMAT_PAL_N     ((ui8) 0x05)
#define LFG_HBITS_IFORM_FORMAT_SECAM     ((ui8) 0x06)
#define LFG_HBITS_IFORM_FORMAT_PAL_NC    ((ui8) 0x07)

/* bEcrop, bOcrop
 * --------------
 */
#define LFG_HMASK_VDELAY_MSB   ((ui8) 0xC0)
#define LFG_HMASK_VACTIVE_MSB  ((ui8) 0x30)
#define LFG_HMASK_HDELAY_MSB   ((ui8) 0x0C)
#define LFG_HMASK_HACTIVE_MSB  ((ui8) 0x03)

/* bEcontrol, bOcontrol
 * --------------------
 */
#define LFG_HMASK_LUMA_NOTCH     LFG_HBITS_LUMA_NOTCH_OFF
#define LFG_HBITS_LUMA_NOTCH_ON  ((ui8) 0x00)
#define LFG_HBITS_LUMA_NOTCH_OFF ((ui8) 0x80)

#define LFG_HMASK_ECON_COMP      LFG_HBITS_ECON_YC
#define LFG_HBITS_ECON_COMP      ((ui8) 0x00)
#define LFG_HBITS_ECON_YC        ((ui8) 0x40)

#define LFG_HMASK_LDEC           LFG_HBITS_LDEC_OFF
#define LFG_HBITS_LDEC_ON        ((ui8) 0x00)
#define LFG_HBITS_LDEC_OFF       ((ui8) 0x20)

#define LFG_HMASK_CON_MSB        LFG_HBITS_CON_MSB_HI
#define LFG_HBITS_CON_MSB_HI     ((ui8) 0x04)
#define LFG_HBITS_CON_MSB_LO     ((ui8) 0x00)
#define LFG_HMASK_SATU_MSB       LFG_HBITS_SATU_MSB_HI
#define LFG_HBITS_SATU_MSB_HI    ((ui8) 0x02)
#define LFG_HBITS_SATU_MSB_LO    ((ui8) 0x00)
#define LFG_HMASK_SATV_MSB       LFG_HBITS_SATV_MSB_HI
#define LFG_HBITS_SATV_MSB_HI    ((ui8) 0x01)
#define LFG_HBITS_SATV_MSB_LO    ((ui8) 0x00)

/* bOform
 * ------
 */
#define LFG_HMASK_OFORM_RANGE         LFG_HBITS_OFORM_RANGE_0_255
#define LFG_HBITS_OFORM_RANGE_0_255   ((ui8) 0x80)
#define LFG_HBITS_OFORM_RANGE_16_253  ((ui8) 0x00)

#define LFG_HMASK_OFORM_CORING     LFG_HBITS_OFORM_CORING_32
#define LFG_HBITS_OFORM_CORING_0   ((ui8) 0x00)
#define LFG_HBITS_OFORM_CORING_8   ((ui8) 0x20)
#define LFG_HBITS_OFORM_CORING_16  ((ui8) 0x40)
#define LFG_HBITS_OFORM_CORING_32  ((ui8) 0x60)

/* bEVscaleHi, bOVscaleHi
 * ----------------------
 */
#define LFG_HMASK_VSCALE_FIELD_FILTERING       ((ui8) 0xA0)
#define LFG_HBITS_VSCALE_FIELD_SCALING         ((ui8) 0x00)
#define LFG_HBITS_VSCALE_FRAME_SCALING         ((ui8) 0x20)
#define LFG_HBITS_VSCALE_FIELD_ALIGNED_SCALING ((ui8) 0x80)
#define LFG_HBITS_COMB                         ((ui8) 0x40)
#define LFG_HMASK_VSCALE_HI                    ((ui8) 0x1f)

/* bTest
 * -----
 */
#define LFG_HBITS_TEST_REG_VALUE  ((ui32) 0x01)

/* bAdelay, bBdelay
 * ----------------
 */
#define LFG_HBITS_ADELAY_443  ((ui8) 0x88)  /* 4.43MHz colour subcarrier */
#define LFG_HBITS_ADELAY_358  ((ui8) 0x70)  /* 3.58MHz colour subcarrier */
#define LFG_HBITS_ADELAY_440  ((ui8) 0x87)  /* 4.406MHz colour subcarrier (SECAM) */
#define LFG_HBITS_ADELAY_425  ((ui8) 0x83)  /* 3.425MHz colour subcarrier (SECAM) */

#define LFG_HBITS_BDELAY_443  ((ui8) 0x73)
#define LFG_HBITS_BDELAY_358  ((ui8) 0x5D)
#define LFG_HBITS_BDELAY_440  ((ui8) 0x73)
#define LFG_HBITS_BDELAY_425  ((ui8) 0x6F)

/* bADCinterface
 * -------------
 */
#define LFG_HMASK_C_SLEEP      LFG_HBITS_C_SLEEP_ON
#define LFG_HBITS_C_SLEEP_ON   ((ui8) 0x02)
#define LFG_HBITS_C_SLEEP_OFF  ((ui8) 0x00)

/* bEVTC, bOVTC
 * ------------
 */
#define LFG_HBITS_VFILT_00        ((ui8) 0x00)
#define LFG_HBITS_VFILT_01        ((ui8) 0x01)
#define LFG_HBITS_VFILT_10        ((ui8) 0x10)
#define LFG_HBITS_VFILT_11        ((ui8) 0x11)

/* bSreset
 * -------
 */
#define LFG_HMASK_SWRESET         LFG_HBITS_SWRESET
#define LFG_HBITS_SWRESET         ((ui8) 0x01)
#define LFG_HBITS_NO_SWRESET      ((ui8) 0x00)

/* bTimGenCtrl
 * -----------
 */
#define LFG_HMASK_TIMGEN_TGCKO        LFG_HBITS_TIMGEN_TGCKO_PLL_I
#define LFG_HBITS_TIMGEN_TGCKO_CLKx1  ((ui8) 0x00)
#define LFG_HBITS_TIMGEN_TGCKO_XTAL   ((ui8) 0x20)
#define LFG_HBITS_TIMGEN_TGCKO_PLL    ((ui8) 0x40)
#define LFG_HBITS_TIMGEN_TGCKO_PLL_I  ((ui8) 0x60)  /* Inverted */

#define LFG_HMASK_TIMGEN_TGCKI        LFG_HBITS_TIMGEN_TGCKI_GP_I
#define LFG_HBITS_TIMGEN_TGCKI_XTAL   ((ui8) 0x00)
#define LFG_HBITS_TIMGEN_TGCKI_PLL    ((ui8) 0x08)
#define LFG_HBITS_TIMGEN_TGCKI_GP     ((ui8) 0x10)
#define LFG_HBITS_TIMGEN_TGCKI_GP_I   ((ui8) 0x18)  /* Inverted */

/* bColorFmt
 * ---------
 */
#define LFG_HMASK_COLOR_FMT        ((ui8) 0xFF)
#define LFG_HBITS_COLOR_FMT_RGB32  ((ui8) 0x00)
#define LFG_HBITS_COLOR_FMT_RGB24  ((ui8) 0x11)
#define LFG_HBITS_COLOR_FMT_RGB16  ((ui8) 0x22)
#define LFG_HBITS_COLOR_FMT_RGB15  ((ui8) 0x33)
#define LFG_HBITS_COLOR_FMT_YUV422 ((ui8) 0x44)
#define LFG_HBITS_COLOR_FMT_YUV411 ((ui8) 0x55)
#define LFG_HBITS_COLOR_FMT_Y8     ((ui8) 0x66)
#define LFG_HBITS_COLOR_FMT_RGB8_D ((ui8) 0x77)

/* bColorCtl
 * ---------
 */
#define LFG_HMASK_COLOR_CTRL_BARS   LFG_HBITS_COLOR_CTRL_BARS
#define LFG_HBITS_COLOR_CTRL_BARS   ((ui8) 0x40)
#define LFG_HBITS_COLOR_CTRL_NBARS  ((ui8) 0x00)

#define LFG_HMASK_COLOR_CTRL_RGB_D  LFG_HBITS_COLOR_CTRL_RGB_D
#define LFG_HBITS_COLOR_CTRL_RGB_D  ((ui8) 0x20)
#define LFG_HBITS_COLOR_CTRL_RGB_ND ((ui8) 0x00)

#define LFG_HMASK_COLOR_CTRL_GAMMA        LFG_HBITS_COLOR_CTRL_GAMMA_LEAVE
#define LFG_HBITS_COLOR_CTRL_GAMMA_REMOVE ((ui8) 0x00)
#define LFG_HBITS_COLOR_CTRL_GAMMA_LEAVE  ((ui8) 0x10)

#define LFG_HMASK_COLOR_CTRL_ENDIAN ((ui8) 0x0f)
#define LFG_HBITS_COLOR_CTRL_NOSWAP ((ui8) 0x00)
#define LFG_HBITS_COLOR_CTRL_WSWAP  ((ui8) 0x0c)
#define LFG_HBITS_COLOR_CTRL_BSWAP  ((ui8) 0x03)


/* bCapCtl
 * -------
 */
#define LFG_HBITS_CAPCTL_FIELD1  ((ui8) 0x01)
#define LFG_HBITS_CAPCTL_FIELD2  ((ui8) 0x02)

/* bPllRefMultLo, bPllRefMultHi, bPllXci
 * -------------------------------------
 */
#define LFG_PLLREF_443_LO  ((ui8) 0xF9)   /* 4.43MHz PAL */
#define LFG_PLLREF_443_HI  ((ui8) 0xDC)
#define LFG_PLLXCI_443     ((ui8) 0x8E)

#define LFG_PLLREF_440_LO  ((ui8) 0xCA)   /* 4.406250MHz SECAM */
#define LFG_PLLREF_440_HI  ((ui8) 0x7C)
#define LFG_PLLXCI_440     ((ui8) 0x8E)

#define LFG_PLLREF_425_LO  ((ui8) 0x3F)   /* 4.25MHz SECAM */
#define LFG_PLLREF_425_HI  ((ui8) 0x64)
#define LFG_PLLXCI_425     ((ui8) 0x8E)

/* dwIntStatus, dwIntMask
 * ----------------------
 */
#define LFG_HMASK_INT_MASK          ((ui32) 0x00ffffff)
#define LFG_HBITS_INT_NO_INTERRUPTS ((ui32) 0x0)
#define LFG_HBITS_INT_RISCI         ((ui32) 0x00000800)
#define LFG_HBITS_INT_GPINT         ((ui32) 0x00000200)
#define LFG_HBITS_INT_I2CDONE       ((ui32) 0x00000100)
#define LFG_HBITS_INT_VSYNC         ((ui32) 0x00000002)

#define LFG_HBITS_STATUS_RACK       ((ui32) 0x02000000)
#define LFG_HBITS_STATUS_VSYNC      ((ui32) 0x01000000)


/* dwGpioDmaCtrl
 * -------------
 */
#define LFG_HMASK_GPINT               ((ui32) 0xc000)
#define LFG_HBITS_GPINT_GPINTC_EDGE   ((ui32) 0x8000)
#define LFG_HBITS_GPINT_GPINTC_LEVEL  ((ui32) 0x0000)
#define LFG_HBITS_GPINT_GPINTI_INVERT ((ui32) 0x4000)
#define LFG_HBITS_GPINT_GPINTI_NOINV  ((ui32) 0x0000)
#define LFG_HBITS_DMA_PACKED_4        ((ui32) 0x0000)
#define LFG_HBITS_DMA_PACKED_8        ((ui32) 0x0004)
#define LFG_HBITS_DMA_PACKED_16       ((ui32) 0x0008)
#define LFG_HBITS_DMA_PACKED_32       ((ui32) 0x000c)

#define LFG_HMASK_DMA_RISC_AND_FIFO (LFG_HBITS_DMA_RISC_ENABLE | LFG_HBITS_DMA_FIFO_ENABLE)
#define LFG_HBITS_DMA_RISC_ENABLE     ((ui32) 0x0002)
#define LFG_HBITS_DMA_FIFO_ENABLE     ((ui32) 0x0001)

/* dwGpioOutEn
 * -----------
 * a '1' indicates an output (enabled Bt878 buffers).
 */
#define LFG_HMASK_GPIO_OUT_ENABLE     ((ui32) 0xffffff)  /* Mask out the lot and re-do each time */
#define LFG_HBITS_GPIO_I2C_EXT_ON     ((ui32) 0x000080)  /* Sets I2C external enable signal to output */
#define LFG_HBITS_GPIO_I2C_EXT_OFF    ((ui32) 0x000000)
#define LFG_HBITS_GPIO_TRIG_IO_OUTOUT ((ui32) 0x000400)  /* Raw trigger */
#define LFG_HBITS_GPIO_TRIG_IO_INPUT  ((ui32) 0x000000)
#define LFG_HBITS_GPIO_PIC_IO0_OUTPUT ((ui32) 0x000800)
#define LFG_HBITS_GPIO_PIC_IO0_INPUT  ((ui32) 0x000000)
#define LFG_HBITS_GPIO_PIC_IO1_OUTPUT ((ui32) 0x001000)
#define LFG_HBITS_GPIO_PIC_IO1_INPUT  ((ui32) 0x000000)
#define LFG_HBITS_GPIO_LED1_OUT       ((ui32) 0x002000)
#define LFG_HBITS_GPIO_EE_WREN_OUT    ((ui32) 0x004000)
#define LFG_HBITS_GPIO_IO_CONFIG      ((ui32) 0xDB0000)  /* Setting for 3 I/Os */
#define LFG_HBITS_GPIO_IO_CONFIG_CM04 ((ui32) 0x1B0000)  /* GPIO22 & 23 need to be inputs for CM04 */

/* dwGpioData
 * ----------
 */
#define LFG_HMASK_GPIO_DATA             ((ui32) 0x6dffff)  /* Mask out all except I/O direction (as must not glitch) */
#define LFG_HBITS_GPIO_I2C_EXT_ENABLE   ((ui32) 0x000080)
#define LFG_HBITS_GPIO_I2C_EXT_DISABLE  ((ui32) 0x000000)
#define LFG_HBITS_GPIO_TRIG_IO_HI       ((ui32) 0x000400)
#define LFG_HBITS_GPIO_TRIG_IO_LO       ((ui32) 0x000000)
#define LFG_HBITS_GPIO_PIC_IO0_HI       ((ui32) 0x000800)
#define LFG_HBITS_GPIO_PIC_IO0_LO       ((ui32) 0x000000)
#define LFG_HBITS_GPIO_PIC_IO1_HI       ((ui32) 0x001000)
#define LFG_HBITS_GPIO_PIC_IO1_LO       ((ui32) 0x000000)
#define LFG_HBITS_GPIO_LED1_ON          ((ui32) 0x000000)
#define LFG_HBITS_GPIO_LED1_OFF         ((ui32) 0x002000)
#define LFG_HBITS_GPIO_EE_WR_ENABLE     ((ui32) 0x000000)
#define LFG_HBITS_GPIO_EE_WR_DISABLE    ((ui32) 0x004000)

#define LFG_HBITS_GPIO_IO0_IN_HI        ((ui32) 0x008000)
#define LFG_HBITS_GPIO_IO0_IN_LO        ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO0_OUT_HI       ((ui32) 0x010000)
#define LFG_HBITS_GPIO_IO0_OUT_LO       ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO0_OUT_ENABLE   ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO0_OUT_DISABLE  ((ui32) 0x020000)

#define LFG_HBITS_GPIO_IO1_IN_HI        ((ui32) 0x040000)
#define LFG_HBITS_GPIO_IO1_IN_LO        ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO1_OUT_HI       ((ui32) 0x080000)
#define LFG_HBITS_GPIO_IO1_OUT_LO       ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO1_OUT_ENABLE   ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO1_OUT_DISABLE  ((ui32) 0x100000)

#define LFG_HBITS_GPIO_IO2_IN_HI        ((ui32) 0x200000)
#define LFG_HBITS_GPIO_IO2_IN_LO        ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO2_OUT_HI       ((ui32) 0x400000)
#define LFG_HBITS_GPIO_IO2_OUT_LO       ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO2_OUT_ENABLE   ((ui32) 0x000000)
#define LFG_HBITS_GPIO_IO2_OUT_DISABLE  ((ui32) 0x800000)


/* RISC INSTRUCTIONS AND STATUS FLAGS
 * ----------------------------------
 * These bit patterns are bitwise ORable to create RISC opcodes.
 * As a style convention, they ought to be _BITS_ but as the names are
 * quite long, anything with _RISC_ in means _BITS_RISC_!
 * The rest of the names are the same the Brooktree software definitions.
 */
#define LFG_RISC_WRITE         ((ui32) 0x10000000)
#define LFG_RISC_WRITE123      ((ui32) 0x90000000)
#define LFG_RISC_WRITE1S23     ((ui32) 0xB0000000)
#define LFG_RISC_WRITEC        ((ui32) 0x50000000)
#define LFG_RISC_SKIP          ((ui32) 0x20000000)
#define LFG_RISC_SKIP123       ((ui32) 0xA0000000)
#define LFG_RISC_JUMP          ((ui32) 0x70000000)
#define LFG_RISC_SYNC          ((ui32) 0x80000000)
#define LFG_RISC_RESYNC        ((ui32) 0x80008000)

#define LFG_RISC_FM1_STAT      ((ui32) 0x00000006)
#define LFG_RISC_FM3_STAT      ((ui32) 0x0000000E)
#define LFG_RISC_SOL_STAT      ((ui32) 0x00000002)
#define LFG_RISC_EOL4_STAT     ((ui32) 0x00000001)
#define LFG_RISC_EOL3_STAT     ((ui32) 0x0000000D)
#define LFG_RISC_EOL2_STAT     ((ui32) 0x00000009)
#define LFG_RISC_EOL1_STAT     ((ui32) 0x00000005)
#define LFG_RISC_VRE_STAT      ((ui32) 0x00000004) /* Sync at end of Even = start of odd */
#define LFG_RISC_VR1_STAT      LFG_RISC_VRE_STAT   /* In Bt878 speak F1 = odd */
#define LFG_RISC_VRO_STAT      ((ui32) 0x0000000C)
#define LFG_RISC_VR2_STAT      LFG_RISC_VRO_STAT
#define LFG_RISC_PXV_STAT      ((ui32) 0x00000000)
                        
#define LFG_RISC_SOL           ((ui32) 0x08000000)
#define LFG_RISC_EOL           ((ui32) 0x04000000)
#define LFG_RISC_IRQ           ((ui32) 0x01000000)

#define LFG_RISC_BE_0000         ((ui32) 0x00000000)
#define LFG_RISC_BE_0001         ((ui32) 0x00001000)
#define LFG_RISC_BE_0010         ((ui32) 0x00002000)
#define LFG_RISC_BE_0011         ((ui32) 0x00003000)
#define LFG_RISC_BE_0100         ((ui32) 0x00004000)
#define LFG_RISC_BE_0101         ((ui32) 0x00005000)
#define LFG_RISC_BE_0110         ((ui32) 0x00006000)
#define LFG_RISC_BE_0111         ((ui32) 0x00007000)
#define LFG_RISC_BE_1000         ((ui32) 0x00008000)
#define LFG_RISC_BE_1001         ((ui32) 0x00009000)
#define LFG_RISC_BE_1010         ((ui32) 0x0000A000)
#define LFG_RISC_BE_1011         ((ui32) 0x0000B000)
#define LFG_RISC_BE_1100         ((ui32) 0x0000C000)
#define LFG_RISC_BE_1101         ((ui32) 0x0000D000)
#define LFG_RISC_BE_1110         ((ui32) 0x0000E000)
#define LFG_RISC_BE_1111         ((ui32) 0x0000F000)

#define LFG_RISC_SET_RISCS_0000    ((ui32) 0x00000000)
#define LFG_RISC_SET_RISCS_0001    ((ui32) 0x00010000)
#define LFG_RISC_SET_RISCS_0010    ((ui32) 0x00020000)
#define LFG_RISC_SET_RISCS_0011    ((ui32) 0x00030000)
#define LFG_RISC_SET_RISCS_0100    ((ui32) 0x00040000)
#define LFG_RISC_SET_RISCS_0101    ((ui32) 0x00050000)
#define LFG_RISC_SET_RISCS_0110    ((ui32) 0x00060000)
#define LFG_RISC_SET_RISCS_0111    ((ui32) 0x00070000)
#define LFG_RISC_SET_RISCS_1000    ((ui32) 0x00080000)
#define LFG_RISC_SET_RISCS_1001    ((ui32) 0x00090000)
#define LFG_RISC_SET_RISCS_1010    ((ui32) 0x000A0000)
#define LFG_RISC_SET_RISCS_1011    ((ui32) 0x000B0000)
#define LFG_RISC_SET_RISCS_1100    ((ui32) 0x000C0000)
#define LFG_RISC_SET_RISCS_1101    ((ui32) 0x000D0000)
#define LFG_RISC_SET_RISCS_1110    ((ui32) 0x000E0000)
#define LFG_RISC_SET_RISCS_1111    ((ui32) 0x000F0000)

#define LFG_RISC_RESET_RISCS_0000  ((ui32) 0x00000000)
#define LFG_RISC_RESET_RISCS_0001  ((ui32) 0x00100000)
#define LFG_RISC_RESET_RISCS_0010  ((ui32) 0x00200000)
#define LFG_RISC_RESET_RISCS_0011  ((ui32) 0x00300000)
#define LFG_RISC_RESET_RISCS_0100  ((ui32) 0x00400000)
#define LFG_RISC_RESET_RISCS_0101  ((ui32) 0x00500000)
#define LFG_RISC_RESET_RISCS_0110  ((ui32) 0x00600000)
#define LFG_RISC_RESET_RISCS_0111  ((ui32) 0x00700000)
#define LFG_RISC_RESET_RISCS_1000  ((ui32) 0x00800000)
#define LFG_RISC_RESET_RISCS_1001  ((ui32) 0x00900000)
#define LFG_RISC_RESET_RISCS_1010  ((ui32) 0x00A00000)
#define LFG_RISC_RESET_RISCS_1011  ((ui32) 0x00B00000)
#define LFG_RISC_RESET_RISCS_1100  ((ui32) 0x00C00000)
#define LFG_RISC_RESET_RISCS_1101  ((ui32) 0x00D00000)
#define LFG_RISC_RESET_RISCS_1110  ((ui32) 0x00E00000)
#define LFG_RISC_RESET_RISCS_1111  ((ui32) 0x00F00000)

#endif /* _LFG_HW_H_ */


