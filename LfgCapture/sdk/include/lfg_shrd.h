/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP09
 * Description : Structure and definitions that need to be shared with driver.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * This file contains structures and definitions that are needed by the 
 * library and low level drivers (including typically the device driver too).
 * A good example is the PLD Info structure that is passed down into the 
 * driver and then the programming algorithm, implemented in the driver,
 * accesses this structure as usual.
 *
 ****************************************************************************
 */

#ifndef _LFG_SHRD_H_
#define _LFG_SHRD_H_


/* PLD Info Structure
 * ------------------
 * For passing info about PLD configuration to the driver.
 * Note: All addresses & bit masks are ui32 rather than ui16 to allow this
 * structure to be passed to the driver without endian issues.
 * Note also that all references here refer to physical hardware registers
 * and bit positions etc.
 *
tLFG_PldInfo(ForBrief)
 */
struct tLFG_PldInfo
{
   ui32 dwConfigAlgorithm;  /* Which algorithm to use                      */

   ui8  *pbDataBuf;         /* Pointer to configuration data               */
   ui32 dwNumDataBytes;     /* Bytes of data pointed to by pbDataBuf       */
   ui32 dwChecksum;         /* 32 bit checksum of the configuration data   */

   ui32 dwAddrDataPort;     /* Address to write data (pbDataBuf)      */

   ui32 dwProgReqAddr;      /* Address to write the program request bit    */
   ui32 dwProgReqHwMask;    /* Bit mask showing position of program request bit */
   ui32 dwProgReqBitsOn;    /* Bit pattern to write to request programming */
   ui32 dwProgReqBitsOff;   /* Bit pattern to stop requesting programming  */
   ui16 *pwProgReqShadow;   /* Shadow of register with program request bit */

   ui32 dwConfAddr;         /* Address to read the config status bit       */
   ui32 dwConfMask;         /* Bit mask showing position of config status bit */
   ui32 dwConfBitsTrue;     /* Bit pattern indicating PLD configured       */
   ui32 dwConfBitsFalse;    /* Bit pattern indicating PLD not configured   */
};


/* dwConfigAlgorithm
 * -----------------
 */
#define LFG_PLD_ALGORITHM_ALT_PSA  (LFG_MAGIC_PLD_ALGORITHM | (ui32) 1)


/* Driver Messages - Used by the Packet Transfer function.
 * ---------------
 *
Packet_Transfer_Messages(ForBrief)
 */
#define LFG_MSG_PLD_PROGRAM         (LFG_MAGIC_DRIVER_MSG | (ui32) 1)
#define LFG_MSG_PLD_UPDATE_SHADOW   (LFG_MAGIC_DRIVER_MSG | (ui32) 2)


#endif /* _LFG_SHRD_H_ */

