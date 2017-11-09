/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP04
 * Description : CDA library definitions.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * Note this include file is also used by drivers.
 *
 * The data structure is described below:
 *
 * "dwNumBytesAllocated"- Defines the number of bytes allocated and pointed 
 * to by "pdwRegisterData". This is set up in the library open call.
 *
 * "dwNumBytesFree" keeps track of the amount of memory free when the list
 * built.
 *
 * The format of the data is:
 *
 * 1. "Register Type" - 4 bytes representing the type of register (byte/word 
 *    etc). Note the type definitions for these are including a "magic"
 *    number for additional assert/error checking.
 *
 * 2. "Address" - 4 bytes containing the hardware register address as an 
 *                address offset from psCDA->dwBaseAddr. For access to 
 *                absolute addresses, the CDA device type would be 
 *                CDA_DEVICE_VIRTUAL.
 *
 * 3. The data itself, which is always aligned to 32 bits and LSB aligned
 *    (i.e. an 8 bit word at D7..D0).
 *
 ****************************************************************************
 */

#ifndef _CDA_LIB_H_
#define _CDA_LIB_H_


#define CDA_REG_RECORD_SIZE        ((ui32) 12)
#define CDA_REG_LIST_DEFAULT_SIZE  (512 * CDA_REG_RECORD_SIZE)   /* Default number of individual registers (entries) */


/* A structure containing register info to send to the driver
 * ----------------------------------------------------------
struct_tCDA_RegList(ForBrief)
 */
struct tCDA_RegList
{
   volatile ui32 dwNumBytesAllocated;
  /* See also _CDA_RegList_GetNumFreeBytes() which is a macro that returns
   * the number of free bytes by looking at the difference between the base 
   * Register List pointer and the roving pointer.
   */
   ui32 *pdwRegList;              /* Base (fixed) pointer */
   volatile ui32 *pdwRegListPtr;  /* Roving pointer (volatile as ISR may update) */
};


/*
 * Register Type Bitwise ORable Options
 * ------------------------------------
CDA_Register_Type_Flags(ForBrief)
 * "dwBitsRegType"
 */
/* NOTE: This is not ideal, as the definition also appears in cda_api.h,
 * but the Win32 drivers need this file to get the access macros, which
 * logically should remain in this file with the rest of the information
 * here.  It is extremely unlikely that the two definitions below should 
 * ever change, so it's probably OK to leave like this, Colin 15-Jun-00.
 */
#ifndef CDA_LIB_ID
#define CDA_LIB_ID            ((ui32) 0xCD000000)
#endif

#ifndef CDA_MAGIC_REG_TYPE
#define CDA_MAGIC_REG_TYPE    ((ui32) CDA_LIB_ID | ( 1 << 16)) /* User API related (and used internally for register list) */
#endif


#define CDA_8BIT      (CDA_MAGIC_REG_TYPE | (ui32) 0x0001)
#define CDA_16BIT     (CDA_MAGIC_REG_TYPE | (ui32) 0x0002)
#define CDA_32BIT     (CDA_MAGIC_REG_TYPE | (ui32) 0x0004)
/* For clarity and convenience, CDA_NO_OP is defined here, although it is 
 * not used by the user. It is used internally to indicate the end of a
 * a cached register list. 
 */
#define CDA_NO_OP         (CDA_MAGIC_REG_TYPE | (ui32) 0x0008) 

#define CDA_NOW           (CDA_MAGIC_REG_TYPE | (ui32) 0x0010)
#define CDA_CACHED        (CDA_MAGIC_REG_TYPE | (ui32) 0x0020)     /* If clear assume CDA_NOW */
#define CDA_READ_NO_FLUSH (CDA_MAGIC_REG_TYPE | (ui32) 0x0040) /* Allows reading without flushing */


/* CDA Register Types
 * ------------------
 * Note format 0xAAAABBCC, where
 * AAAA = Magic number.
 * BB   = CDA_REG_READ/WRITE.
 * CC   = CDA Register Type Flags (see above).
 * These types are used in the register list passed to the driver.
 */
#define CDA_REG_WRITE  ((ui32) 0x00000100)
#define CDA_REG_READ   ((ui32) 0x00000200)
#define CDA_CNF_READ   ((ui32) 0x00000400)

#define CDA_REG_WRITE_8BITS    ( CDA_REG_WRITE | CDA_8BIT  )
#define CDA_REG_WRITE_16BITS   ( CDA_REG_WRITE | CDA_16BIT )
#define CDA_REG_WRITE_32BITS   ( CDA_REG_WRITE | CDA_32BIT )

#define CDA_REG_READ_8BITS     ( CDA_REG_READ | CDA_8BIT  )
#define CDA_REG_READ_16BITS    ( CDA_REG_READ | CDA_16BIT )
#define CDA_REG_READ_32BITS    ( CDA_REG_READ | CDA_32BIT )

#define CDA_CNF_READ_8BITS     ( CDA_CNF_READ | CDA_8BIT  )
#define CDA_CNF_READ_16BITS    ( CDA_CNF_READ | CDA_16BIT )
#define CDA_CNF_READ_32BITS    ( CDA_CNF_READ | CDA_32BIT )

/*
 * Macros to access Register List
 * ------------------------------
Register_List_MACROS(ForBrief)
 */
#define _CDA_RegList_GetNumFreeBytes(pRL)  (pRL->dwNumBytesAllocated - ((volatile ui8*)pRL->pdwRegListPtr - (ui8*)pRL->pdwRegList))

#define _CDA_RegList_SetRegType(pRL, dwVal)    /*lint -save -e415*/ (*((volatile ui32*)(pRL)+0) = (dwVal)) /*lint -restore*/
#define _CDA_RegList_SetRegAddr(pRL, dwVal)    /*lint -save -e415*/ (*((volatile ui32*)(pRL)+1) = (dwVal)) /*lint -restore*/
#define _CDA_RegList_SetRegData32(pRL, dwVal)  /*lint -save -e415*/ (*((volatile ui32*)(pRL)+2) = (dwVal)) /*lint -restore*/

#define _CDA_RegList_GetRegType(pRL)             (*((ui32*)(pRL)+0))
#define _CDA_RegList_GetRegAddr(pRL)             (*((ui32*)(pRL)+1))
#define _CDA_RegList_GetRegData32(pRL)           (*((ui32*)(pRL)+2))

#endif  /* _CDA_LIB_H_ */

