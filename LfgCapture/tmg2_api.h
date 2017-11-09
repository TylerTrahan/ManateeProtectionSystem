/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : TMG Imaging Library
 * Description : Header file for new API and DVR functions.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 *
 ****************************************************************************
 */

#ifndef _TMG2_API_H_
#define _TMG2_API_H_


/* OS Dependences
 * --------------
 */
#ifdef _TMG_WINDOWS
#ifndef _AVI_WINDOWS
#define _AVI_WINDOWS
#endif
#endif

#ifdef _TMG_LINUX
#ifndef _AVI_LINUX
#define _AVI_LINUX
#endif
#endif

#ifdef _TMG_VXWORKS
#ifndef _AVI_VXWORKS
#define _AVI_VXWORKS
#endif
#endif


/* Type Definitions
 * ----------------
 */
/* None */


/* TMG Include Files
 * -----------------
 */
#if defined _TMG_MACOSX && !defined __MWERKS__
#import <tmglib/tmg_api.h>
#import <tmglib/avi_lib.h>
#import <tmglib/avi_handle.h>
#else
#include <tmg_api.h>
#include <avi_lib.h>
#include <avi_handle.h>
#endif



/* General SETTINGS
 * ----------------
 */
#define TMG_DEFAULT_WIDTH     2048   /* Image width and height, if no buffer size declared - see TMG_ImageCreate() */
#define TMG_DEFAULT_HEIGHT    2048


/* General Set/Get Parameters for sequence recording
 * -------------------------------------------------
 */
#define TMG_DVR_MAX_STREAMS     16   /* Max streams for video recording and replay */

#define TMG_DVR_FRAME_RATE     1001
#define TMG_DVR_FORMAT         1002
#define TMG_DVR_SOURCE         1003
#define TMG_DVR_DESTINATION    1004
#define TMG_DVR_FRAME_COUNT    1005


/* Pointer related parameters
 * --------------------------
 */
#define TMG_FILENAME_IN          1101
#define TMG_FILENAME_OUT         1102
#define TMG_IMAGE_BUFFER         1103
#define TMG_INTERNAL_STRUCTURE   1104


/* Sub-Parameter Sets
 * ------------------
 */
#define TMG_AVI_MJPEG           1205


/* Drawing Related
 * ---------------
 */
#define TMG_FONT_9x14          (TMG_MAX_VALUE+500)


/* Display Related Parameters
 * --------------------------
 */
#define TMG_GRAPHICS_PLANE     (TMG_MAX_VALUE+600)
#define TMG_VIDEO_OVERLAY      (TMG_MAX_VALUE+601)
#define TMG_VIDEO_OVERLAY_PTR  (TMG_MAX_VALUE+602)


/* Boolean Operators
 * -----------------
 */
#define TMG_AND         (TMG_MAX_VALUE+700)
#define TMG_OR          (TMG_MAX_VALUE+701)
#define TMG_XOR         (TMG_MAX_VALUE+702)
#define TMG_SHIFT_RIGHT (TMG_MAX_VALUE+703)
#define TMG_SHIFT_LEFT  (TMG_MAX_VALUE+704)
#define TMG_ADD         (TMG_MAX_VALUE+705)


/* TMG_DVR Structure
 * -----------------
 * This structure is for Digital Video recording and playback.
 */
struct tTMG_DVR
{
   int fInUse;
   int fOpenedForInput;      /* Flag to indicate whether opened for input (read) or output (write) */

   int hHandle;   /* Back reference to user handle number to reference instance of structure */

   char szFilenameIn[TMG_FIELD_LENGTH];
   char szFilenameOut[TMG_FIELD_LENGTH];

   int nImageWidth;
   int nImageHeight;
   int nPixelFormat;
   int nFrameRate;
   int fColor;         /* Shortcut for colour or not */

   int nStreamFormat;  /* e.g. TMG_AVI_MJPEG */
   int nStreamSource;  /* e.g. TMG_FILE      */
   int nStreamDest;    /* e.g. TMG_FILE      */

   int nQFactor;       /* 1..400 JPEG Q factor */

   ui32 hJpegImage;
#define TMG_DVR_JPEG_BUFFER_SIZE 4194304   /* SETTING: JPEG buffer size for recording */
   int nJpegBufferSize;
   ui8 *pbJpegBuffer;

#define TMG_DVR_AUDIO_BUFFER_SIZE (44100*2*2)  /* 1 second of max sampling at 16 bits per channel for 2 channels */
   ui8 *pbAudioData;


#if defined _TMG_WINDOWS || defined _TMG_MAC || defined _TMG_LINUX || defined _TMG_MACOSX || defined _TMG_QNX6
   void (*pFnErrorHandler)(ui32, char*, ui32, char*); /* Pointer to error handler function */
#else /* _TMG_DOS32 */
   void (EXPORT_FN *pFnErrorHandler)(ui32, char*, ui32, char*); /* Pointer to error handler function */
#endif

  /* AVI Library Related
   * -------------------
   */
   AviHandle hAVI;
   int nFrameCount;          /* Number of frames in input stream (AVI) */ 
   int nRecordedFrameCount;  /* For recording */
};


/* Prototypes
 * ----------
 */
#ifdef __cplusplus
extern "C" {
#endif

/* tmg2_api.c
 * ----------
 */
ui32 EXPORT_FN TMG_Stream_Library_Open( void);
ui32 EXPORT_FN TMG_Stream_Library_Close( void);

ui32 EXPORT_FN TMG_Stream_Create( int *phDVR);
ui32 EXPORT_FN TMG_Stream_Destroy( int hDVR);
ui32 EXPORT_FN TMG_Stream_OpenForInput( int hDVR);
ui32 EXPORT_FN TMG_Stream_OpenForOutput( int hDVR);
ui32 EXPORT_FN TMG_Stream_Close( int hDVR);

ui32 EXPORT_FN TMG_Stream_FilenameGet( int hDVR, int nParameter, char *pszFilename);
ui32 EXPORT_FN TMG_Stream_FilenameSet( int hDVR, int nParameter, char *pszFilename);

ui32 EXPORT_FN TMG_Stream_ParameterGet( int hDVR, int nParameter, int *pnSetting);
ui32 EXPORT_FN TMG_Stream_ParameterSet( int hDVR, int nParameter, int nSetting);

ui32 EXPORT_FN TMG_Stream_ParameterPtrGet( int hDVR, int nParameter, void **ppVoid);
ui32 EXPORT_FN TMG_Stream_ParameterPtrSet( int hDVR, int nParameter, void *pVoid);

ui32 EXPORT_FN TMG_Stream_FileRead( int hDVR, int hImage, int nIndex, int nFormat);
ui32 EXPORT_FN TMG_Stream_FileWrite( int hDVR, int hImage);

ui32 EXPORT_FN TMG_Stream_InfoGet( ui32 hHandle, struct tTMG_DVR *psDVR_ForUser);
ui32 EXPORT_FN TMG_Stream_InfoSet( ui32 hHandle, struct tTMG_DVR *psDVR_FromUser);


ui32 EXPORT_FN TMG_Image_Create( int *phImage, int nSizeInPixels, void *pVoid);
ui32 EXPORT_FN TMG_Image_Destroy( int hImage);

ui32 EXPORT_FN TMG_Image_FileRead( int hImage);
ui32 EXPORT_FN TMG_Image_FileWrite( int hImage, int nFormat);

ui32 EXPORT_FN TMG_Image_FilenameSet( int hImage, int nParameter, char *pszFilename);
ui32 EXPORT_FN TMG_Image_FilenameGet( int hImage, int nParameter, char *pszFilename);

ui32 EXPORT_FN TMG_Image_FlagsSet( int hImage, ui32 dwFlags, int nSetting);
int  EXPORT_FN TMG_Image_FlagsGet( int hImage, ui32 dwFlags);

ui32 EXPORT_FN TMG_Image_ParameterSet( int hImage, int nParameter, int nSetting);
ui32 EXPORT_FN TMG_Image_ParameterGet( int hImage, int nParameter, int *pnSetting);

ui32 EXPORT_FN TMG_Image_ParameterPtrGet( int hImage, int nParameter, void **ppVoid);
ui32 EXPORT_FN TMG_Image_ParameterPtrSet( int hImage, int nParameter, void *pVoid);


/* Display Related
 * ---------------
 */
#if defined _TMG_WINDOWS
ui32 EXPORT_FN TMG_Display_Create( int *phDisplay);
ui32 EXPORT_FN TMG_Display_Destroy( int hDisplay);
ui32 EXPORT_FN TMG_Display_Init( Tdisplay_handle hDisplay, HWND hWnd);
ui32 EXPORT_FN TMG_Display_Image( Tdisplay_handle hDisplay, ui32 hImage, int nTargetPlane);
ui32 EXPORT_FN TMG_Display_ParameterGetPtr( Tdisplay_handle hDisplay, int nParameter, void **ppVoid);

ui32 EXPORT_FN TMG_Display_VideoOverlaySetSourceSize( Tdisplay_handle hDisplay, int nWidth, int nHeight, ui32 dwPixelFormat);
ui32 EXPORT_FN TMG_Display_VideoOverlaySetTargetSize( Tdisplay_handle hDisplay, i16 gTargetRoi[] );

ui32 EXPORT_FN TMG_Draw_Text( int hImage, char *pszString, int nFont, int nX_Across, int nY_Down, int nBlendMode, int nColor);
#endif  /* Only supported under certain OSs */

/* tmg2_ip.c
 * ---------
 */
ui32 EXPORT_FN TMG_IP_ColorBalance( int hImageIn, int hImageOut, int nR, int nG, int nB);
ui32 EXPORT_FN TMG_IP_ImageOperation( int hImage1, int hImage2, int nOperation);
ui32 EXPORT_FN TMG_IP_ImageOperation_Matrix3x3( int hImage, float fMatrix[9], int nOffset[3]);
ui32 EXPORT_FN TMG_IP_PixelOperation( int hImage, int nOperation, ui32 dwOperation);

ui32 EXPORT_FN TMG_IP_Image_Convert( int hImageIn, int hImageOut, int nPixelFormat, ui32 dwFlags);
ui32 EXPORT_FN TMG_IP_Image_Copy( int hImageIn, int hImageOut);

ui32 EXPORT_FN TMG_IP_JPEG_Compress( int hImageIn, int hImageOut);
ui32 EXPORT_FN TMG_IP_JPEG_Decompress( int hImageIn, int hImageOut);

ui32 EXPORT_FN TMG_IP_Field_to_Frame( int hImageIn, int hImageOut, int nMethod );
ui32 EXPORT_FN TMG_IP_Frame_to_Field( int hImageIn, int hImageOut, int nFieldSelect );


/* tmg2_bayer.c
 * ------------
 */
ui32 EXPORT_FN TMG_IP_Bayer_Convert_to_BGRX32( int hImageIn, int hImageOut, int nDecodeScheme, float fColorMatrix[9], int nOffset[3], int nTopLeftPixel);
ui32 EXPORT_FN TMG_IP_Bayer_Convert_to_ThreePlanes( int hImageIn, int hImageOutR, int hImageOutG, int hImageOutB, int nTopLeftPixel);
ui32 EXPORT_FN TMG_IP_Bayer_Map_to_RGB24( int hImageIn, int hImageBayer_RGB, int nTopLeftPixel);
Terr EXPORT_FN TMG_IP_Bayer_Convert_to_BGRX32_Sub2( int hImageIn, int hImageOut, int nTopLeftPixel);


#ifdef __cplusplus
};
#endif


/* Macros
 * ------
 * Note as the error handler is stored in the structure, if hHandle is 
 * invalid we cannot call the error handler - thus we must just return
 * TMG_ERROR_BAD_PARAM silently.
 */
#define _TMG_DVR_GetAndCheckpsDVR(hHandle, psDVR) \
   { if ( (hHandle) > TMG_DVR_MAX_STREAMS ) \
     { \
        return(TMG_ERROR_BAD_PARAM); \
     } \
     else \
     { \
        psDVR = &gsTMG_DVR_Array[hHandle]; \
        _TMG_Assert(psDVR != NULL); \
        if ( !psDVR->fInUse ) \
        { \
           return(TMG_ERROR_BAD_PARAM); \
        } \
     } \
   }


#endif  /* _TMG2_API_H_ */

