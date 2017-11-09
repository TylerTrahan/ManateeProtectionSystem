/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP04
 * Description : CDA buffer structure and related scatter/gather table stuff.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * Note this include file is also used by drivers.
 *
 ****************************************************************************
 */

#ifndef _CDA_BUF_H_
#define _CDA_BUF_H_

/*
 * Scatter/Gather Table (Physical Addresses & Lengths)
 * ---------------------------------------------------
 * Scatter/Gather table information/macros for locking and freeing physical
 * memory. Note the Area For Transfer (AFT) entries - this allows a sub-area
 * within the total scatter/gather table to be handled. This can be useful to
 * handle a region of interest within a larger region.
 * 
 * The SG table is of the form (all entries are ui32s):
 *
 *   pSGTable-> 0. Allocated size of table (in bytes),
 *              1. Virtual pointer (to base address),
 *              2. Number of Entries (Physical address/length pairs),
 *              3. Total amount of memory represented by table (in bytes),
 *              4. Offset (in bytes - virtual) to Area For Transfer (AFT),
 *              5. Transfer Size of AFT (if 0, then transfer full area).
 *              - Address,
 *              - Length,
 *              ...
 *
 * x = Pointer to SG table, y = Entry number (0..N-1), z = Value to set.
 * 
 * All sizes are in bytes.
 *
Scatter_Gather_Macros(ForBrief)
 */

/* 64 bit Scatter-Gather tables */
#if defined _CDA_SG64
  #if defined _PHX_WIN32
    typedef UINT64  uiSG;   
  #elif defined _PHX_LINUX
    #if defined _LP64
      typedef unsigned long uiSG;
    #else
      typedef unsigned long long uiSG;
    #endif /* _LP64 */
  #elif defined _PHX_MACOSX
    typedef UInt64 uiSG;
  #elif defined _LFG_MACOSX
    typedef UInt64 uiSG;
  #elif defined _LFG_WIN32
    typedef UINT64  uiSG;   
  #elif defined _LFG_WIN64
    typedef UINT64  uiSG;   
  #else
    typedef unsigned long long uiSG;
  #endif /* _PHX_WIN32 etc  */

/* 32 bit Scatter-Gather tables */
#elif defined(__LP64__)
  typedef unsigned int uiSG;
#else
  typedef unsigned long uiSG;
#endif /* _CDA_SG64 */


#if defined _CDA_SG64
  #define _CDA_SGTable_AddrLo32(x) ((ui32) (x)    )
  #define _CDA_SGTable_AddrHi32(x) ((ui32) (x>>32))
#else
  /* For library compatability with 32 bit only OSs, redefine the Hi & Lo addr macros */
  #define _CDA_SGTable_AddrLo32(x) ((ui32) (x))
  #define _CDA_SGTable_AddrHi32(x) ((ui32) (0))
#endif


#define CDA_SGTABLE_HEADER_SIZE      ((ui32)(6*sizeof(uiSG)))  /* 6 header values */
#define CDA_SGTABLE_BYTES_PER_ENTRY  ((ui32)(2*sizeof(uiSG)))  /* 1 address + 1 length */

#define _CDA_SGTable_Get_AllocatedSize(x)     (*(((uiSG*)(x))+0))
#define _CDA_SGTable_Get_VirtualAddr(x)       (*(((uiSG*)(x))+1))
#define _CDA_SGTable_Get_NumEntries(x)        (*(((uiSG*)(x))+2))
#define _CDA_SGTable_Get_MemorySize(x)        (*(((uiSG*)(x))+3))
#define _CDA_SGTable_Get_StartOffset(x)       (*(((uiSG*)(x))+4))
#define _CDA_SGTable_Get_TransferSize(x)      (*(((uiSG*)(x))+5))

#define _CDA_SGTable_Get_PhysAddr(x,y)        (*(((uiSG*)(x))+6+((y)<<1)))
#define _CDA_SGTable_Get_PhysLen(x,y)         (*(((uiSG*)(x))+7+((y)<<1)))

#if defined _CDA_SG64
#define _CDA_SGTable_Get_PhysAddrLo32(x,y)    ((ui32)(*(((uiSG*)(x))+6+((y)<<1))))
#define _CDA_SGTable_Get_PhysLenLo32(x,y)     ((ui32)(*(((uiSG*)(x))+7+((y)<<1))))
#define _CDA_SGTable_Get_PhysAddrHi32(x,y)    ((ui32)((*(((uiSG*)(x))+6+((y)<<1))>>32))
#define _CDA_SGTable_Get_PhysLenHi32(x,y)     ((ui32)((*(((uiSG*)(x))+7+((y)<<1))>>32))
#else
#define _CDA_SGTable_Get_PhysAddrLo32(x,y)    _CDA_SGTable_Get_PhysAddr(x,y)
#define _CDA_SGTable_Get_PhysLenLo32(x,y)     _CDA_SGTable_Get_PhysLen(x,y)
#endif

#define _CDA_SGTable_Set_AllocatedSize(x,z)   (*(((uiSG*)(x))+0) = z)
#define _CDA_SGTable_Set_VirtualAddr(x,z)     (*(((uiSG*)(x))+1) = z)
#define _CDA_SGTable_Set_NumEntries(x,z)      (*(((uiSG*)(x))+2) = z)
#define _CDA_SGTable_Set_MemorySize(x,z)      (*(((uiSG*)(x))+3) = z)
#define _CDA_SGTable_Set_StartOffset(x,z)     (*(((uiSG*)(x))+4) = z)
#define _CDA_SGTable_Set_TransferSize(x,z)    (*(((uiSG*)(x))+5) = z)

#define _CDA_SGTable_Set_PhysAddr(x,y,z)      (*(((uiSG*)(x))+6+((y)<<1)) = z)
#define _CDA_SGTable_Set_PhysLen(x,y,z)       (*(((uiSG*)(x))+7+((y)<<1)) = z)


/* CDA Buffer Structure
 * --------------------
 * This structure is suitable for use as a DMA buffer - or any application
 * that requires scatter/gather information and the locking down of virtual
 * memory into physical memory.
 *
struct_tCDA_Buffer(ForBrief)
 */
struct tCDA_Buffer
{
   void *pvData;         /* Virtual pointer to malloc'ed data */
   ui32 dwSizeBytes;     /* Number of bytes allocated         */
   ui32 fUserAllocated;  /* To indicate who's allocated the DMA buffer */
#if defined _PHX_CDA
   ui32 fUserLocked;     /* To indicate if this is part of User Locked buffers */
#endif
   ui32 dwPageSize;      /* Physical (DMA) page size in bytes - useful for some library routines */
   void *pvSGTable;      /* Pointer to scatter table of physical addresses & lengths */

#if defined _PHX_CDA
   struct tCDA_Buffer *pPrev;
   struct tCDA_Buffer *pNext;
#endif
}; 

typedef struct tCDA_Buffer CDA_Buffer;

#if defined _PHX_CDA
/* CDA User Locked Buffer Structure
 * --------------------------------
 * This contains the Host memory buffer.
 *
struct_tCDA_UserLockedBuffInfo(ForBrief)
 */
struct tCDA_UserLockedBuffInfo
{
   /* N.B. Win32 is unable to lock down a CDA_Buffer greater than 63.96MBytes.
    * Therefore in order to cope with User Locked buffers of greater size than this,
    * we must have a linked list of CDA_Buffers, with a max size of say 32MB (for convenience).
    */
   /* Buffer structures */
   CDA_Buffer *psCdaBufferFirst;    /* Video data      */

   ui64  qwTotalSizeBytes;          /* The total size of data in the linked list of CDA_Buffers */ 

   /* Pointers to previous and next structures in the linked list */
   struct tCDA_UserLockedBuffInfo *pPrev;
   struct tCDA_UserLockedBuffInfo *pNext;

};
typedef struct tCDA_UserLockedBuffInfo CDA_UserLockedBuffInfo;
#endif

#endif /* _CDA_BUF_H_ */

