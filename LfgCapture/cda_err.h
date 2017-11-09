/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : AP04
 * Description : Status and error definitions.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 *
 ****************************************************************************
 */

#ifndef _CDA_ERR_H_
#define _CDA_ERR_H_

/*
 * Status and Error Return Codes
 * -----------------------------
 * Status values are returned as a 32 bit integer with the top 16 bits
 * indicating where the error came from (top 8 bits) and the actual error
 * code (next 8 bits down). The lower 16 bits may be used for any other 
 * purpose - such as to return more detailed status information etc.
 */
#define CDA_MAX_ERROR_STRING_LENGTH  512   /* Used in CDA_ErrDisplay */
#define CDA_ERROR_ID_CODE            ((ui32) CDA_MAGIC_ERROR)
#define CDA_ERROR_CODE_MASK          ((ui32) 0xFFFF0000)

#define CDA_OK                       ((ui32) 0x00000000)
#define CDA_ERROR_NOT_CDA            ((ui32) CDA_ERROR_ID_CODE |  1 << 16)
#define CDA_ERROR_BAD_PARAM          ((ui32) CDA_ERROR_ID_CODE |  2 << 16)
#define CDA_ERROR_OPEN_FAILED        ((ui32) CDA_ERROR_ID_CODE |  3 << 16)
#define CDA_ERROR_INCOMPATIBLE       ((ui32) CDA_ERROR_ID_CODE |  4 << 16)
#define CDA_ERROR_HANDSHAKE          ((ui32) CDA_ERROR_ID_CODE |  5 << 16)
#define CDA_ERROR_INTERNAL_ERROR     ((ui32) CDA_ERROR_ID_CODE |  6 << 16)
#define CDA_ERROR_OVERFLOW           ((ui32) CDA_ERROR_ID_CODE |  7 << 16)
#define CDA_ERROR_TIMEOUT            ((ui32) CDA_ERROR_ID_CODE |  8 << 16)
#define CDA_ERROR_NOT_IMPLEMENTED    ((ui32) CDA_ERROR_ID_CODE |  9 << 16)
#define CDA_ERROR_HW_PROBLEM         ((ui32) CDA_ERROR_ID_CODE | 10 << 16)
#define CDA_ERROR_NOT_SUPPORTED      ((ui32) CDA_ERROR_ID_CODE | 11 << 16)
#define CDA_ERROR_OUT_OF_RANGE       ((ui32) CDA_ERROR_ID_CODE | 12 << 16)
#define CDA_ERROR_MALLOC_FAILED      ((ui32) CDA_ERROR_ID_CODE | 13 << 16)
#define CDA_ERROR_SYS_CALL_FAILED    ((ui32) CDA_ERROR_ID_CODE | 14 << 16)
#define CDA_ERROR_DEVICE_IN_USE      ((ui32) CDA_ERROR_ID_CODE | 15 << 16)


/*
 * Status and Error Macros
 * -----------------------
 * _CDA_Roe    - Tests result and returns result if it is an error.
 * _CDA_Proe   - Tests result, if it is an error, the installed error handler 
 *               is called.  The default error handler (CDA_ErrorHandler) will
 *               print a message.
 *               WARNING: Use these macros with "{}" in an "else" statement.
 * _CDA_ErrRet - Used for returning an error with an additional 16 bit
 *               error value (often used to represent the Nth occurrence
 *               in a function).
 *
Status_and_Error_Macros(ForBrief)
 */

#define _CDA_Roe(Function) \
      { ui32 _dwStatus; \
        _dwStatus = Function; \
        if (_dwStatus != 0) return(_dwStatus); }

#define _CDA_Proe(Function, szDescString) \
      { ui32 _dwStatus;\
        _dwStatus = Function; \
        if (_dwStatus != 0) \
        { CDA_ErrProcess(hDevice,szFnName,_dwStatus,szDescString); \
          return(_dwStatus); \
        } }

#define _CDA_ErrRet(dwErrCode, szDescString, dwReturnValue) \
      { ui32 _dwStatus; \
        _dwStatus = (dwErrCode) | (dwReturnValue & 0xFFFF); \
        CDA_ErrProcess(hDevice,szFnName,_dwStatus,szDescString); \
        return(_dwStatus); \
      }

#endif /* _CDA_ERR_H_ */


