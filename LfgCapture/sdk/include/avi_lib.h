/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : TMG Imaging Library
 * Description : Public include file for AVI handling library.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 *
 ****************************************************************************
 */

#if defined _AVI_WINDOWS  /* 32 and 64 bit --------------------------------------------------------------------*/
#include <windows.h>
/*#include <iostream.h>*/

/* Colin999: dllexport always set - much simpler */
#define AVI_LIB_API __declspec(dllexport)

#include <io.h>
#include <share.h>
#include <malloc.h>

#elif defined _AVI_LINUX    /* ---------- */
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#include <malloc.h>
#define AVI_LIB_API /* Defined as nothing when compiled into TMG library */

#elif defined _AVI_MACOSX   /* ---------- */
#include <unistd.h>
#include <sys/uio.h>
#include <pthread.h>
#define AVI_LIB_API /* Defined as nothing when compiled into TMG library */

#if !defined TRUE
#define TRUE (1==1)
#endif
#if !defined FALSE
#define FALSE (1==0)
#endif


#elif defined _AVI_VXWORKS  /* ---------- */
#define AVI_LIB_API /* Defined as nothing when compiled into DVR/TMG library */

#else
#error *** AVI Library - missing compiler directive (e.g. _AVI_WINDOWS) *** 

#endif  /* OS Dependencies  ---------------------------------------------------------------------- */


#ifndef _AVI_VXWORKS
#include <memory.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


/* ASL Standard types */

#ifdef _AVI_WINDOWS
#ifndef _ASL_TYPEDEFS
   #define _ASL_TYPEDEFS
   typedef  char              i8;
   typedef  unsigned char     ui8;
   typedef  short             i16;
   typedef  unsigned short    ui16;
   typedef  long              i32;
   typedef  unsigned long     ui32;
   typedef  short             m16;
   typedef  unsigned short    mu16;
   typedef  long              m32;
   typedef  unsigned long     mu32;
   typedef  signed _int64     i64;
   typedef  unsigned _int64   ui64;
   typedef  _int64            off64_t;
   typedef  int               fDesc;

#endif
#define _fSeek _lseeki64

#elif defined _AVI_LINUX
#ifndef _ASL_TYPEDEFS
   #define _ASL_TYPEDEFS
   typedef char                  i8;
   typedef unsigned char         ui8;
   typedef short                 i16;
   typedef unsigned short        ui16;
   typedef int                   i32;
   typedef unsigned int          ui32;
   typedef long                  m16;
   typedef unsigned long         mu16;
   typedef long                  m32;
   typedef unsigned long         mu32;
   typedef  long long            i64;
   typedef  unsigned long long   ui64;
   /*typedef long long             off64_t;  [Commented out by Rich during March 2012 Linux port] */
   typedef int                   fDesc;
#endif
#define _fSeek lseek64

#elif defined _AVI_MACOSX
#ifndef _ASL_TYPEDEFS
   #define _ASL_TYPEDEFS
   typedef char                  i8;
   typedef unsigned char         ui8;
   typedef short                 i16;
   typedef unsigned short        ui16;
   typedef int                   i32;
   typedef unsigned int          ui32;
   typedef long                  m16;
   typedef unsigned long         mu16;
   typedef long                  m32;
   typedef unsigned long         mu32;
   typedef unsigned long long    ui64;
   typedef off_t                 off64_t;	/* This seems to be a signed 64 bit integer in the MAC world */
   typedef int                   fDesc;
#endif
#define _fSeek lseek

#elif defined _AVI_VXWORKS
#ifndef _ASL_TYPEDEFS
   #define _ASL_TYPEDEFS
   typedef char                  i8;
   typedef unsigned char         ui8;
   typedef short                 i16;
   typedef unsigned short        ui16;
   typedef int                   i32;
   typedef unsigned int          ui32;
   typedef long                  m16;
   typedef unsigned long         mu16;
   typedef long                  m32;
   typedef unsigned long         mu32;
   typedef  long long            i64;
   typedef  unsigned long long   ui64;
   typedef long long             off64_t;
   typedef int                   fDesc;
#endif
#define _fSeek(fd, offet, whence) lseek(fd, (ui32) offet, whence)

#endif  /* OS Types */

typedef enum{
   AVI_ERR_OK,
   AVI_INVALID_ARGUMENT,   /* Invalid argument */
   AVI_INVALID_HANDLE,     /* Invalid handle passed to function */
   AVI_NOT_INIT,           /* Function has been called without the library having been initialized. */
   AVI_NULL_POINTER,       /* Invalid NULL pointer passed to function. */
   AVI_ASSIGNED_POINTER,   /* Non null (i.e. probably already used) pointer has been passed to malloc function */
   AVI_MEM_ALLOC,          /* Memory allocation error */
   AVI_MEM_REALLOC,        /* Memory re-allocation error.*/
   AVI_MEM_BUFFER_FULL,    /* Memory buffer used for buffered writes if full */
   AVI_OUT_FILE_OPEN,      /* Fail to open output file */
   AVI_HANDLE_IN_USE,      /* File handle already in use */
   AVI_FILE_NAME,          /* Error in file name */
   AVI_WRITE_FAIL,         /* Output file write failure */
   AVI_IN_FILE_OPEN,       /* Fail to open input file */
   AVI_NAME_ERROR,         /* Error in file name / file name too long */
   AVI_MAX_OPEN_FILES,     /* Number of input / output files is already at maximum */
   AVI_READ_FAIL,          /* Input file read fail */
   AVI_CORRUPT_FILE,       /* File corrupt */
   AVI_FILE_RW,            /* Attempt to read a file opened for write, or write a file opened for read */
   AVI_FILE_NOT_SUPPORTED, /* Unsupported file type, e.g. old style file index */
   AVI_END_OF_SEQUENCE,    /* Request has been made to read a frame number beyond the end of the sequence */
   AVI_DATA_ERROR,         /* Error in image data */
   AVI_THREAD_ERROR,       /* Multi-threading error. */
   AVI_THREAD_CS_ERROR,    /* Error in thread critical section of code */
   AVI_AUDIO_ERROR,        /* Invalid parameters for audio stream. */
}AVI_ERR;

typedef enum{
   eAVI_MJPG,              /* Standard MJPEG .avi file type */
   eAVI_BGR24,             /* Standard .avi file using 24bit RGB images*/
   eAVI_BGR32,             /* .avi compatible 32 bit colour */
   eAVI_BGR48,             /* .avi compatible 48 bit colour */
   eAVI_MONO8,             /* Standard .avi file using 8 bit mono images */
   eAVI_MONO16,            /* .avi compatible 16 bit mono */
   eAVI_MJPGi,             /* Interlaced MJPEG file */
   eAVI_BGR24i,            /* Interlaced bitmap file */
}eFile_Type;

#define AVI_SUPPORTED_TYPE_COUNT 8

typedef enum{
   eRead,                  /* File opened for read */
   eWrite,                 /* File opened for write */
}eReadWrite;

typedef enum{
   eDisk,                  /* Disk file */
   eRAM,                   /* RAM */
}eStore_Type;

typedef struct tagAviHandle {               
   eReadWrite eRW;            /* Flag showing whether the file has been opened for read or write */
   char  *pszFileBufferName;  /* Pointer to file or buffer name */
   eFile_Type eFileType;      /* File type */
   eStore_Type eStoreType;    /* Specifies whether the file is in volatile memory or on disk */
   ui32 dwNumberOfFrames;     /* Total number of frames in the file */
   ui32 dwMaxFrameSize;       /* Suggested buffer size for video frame read. */
   ui32 dwWidth;              /* Frame width  */
   ui32 dwHeight;             /* Frame height */
   ui32 dwMicroSecPerFrame;   /* Proxy for file frame rate */
   ui32 dwAudioSampleRate;    /* Samples per second */
   ui32 dwAudioBitsPerSample; /* Bits per sample, must be 8 or 16 */
   ui32 dwChannels;           /* Number of channels, 1 for mono, 2 for stereo. If 0 is specified, no audio indexes or audio data are recorded*/
   ui8  *pbReadFrameBuffer;   /* Pointer to frame buffer used to read this file. Always NULL for files opened for write */
   ui8  *pbReadAudioBuffer;   /* Pointer to audio read buffer. Always NULL for files opened for write. */
   void *pReserved;        /* ALL STRUCTURE MEMBERS ARE READ ONLY. */
} AviHandle;


AVI_LIB_API AVI_ERR AVI_Init_Library(void (* fnAVIErrorHandler)(const char *, AVI_ERR), ui32 *pdwMaxStreams);
/* Passes pointer to the error handler function which will be used in all subsequent function calls.
 * Sets maximum number (input + output) streams which can be open simultaneously. Defaults to 16 streams if NULL is passed
 * to the function.
 */

AVI_LIB_API void AVI_Close_Library(void);
/* Frees allocated memory and deletes critical sectioins / mutexes used by the library.
 */

AVI_LIB_API AVI_ERR AVI_OpenInputFile(AviHandle *phFileHandle, ui8 *pbFrameBuffer, ui8 *pbAudioBuffer, char *pszInputFileName); 
                          
/* Opens file name pszInputFileName
 * Errors are returned if the file has already been opened for read or write, if the file name is corrupt / too long, or if 
 * the file is of an unsupported type.
 *
 * See eFileType enumeration for supported types.
 *
 * If NULL is passed to the function in the pbFrameBuffer position, the function automatically allocates sufficient memory to
 * read any frame in the file and returns a pointer to this memory as a member of the handle structure. If a non NULL pointer
 * is passed to the function, this is copied to the pbFrameBuffer member of the handle structure: new memory is not allocated.
 *
 * Passes back a handle to the open file via a pointer. Handle is a structure into which public data members such as 
 * frame width and height are entered by the function.
 */

AVI_LIB_API AVI_ERR AVI_GetFrame(AviHandle *phFileHandle, ui32 dwFrameNumber, ui32 *pdwFrameSize, ui32 *pdwField2Offset);
/*
 * Retrieves specified frame from the opened file.
 * If the input file has not already been opened, AVI_OUT_FILE_OPEN is returned.
 * If dwFrameNumber is 0, the next frame in the sequence is returned.
 * If the frame number requested is beyond the end of the sequence, AVI_END_OF_SEQUENCE is returned.
 * The size of the current frame is returned via the pointer pdwFrameSize.
 * In the case of interlaced MJPEG frames, the offset to the second field in the frame is returned via pdsField2Offset.
 * In the case of interlaced bitmaps eAVI_BGRi, the "mixed" bitmap is returned of the pdwField2Offset parameter is NULL.
 * If a non null parameter is passed, then the library "unmixes" the interlaced lines into concatenated fields.
 */

AVI_LIB_API AVI_ERR AVI_CloseInputFile(AviHandle *phFileHandle);
/*
 * Safely closes the input file and frees the frame buffer associated with that handle.
 */

AVI_LIB_API AVI_ERR AVI_OpenOutputFile(AviHandle *phFileHandle, char *pszOutputFileName, ui32 dwWidth, ui32 dwHeight,
                           ui32 dwMicroSecondsPerFrame, ui32 dwAudioSampleRate, ui32 dwAudioBitsPerSample, ui32 dwChannels, eFile_Type eFileType);
/*
 * Opens an output file of the specified name, type and frame rate.
 * Individual frames are buffered to memory before committing to disk to improve write performance.
 */

AVI_LIB_API AVI_ERR AVI_OpenBufferedOutputFile(AviHandle *phFileHandle, char *pszOutputfileName, ui32 dwWidth, ui32 dwHeight,
                           ui32 dwMicroSecondsPerFrame, ui32 dwAudioSampleRate, ui32 dwAudioBitsPerSample, ui32 dwChannels, eFile_Type eFileType, ui64 qwBufferSize);
/*
 * Opens an output file of the specified type and frame rate.
 * All data is written to memory allocated by the function. Data is written to disk when the file is closed.
 * qwBufferSize bytes of memory are allocated, unless this figure is below a minimum value (currently corresponding to 100MB for frame data).
 * For 32 bit platforms, maximum file size is restricted by the O.S., generally to less than 2GB depending on the operating system and operating system configuration.
 */

AVI_LIB_API AVI_ERR AVI_WriteNextFrame(AviHandle *phFileHandle, ui8 *pbBuffer, ui32 dwFrameSize, ui8 *pbAudioBuffer);
/*
 * Writes the next frame in the sequence, 
 *
 * If possible, Huffman tables should not be written as part of the frame structure as they increase the file size.
 *
 * Audio must be uncompressed pcm, with fixed buffer size set by the choice of frame rate, sample rate, and bits per sample
 * 
 * Audio buffer size = Sample rate * bits per sample * No of Channels/ (Frame rate * 8), this buffer size to be stretched to
 * the next available alignment boundary as follows:
 * 
 * 8 bit mono:    1 byte
 * 16 bit mono:   2 bytes
 * 16 bit stereo: 4 bytes
 *
 
 * Returns AVI_DATA_ERROR if the following conditions are not met:
 *    Frame start and stop markers are not in the correct position
 *    Frame size information embedded in the data structure does not match information given on file open. 
 */

AVI_LIB_API AVI_ERR AVI_WriteNextFrame_Interlaced(AviHandle *phFileHandle, ui8 *pbField1Buffer, ui8* pbField2Buffer, ui32 dwField1Size, ui32 dwField2Size, ui8 *pbAudioBuffer);
/* Writes MJPEG files with concatenated JPEGs in the video data block, field 1 marked with an "AVI1"1 marker and field 2 marked with an "AVI1"2 marker.
 */

AVI_LIB_API AVI_ERR AVI_CloseOutputFile(AviHandle *phFileHandle, ui32 *pdwNumberOfFrames);
/*
 * Completes writes to output file, and safely closes the file. 
 * Returns the total number of frames in the sequence via a pointer.
 */

AVI_LIB_API AVI_ERR AVI_FileCopy(char *pszInputFileName, char *pszOutputFileName, ui32 dwStartFrame, ui32 dwStopFrame);
/* Copies frames from dwStartFrame to dwStopFrame from input file to newly created output file.
 * Existing output files of the same name will be overwritten.
 * All available frames within the dwStartFrame to dwStopFrame will be written, but an AVI_END_OF_SEQUENCE error 
 * will occurr if there is an attempt to access frames not available in the input file.
 */

