/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP04
 * Description : PCI definitions used by the CDA library/driver.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 *
 ****************************************************************************
 */

#ifndef _CDA_PCI_H
#define _CDA_PCI_H


#define CDA_PCI_CONFIG_SIZE_BYTES       ((ui32) 0x40)   /* 64 in decimal = 16 x 32 bit words */


/* PCI BIOS Function Codes
 * -----------------------
 */
#define CDA_PCI_FUNCTION_ID             ((ui8) 0xB1)
#define CDA_PCI_BIOS_PRESENT            ((ui8) 0x01)
#define CDA_PCI_FIND_PCI_DEVICE         ((ui8) 0x02)
#define CDA_PCI_FIND_PCI_CLASS_CODE     ((ui8) 0x03)
#define CDA_PCI_GENERATE_SPECIAL_CYCLE  ((ui8) 0x06)
#define CDA_PCI_READ_CS_BYTE            ((ui8) 0x08)
#define CDA_PCI_READ_CS_WORD            ((ui8) 0x09)
#define CDA_PCI_READ_CS_DWORD           ((ui8) 0x0A)
#define CDA_PCI_WRITE_CS_BYTE           ((ui8) 0x0B)
#define CDA_PCI_WRITE_CS_WORD           ((ui8) 0x0C)
#define CDA_PCI_WRITE_CS_DWORD          ((ui8) 0x0D)


/* PCI BIOS Return Codes
 * ---------------------
 */
#define CDA_PCI_SUCCESSFUL              ((ui8) 0x00)
#define CDA_PCI_NOT_SUCCESSFUL          ((ui8) 0x01)
#define CDA_PCI_FUNC_NOT_SUPPORTED      ((ui8) 0x81)
#define CDA_PCI_BAD_VENDOR_ID           ((ui8) 0x83)
#define CDA_PCI_DEVICE_NOT_FOUND        ((ui8) 0x86)
#define CDA_PCI_BAD_REGISTER_NUMBER     ((ui8) 0x87)


/* PCI Configuration Space Registers
 * ---------------------------------
 */
#define CDA_PCI_CS_VENDOR_ID            ((ui16) 0x00)
#define CDA_PCI_CS_DEVICE_ID            ((ui16) 0x02)
#define CDA_PCI_CS_COMMAND              ((ui16) 0x04)
#define CDA_PCI_CS_STATUS               ((ui16) 0x06)
#define CDA_PCI_CS_REVISION_ID          ((ui16) 0x08)
#define CDA_PCI_CS_CLASS_CODE           ((ui16) 0x09)
#define CDA_PCI_CS_CACHE_LINE_SIZE      ((ui16) 0x0C)
#define CDA_PCI_CS_MASTER_LATENCY       ((ui16) 0x0D)
#define CDA_PCI_CS_HEADER_TYPE          ((ui16) 0x0E)
#define CDA_PCI_CS_BIST                 ((ui16) 0x0F)
#define CDA_PCI_CS_BASE_ADDRESS_0       ((ui16) 0x10)
#define CDA_PCI_CS_BASE_ADDRESS_1       ((ui16) 0x14)
#define CDA_PCI_CS_BASE_ADDRESS_2       ((ui16) 0x18)
#define CDA_PCI_CS_BASE_ADDRESS_3       ((ui16) 0x1C)
#define CDA_PCI_CS_BASE_ADDRESS_4       ((ui16) 0x20)
#define CDA_PCI_CS_BASE_ADDRESS_5       ((ui16) 0x24)
#define CDA_PCI_CS_EXPANSION_ROM        ((ui16) 0x30)
#define CDA_PCI_CS_INTERRUPT_LINE       ((ui16) 0x3C)
#define CDA_PCI_CS_INTERRUPT_PIN        ((ui16) 0x3D)
#define CDA_PCI_CS_MIN_GNT              ((ui16) 0x3E)
#define CDA_PCI_CS_MIN_LAT              ((ui16) 0x3F)


#endif  /* _CDA_PCI_H */

