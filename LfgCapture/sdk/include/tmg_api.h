/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : TMG Imaging Library
 * Description : Header file for the TMG library (graphics/imaging functions)
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 *
 ****************************************************************************
 */

#ifndef _TMG_API_H_
#define _TMG_API_H_
                

/*
 * Type Definitions
 * ----------------
Type_Definitions(ForBrief)
 */
/* None */


/* TMG Include Files
 * -----------------
 * This file includes only the files that are required in order that the user
 * may may compile his TMG application.
 * tmg_os.h includes operating system files such as "stdio.h" - these are 
 * needed for building the library.
 */

#if defined _TMG_MACOSX && !defined __MWERKS__
#import <tmglib/tmg_os.h>
#import <tmglib/tmg_err.h>

#if defined _TMG_WAVELET_SUPPORT
#import <tmglib/ap10_ip.h>  /* AP10 image processing routines (for wavelet) */
#import <tmglib/tmg_wvlt.h>
#endif

#else
#include <tmg_os.h>
#include <tmg_err.h>
#if defined _TMG_WINDOWS
#include <tmg_ovl.h>
#endif  /* _TMG_WINDOWS */

#if defined _TMG_WAVELET_SUPPORT
#include <ap10_ip.h>        /* AP10 image processing routines (for wavelet) */
#include <tmg_wvlt.h>
#endif

#endif  /* end else _TMG_MACOSX */
/* Note: tmg_pro.h is included at the end of this file, so that definitions
 *       used by it in parameter lists are accessible.
 */


/* TMG Library Revision Level (see also "File History" above)
 * ----------------------------------------------------------
 * Composed of major.minor.sub-minor.  e.g. 0x12015 = v1.20 Rev.15.
 * The sub-minor should be rolled on for every interim/customer release.
 * The minor and major rev levels should be rolled on as part of a major
 * release that includes thorough testing.
 * Note that hex coding is used - this is so that it is easy to extract
 * the three fields.
 *
 * A variable at the top of tmg_snp.c is set to this value.
 *
TMG_Library_Revision_Level(ForBrief)
 */
#define TMG_LIBRARY_REVISION_LEVEL ((ui32) 0x50003)   /* Remember to change version number in resource file */



/*
 * TMG Pixel Formats
 * -----------------
TMG_Pixel_Formats(ForBrief)
 */
#define TMG_NO_IMAGE    ((ui16)  0)
#define TMG_NO_DISPLAY TMG_NO_IMAGE
#define TMG_BILEVEL     ((ui16)  1)
#define TMG_Y8          ((ui16)  2)
#define TMG_PALETTED    ((ui16)  3)
#define TMG_RGB8        ((ui16)  4)
#define TMG_RGB15       ((ui16)  5)
#define TMG_RGB16       ((ui16)  6)
#define TMG_RGB24       ((ui16)  7)
#define TMG_BGR24       ((ui16)  8)
#define TMG_RGBX32      ((ui16)  9)
#define TMG_BGRX32      ((ui16) 10)
#define TMG_XBGR32      ((ui16) 11)
#define TMG_YUV422      ((ui16) 12)  /* 4:2:2 CCIR Digital Video at square pixel sampling */
#define TMG_CMYK32      ((ui16) 13)
#define TMG_Y16         ((ui16) 14)
#define TMG_XRGB32      ((ui16) 15)  /* Used by Mac */
#define TMG_RGGB32      ((ui16) 16)  /* Bayer encoded image, 8 bits per colour plane */
#define TMG_RGGB64      ((ui16) 17)  /* Bayer encoded image, up to 16 bits per colour plane */

#define TMG_MAX_VALUE   ((ui16) 17)  /* Note: Max real TMG pixel format */
/* Also any new formats need adding to TMG_image_check and TMG_set_parameter */

#define TMG_I32         ((ui16) 100)  /* for compression/computation - 1 x i32 per pixel - e.g Y8  */
#define TMG_I32x2       ((ui16) 101)  /* for compression/computation - 2 x i32 per pixel - e.g YUV */
#define TMG_I32x2YUV411 ((ui16) 102)  /* special extension to recognise YUV411 sampling */
#define TMG_I16         ((ui16) 103)  /* for compression/computation - 1 x i16 per pixel - e.g Y8  */
#define TMG_I16x2       ((ui16) 104)  /* for compression/computation - 2 x i16 per pixel - e.g YUV */
#define TMG_Y8_OR_RGB24 ((ui16) 105)  /* special case for paletted conversion (from paletted) */
#define TMG_YUV411      ((ui16) 106)  /* 4:1:1 (limited support - used in JPEG only) */
#define TMG_YYUV        ((ui16) 107)  /* Special for JPEG decompression - YUV 4:2:2 with byte ordering YYUV */
#define TMG_HSI         ((ui16) 108)  /* Limited support (YUV -> HSI) */


/*
 * General TMG #defines
 * --------------------
General_TMG_Definitions(ForBrief)
 */
#define TMG_PI  ((double) 3.14159265358979323846)

#define TMG_MAX_LIBRARY_INSTANCES ((ui32) 1)

#define TMG_LIB_ID        ((ui32) 0x10000000)
#define TMG_LIB_ID_MASK   ((ui32) 0xFF000000)
#define TMG_MAGIC_ERROR   ((ui32) TMG_LIB_ID | ( 0 << 16)) /* MUST be "| 0" */


#define TMG_NULL          ((ui32) NULL)
#define TMG_FIELD_LENGTH  128

#if defined __SC__ || defined _TMG_ARM7
#define TMG_NUM_IMAGES    ((int) 4096)    /* Due to SC compiler - see g_tmg.c */
#else
#define TMG_NUM_IMAGES    ((int) 65536)   /* Maximum number of images */
#endif

#define TMG_ALL_HANDLES         ((Thandle) TMG_NUM_IMAGES+1)   /* unique identifier */
#define TMG_ALL_DATA_STRUCTURES ((Thandle) TMG_NUM_IMAGES+2)   /* unique identifier */

#define TMG_COLOURS    256    /* maximum colourmap size    */

#define TMG_START        1    /* start processing          */
#define TMG_STARTED      2    /* started processing        */
#define TMG_RUN          3    /* continue running strip proccessing            */
#define TMG_RESET        4    /* reset strip processing to start of file/image */
#define TMG_JPEG_RST0    5    /* special for sequence acquisition              */
#define TMG_INIT         6    /* only run initialization code, not main body   */
#define TMG_STRIP        7    /* only process one strip, not init/reset code   */
#define TMG_ASYNC        8    /* Process 1 strip asynchronously */

#define TMG_COLOUR_ARRAY_SIZE 4096     /* 4096 = 4 bits per colour      */
                                       /* 32768 = 5 bits per colour     */
#define TMG_LUT_SIZE             256   /* for gamma, contrast functions */
#define TMG_NUM_LUTS              16
#define TMG_NUM_DISPLAYS          32
#define TMG_NUM_CHROMA_KEYS        4

/*
 * Definitions for display ROI.
 */
#define TMG_ROI_X_START    0
#define TMG_ROI_Y_START    1
#define TMG_ROI_X_LENGTH   2
#define TMG_ROI_Y_LENGTH   3
#define TMG_SIZE_2D_ROI    4

/* Also use same array for end points in TMG_display_Overlay... */
#define TMG_ROI_X_END      2
#define TMG_ROI_Y_END      3



/* TMG_draw font structure types
 * -----------------------------
TMG_draw_font_structure_types(ForBrief)
 */
#define TMG_FONT_STRUCT_6x7      0
#define TMG_FONT_STRUCT_9x14     1
#define TMG_FONT_STRUCT_15x19    2

/* TMG_draw_text dwMode types
 * -----------------------------
TMG_draw_text_dwMode_types(ForBrief)
 */
/* 3-Mar-08: Request for full transparency added plus a couple of others and keeping compatible */
#define TMG_DRAW_BG_OPAQUE           1   /* Gray background  at 127 */
#define TMG_DRAW_BG_TRANSPARENT_0    2   /* 0% transparent,   BG = Background: black  */
#define TMG_DRAW_BG_TRANSPARENT_50   3   /* 50% transparent,  BG = BackGround: 0..127 */
#define TMG_DRAW_BG_TRANSPARENT_100  4   /* 100% transparent, BG = BackGround, 0..255 */

#define TMG_DRAW_BG_TRANSPARENT       TMG_DRAW_BG_TRANSPARENT_100
#define TMG_DRAW_BG_SEMI_TRANSPARENT  TMG_DRAW_BG_TRANSPARENT_50
#define TMG_DRAW_BG_BLACK             TMG_DRAW_BG_TRANSPARENT_0 


/* TMG_draw_timestamp dwMode types
 * -------------------------------
TMG_draw_timestamp_dwMode_types(ForBrief)
 */
#define TMG_DRAW_FLASH_COLON      128


/* General Set/Get Parameters
 * --------------------------
General_SetGet_Parameters(ForBrief)
 */
#define TMG_PIXEL_FORMAT          10
#define TMG_WIDTH                 11
#define TMG_HEIGHT                12
#define TMG_IMAGE_WIDTH           TMG_WIDTH
#define TMG_IMAGE_HEIGHT          TMG_HEIGHT
#define TMG_DEPTH                 13
#define TMG_LINES_THIS_STRIP      14
#define TMG_LINES_PER_STRIP       TMG_LINES_THIS_STRIP  /* Easy concept for user */
#define TMG_BYTES_PER_LINE        15
#define TMG_NUM_BYTES_DATA        16
#define TMG_DATA_WIDTH            17
#define TMG_NUM_FRAMES            18
#define TMG_CURRENT_FRAME         19
#define TMG_CMAP_SIZE             20
#define TMG_JPEG_NUM_BYTES_DATA   21
#define TMG_LIBRARY_REV_LEVEL     22
#define TMG_FIELD_ID              23
#define TMG_ALIGNMENT_LINE        24
#define TMG_ALIGNMENT_LINE_OUTPUT 25   /* next image in chain */
#define TMG_NUM_PLANES            26
#define TMG_JPEG_Q_FACTOR         27


/* Get Pointer Sets/Gets
 * ---------------------
SetGets_GetPointer(ForBrief)
 */
#define TMG_IMAGE_DATA          100
#define TMG_CMAP_STRUCT         101
#define TMG_JPEG_DATA           102
#define TMG_JPEG_CURRENT_PTR    103
#define TMG_PIMAGE_STRUCT       104
#define TMG_PIMAGE_PJPEG_STRUCT 105
#define TMG_JPEG_USER_COMMENT   106

/* Set/Gets For Conversion LUTs
 * ----------------------------
SetGets_ConversionLUT(ForBrief)
 */
#define TMG_RGB16_TO_PALETTED_LUT   200
#define TMG_Y8_TO_PALETTED_LUT      201
#define TMG_Y16_TO_PALETTED_LUT     202
#define TMG_YUV422_TO_RGB16_LUT     203
#define TMG_YUV422_TO_RGB15_LUT     204
#define TMG_YUV422_TO_PALETTED_LUT  205

/* Set/Gets For Chroma Key Parameters
 * ----------------------------------
SetGets_ChromaKeyParameters(ForBrief)
 */
#define TMG_HUE       300
#define TMG_HUE_TOL   301
#define TMG_LUMA      302
#define TMG_LUMA_TOL  303

/* Set/Gets For Display Parameters
 * -------------------------------
SetGets_DisplayParameters(ForBrief)
 */
#define TMG_FRAME_MEMORY_OFFSET 400
#define TMG_RASTER_OP           401
#define TMG_DISPLAY_DIRECT_CAPS 402
#define TMG_DCI_CAPS            TMG_DISPLAY_DIRECT_CAPS
#define TMG_RESERVED_COLOURS    403
#define TMG_DISPLAY_MANAGED_BY_GRAPHICS_CARD 404


/* Y,U,V Get Component (in TMG_CK...)
 * ----------------------------------
YUV_GetComponent_TMG_CK(ForBrief)
 */
#define TMG_Y_COMPONENT  500
#define TMG_U_COMPONENT  501
#define TMG_V_COMPONENT  503

/* TMG_JPEG_get_parameter Parameters
 * ---------------------------------
TMG_JPEG_get_parameter_Parameters(ForBrief)
 */
#define TMG_JPEG_NUM_SCAN_COMPS 600
#define TMG_JPEG_START_OFFSET   601
#define TMG_JPEG_NUM_Q_TABLES   602

/* Font Options Using Flash Graphics Under DOS
 * -------------------------------------------
Font_Options_Using_Flash_Graphics_Under_DOS(ForBrief)
 */ 
#define TMG_FG_6X7       700
#define TMG_FG_8X8       701
#define TMG_FG_8X14      702
#define TMG_FG_8X16      703
#define TMG_FG_15X19     704

/* X Graphics TMG_set_Xid Types
 * ----------------------------
X_Graphics_Xid_Types(ForBrief)
 */
#define TMG_XID_FRAME    800
#define TMG_XID_CANVAS   801
#define TMG_XID_WINDOW   802

/* TMG Filter Orders
 * -----------------
Filter_Orders(ForBrief)
 */
#define TMG_LP_FILTER_ORDER_0 900


/* File Formats
 * ------------
 * Note: TMG_MEMORY is a special type - just an image in memory.
 *
File_Formats(ForBrief)
 */
#define TMG_NOT_DEFINED  ((ui16) 0)
#define TMG_MEMORY       1
#define TMG_FILE         2   /* Refers to from file as opposed to memory */
#define TMG_TIFF         3   /* Non-compressed */
#define TMG_POSTSCRIPT   4
/*#define TMG_TARGA        5    Some type of Targa file. Code stripped out, 5-Aug-09 */
/*#define TMG_TARGA_16BIT  6    Code stripped out, 5-Aug-09 (never used) */
/*#define TMG_TARGA_24BIT  7    Code stripped out, 5-Aug-09 (never used) */
#define TMG_JPEG         8
#define TMG_EPS          9
#define TMG_BMP         10
#define TMG_RAW         11   /* Raw image data - TMG_write_raw_data_file */
/*      TMG_WVLT        12   defined in tmg_wvlt.h */

/*
 * TMG Structure
 * -------------
 * An array of these structures are used globally to allow each instance
 * of the library to be independent. e.g. two processes installing their
 * own error handlers etc.
 * NOTE: At present, this is not fully supported - in fact the error
 * handling forces the use of handle 1.
 *
struct_tTMG(ForBrief)
*/
struct tTMG
{
   ui32 dwLibraryRevLevel;   /* Software library revision level - see g_lfg.c */
   ui32 dwErrCode;           /* To record last error                  */
   ui32 dwOtherLibErrCode;   /* To record error if from different library (e.g. driver) */

#if defined _TMG_WINDOWS || defined _TMG_MAC || defined _TMG_LINUX || defined _TMG_MACOSX || defined _TMG_QNX6
   void (*pFnErrorHandler)(ui32, char*, ui32, char*); /* Pointer to error handler function */
#else /* _TMG_DOS32 */
   void (EXPORT_FN *pFnErrorHandler)(ui32, char*, ui32, char*); /* Pointer to error handler function */
#endif
};


/*
 * TMG Image Structure
 * -------------------
 * Pixel refers to one dot in a colour or mono image - generally in X
 * terminology it means the value fed into an output palette and therefore
 * may not represent an actual intensity.
 * Depth is the colour/grayscale depth of the image in bits - e.g. 24 bit 
 * colour, 8 bit grayscale etc.
 * Images are generally 16 bit aligned (though DIBs have to be 32 bit 
 * aligned).
 *
struct_Timage(ForBrief)
 */
struct Timage  /* NOTE: TMG_copy_Timage_parameters etc may need */
               /* updating if any changes are done here         */
{
  ui16 pixel_format;                  /* TMG_RGB24 or ... etc              */
  ui16 data_width;                    /* 8 usually, or 8..16 for Y16       */
  char creator[TMG_FIELD_LENGTH];     /* sw and version that created image */
  char infilename[TMG_FIELD_LENGTH];  /* input file (and path) of image    */
  char outfilename[TMG_FIELD_LENGTH]; /* output file (and path) of image   */

  ui32 width;              /* width of image                      */
  ui32 height;             /* height of image                     */
  ui16 depth;              /* depth in bits - typically 8 or 24   */
  ui32 dwAlignment;        /* for current image line alignment- in bytes */
  ui32 dwOutAlignment;     /* for next image in processing chain  */
#define TMG_LINE_ALIGNMENT ((ui32) 2) /* NOTE: alignment 16 bit for now */
  ui32 dwSkipBytes;        /* Bytes to skip to keep alignment     */
  ui32 num_frames;         /* for sequences                       */
  ui32 current_frame;      /* current frame - in JPEG or raw data */
  IM_UI8 *Pdata;           /* to actual image data                */
  IM_UI8 *Pcurrent;        /* current position in raw data        */
  ui32 num_bytes_data;     /* for TMG_DATA_STREAM - "continous" block of data */
  ui32 flags;              /* general purpose flags */
#define TMG_LOCKED             ((ui32) 0x001)  /* don't free and malloc        */
#define TMG_IS_JPEG            ((ui32) 0x002)  /* its a JPEG image */
#define TMG_IS_DIB             ((ui32) 0x004)  /* to say we have a DIB pointed */
#define TMG_DIB_NON_INVERTED   ((ui32) 0x008)  /* default DIBs are inverted    */
#define TMG_USE_LUT            ((ui32) 0x010)  /* use LUT for conversion       */
#define TMG_HALF_ASPECT        ((ui32) 0x020)  /* like single "field" of data  */
#define TMG_DATA_STREAM        ((ui32) 0x040)  /* continous block of data      */
#define TMG_DITHER             ((ui32) 0x080)  /* dither option on conversion  */
#define TMG_USER_ALLOCATED_MEM ((ui32) 0x100)  /* Indicates memory buffer is by user */
#define TMG_JPEG_DO_NOT_WRITE_HUFFMAN_TABLE ((ui32) 0x200)  /* Save space and time in AVI MJPEG */
#define TMG_TIFF_LZW           ((ui32) 0x400)  /* JPA: Enables LZW for TIFF */
#define TMG_TIFF_LZW_PREDICTOR ((ui32) 0x800)  /* JPA: Enables LZW predictor for TIFF */ 
/*      TMG_IS_WVLT          ((ui32) 0x100)  Defined in tmg_wvlt.h */


  ui16 cmap_size;                /* size of colourmap                 */
  struct Tcmap *Pcmap;           /* pointer to Tcmap                  */
  struct Tcmap_info *Pcmap_info; /* fast paletting information        */
  ui32 lines_this_strip;         /* lines to process in current strip */
  ui32 bytes_per_line;           /* Accelerator - includes skip bytes */
  ui16 field_id;                 /* 1st / 2nd or full frame   */
#define TMG_FRAME           ((ui16) 0)   /* Full frame   */
#define TMG_1ST_FIELD       ((ui16) 1)   /* Single field */
#define TMG_2ND_FIELD       ((ui16) 2)   /* Single field */
#define TMG_FIELD_1         TMG_1ST_FIELD
#define TMG_FIELD_2         TMG_2ND_FIELD
#define TMG_FIELD_1_OR_2    ((ui16) 3)   /* Single field */
#define TMG_FRAME_FIELDS_12 ((ui16) 4)   /* Full frame - field 1 then 2 */
#define TMG_FRAME_FIELDS_21 ((ui16) 5)   /* Full frame - field 2 then 1 */

  struct Tjpeg *Pjpeg;           /* Pointer to JPEG structure */  
};


/* TMG colourmap definition
 * ------------------------
struct_Timage_cmap(ForBrief)
 */
struct Timage_cmap
{
  ui8 red;
  ui8 green;
  ui8 blue;
  ui8 pad;  /* unused - for 32 bit alignment */
};

struct Tcmap
{
  struct Timage_cmap cmap[TMG_COLOURS];
};


/* TMG LUT Structure
 * -----------------
struct_Ttmg_LUT(ForBrief)
 */
struct Ttmg_LUT
{
  i32 num_elements;    /* Must be signed for limiting fn in TMG_LUT_generate */
  ui16 element_size;   /* In bytes */
  /* pointers to LUT array - size determined on creation */
  ui8 *luma_ui8;       /* Y channel for YUV images */
  ui8 *red_ui8;
  ui8 *green_ui8;
  ui8 *blue_ui8;
  IM_UI16 *luma_ui16;  /* Must be _huge as can have 65536 elements (of 2 bytes each) */
  IM_UI16 *red_ui16;
  IM_UI16 *green_ui16;
  IM_UI16 *blue_ui16;
  i16 brightness;
  i16 contrast;
  i16 gamma;
  i16 ri;   /* red intensity (gamma)   */
  i16 gi;   /* green intensity (gamma) */
  i16 bi;   /* blue intensity (gamma)  */
};


/* Define constants relating to the LUT based enhancement routines */
#define TMG_MAX_BRIGHTNESS     200
#define TMG_DEFAULT_BRIGHTNESS 100
#define TMG_MIN_BRIGHTNESS       0
#define TMG_BRIGHTNESS_SCALE     2

#define TMG_MAX_CONTRAST     200
#define TMG_DEFAULT_CONTRAST 100
#define TMG_MIN_CONTRAST       0
#define TMG_CONTRAST_SCALE    75

#define TMG_MAX_GAMMA     400
#define TMG_DEFAULT_GAMMA 100
#define TMG_MIN_GAMMA      25

#define TMG_MAX_INTENSITY     200
#define TMG_DEFAULT_INTENSITY 100
#define TMG_MIN_INTENSITY       0
#define TMG_INTENSITY_SCALE   100

#define TMG_MAX_COLOUR     200
#define TMG_DEFAULT_COLOUR 100
#define TMG_MIN_COLOUR       0
#define TMG_COLOUR_SCALE   100

/*
struct_Tchroma_key(ForBrief)
 */
struct Tchroma_key
{
  ui16 hue;
  ui16 hue_tol;
  ui16 luma;
  ui16 luma_tol;
};


/*
 * Standard Colourmap Types
 * ------------------------
 * Note starts from 1 because TMG_VGA16 is also used as an image type
 * to describe a display (very rarely - in practice never used).
 * AND because TMG_BLACK (#define 0) is used by TMG_cmap_set_type too.
 *
Standard_Colourmap_Types(ForBrief)
 */
/* #define TMG_BLACK 0 */
#define TMG_VGA16          1   /* 16 colour VGA           */
#define TMG_GRAYSCALE_RAMP 2   /* 256 grayscale ramp      */
#define TMG_332_RGB        3   /* 3:3:2 RGB direct colour */

/*
 * Colourmap generation definitions
 * --------------------------------
 * The colourmap generation functions only work under 32 bit DOS
 * or Windows.
 * Colour histogram element for optimum colourmap generation.
 *
struct_Tcolour_hist_element(ForBrief)
 */
struct Tcolour_hist_element
{
  ui32 value;        /* value of RGB or Grayscale data       */
  ui32 occurrences;  /* no of occurrences of colour 'rgb'    */
  i32  cmap_index;   /* index to colour in Timage colour map */
};

/* Info used in the generation and display of colourmapped pixels */
struct Tcmap_info
{
  struct Tcolour_hist_element histogram[TMG_COLOUR_ARRAY_SIZE];
  ui32 dwArraySize;            /* number of elements in array */
  ui8  red_bits;               /* bits for colour accuracy    */
  ui8  green_bits;
  ui8  blue_bits;
  ui8  gray_bits;
  ui16 no_of_free_colours;     /* the number of colours to use         */
  ui32 no_of_colours;          /* number in the image after our shifts */
  ui16 percentage_paletted;    /* % of pixels indexed in the palette   */
  ui16 direct_colour_3_3_2;
  ui16 dither;                 /* FALSE or one of several algorithms   */
};

/*
 * A squares LUT (for geometric mean) for closest colour matching
 * - note 512 hard coded - this represents 8 bit input data.
 */
struct Tsquare_lut
{
  ui16 red[512];
  ui16 green[512];
  ui16 blue[512];
};


/*
 * Histogram structure.  Where planes can be the following:
 * Plane 1: H R Y
 * Plane 2: S G U
 * Plane 3: I B V
 * (Depending on context.)
 *
struct_tTMG_Histogram(ForBrief)
 */
struct tTMG_Histogram {
#define TMG_HIST_PLANE1_LENGTH     ((ui32) 360)
#define TMG_HIST_PLANE2AND3_LENGTH ((ui32) 256)
   /* This could be a two dimension array, but that can be speed inefficient */
   ui32  dwPlane1[TMG_HIST_PLANE1_LENGTH];  /* 0 to 359 degrees for hue */
   ui32  dwPlane2[TMG_HIST_PLANE2AND3_LENGTH];
   ui32  dwPlane3[TMG_HIST_PLANE2AND3_LENGTH];
   ui32  dwPlane1Integral;
   ui32  dwPlane2Integral;  /* Area under respective "curves" */
   ui32  dwPlane3Integral;
};


/* Structure to return image averaging calculations
 * ------------------------------------------------
 *
struct_tAverages(ForBrief)
 */
struct tTMG_Averages {
   ui32  dwPlane1;   /* R, H, Y */
   ui32  dwPlane2;   /* G, S, U */
   ui32  dwPlane3;   /* B, I, V */
};


/* Structure to return results of correlation functions
 * ----------------------------------------------------
 *
struct_tCorrelation(ForBrief)
 */
struct tTMG_Correlation {
   ui32  dwX1;        /* Search area in target image - top left */
   ui32  dwY1;
   ui32  dwX2;        /* Search area in target image - bottom right */
   ui32  dwY2;
   ui32  dwX;         /* Best match X coord from top left */
   ui32  dwY;         /* Best match Y coord from top left */
   i32   nThreshold;  /* Threshold used for offset binary conversion   */
                      /* Typically half the maximum value in the image */
   i32   nMaxSum;     /* Highest number in correlation algorithm       */
};

/* TMG font structure
 * ------------------
 *
TMG_font_structure(ForBrief)
 */
struct sTMG_Font
{
   ui32  dwCharHeight;     /* Height of each character in font   */
   ui32  dwCharWidth;      /* Width  of each character in font   */
   ui32  dwBytesPerChar;   /* Number of bytes defining each character in font table */
   ui8  *pbFontData;       /* Pointer to beginning of font table */
};


/* Command Line Parsing Defines
 * ----------------------------
 * These are used by the console command line utility functions
 * TMG_Util_ParseCommand() and TMG_Util_ExtractOptions().
 *
struct_tTMG_Cmd(ForBrief)
 */
#define  TMG_CMD_MAX_FIELD_LENGTH 128
#define  TMG_CMD_MAX_OPTIONS        3
#define  TMG_CMD_MAX_PARAMS         3

struct tTMG_Cmd
{
   i32  nNumFields;   /* Total number of fields (= argc)       */
   i32  nNumOptions;  /* Number of -<optons>                   */
   i32  nNumParams;   /* Number of parameters - e.g. filenames */
   char szCmdName[TMG_CMD_MAX_FIELD_LENGTH];
   char szOptions[TMG_CMD_MAX_OPTIONS][TMG_CMD_MAX_FIELD_LENGTH];
   char szParams[TMG_CMD_MAX_PARAMS][TMG_CMD_MAX_FIELD_LENGTH];
};


/*
 * YUV to RGB LUT Size Definitions
 * -------------------------------
 * These bits define the depth of Y,U and V in various modes for the
 * conversion to RGB.  The ones with "16" as the suffix are for the 16
 * bit output LUT (i.e. RGB 5:6:5 and RGB 5:5:5) and the "8" ones are
 * for the 8 bit LUT (paletted/indexed RGB).
 *
YUV_to_RGB_LUT_Size_Definitions(ForBrief)
 */
#if defined _DOS16        /* 16 bit DOS */
#define TMG_Y_BITS16  5
#define TMG_U_BITS16  5
#define TMG_V_BITS16  5
#define TMG_Y_BITS8   5
#define TMG_U_BITS8   5
#define TMG_V_BITS8   5
#elif defined _WIN31       /* 16 bit Windows 3.1 */
#define TMG_Y_BITS16  5
#define TMG_U_BITS16  5
#define TMG_V_BITS16  5
#define TMG_Y_BITS8   5
#define TMG_U_BITS8   5
#define TMG_V_BITS8   5
#else                     /* All 32 bit memory models */
#define TMG_Y_BITS16  7
#define TMG_U_BITS16  6
#define TMG_V_BITS16  6
#define TMG_Y_BITS8   5
#define TMG_U_BITS8   5
#define TMG_V_BITS8   5
#endif
#define TMG_YUV_TO_RGB16_LUT_SIZE     ((ui32) 1 << (TMG_Y_BITS16 + TMG_U_BITS16 + TMG_V_BITS16))
#define TMG_YUV_TO_PALETTED_LUT_SIZE  ((ui32) 1 << (TMG_Y_BITS8 + TMG_U_BITS8 + TMG_V_BITS8))

/* RGB16/Y8 to paletted LUT Definitions
 * ------------------------------------
 */
#define TMG_RGB16_TO_PALETTED_LUT_SIZE ((ui32) 65536)
#define TMG_Y8_TO_PALETTED_LUT_SIZE    ((ui32) 256)

/* UV to Hue (angle) LUT
 * ---------------------
 * Note the size is the same in terms of number of entries, but the 
 * resolution is 2 bytes (128k total) normally and 1 byte (64k) under
 * DOS/Windows 3.1.
 *
UV_to_Hue_LUT(ForBrief)
 */
#ifdef _CPU16
#define TMG_UV_TO_HUE_LUT ((ui32) 65536)
#else
#define TMG_UV_TO_HUE_LUT ((ui32) 65536)
#endif

/*
 * TIFF Definitions
 * ----------------
TIFF_Definitions(ForBrief)
 */
#define TIFF_Motorola  (ui16) 'M'
#define TIFF_Intel     (ui16) 'I'
#define TIFF_Short             0x03
#define TIFF_Long              0x04     
#define TIFF_NewSubFileType    0x00fe
#define TIFF_ImageWidth        0x0100
#define TIFF_ImageLength       0x0101
#define TIFF_BitsPerSample     0x0102
#define TIFF_Compression       0x0103
#define TIFF_PhotometricInterp 0x0106
#define TIFF_StripOffset       0x0111
#define TIFF_SamplesPerPixel   0x0115
#define TIFF_StripByteCounts   0x0117
#define TIFF_RowsPerStrip      0x0116
#define TIFF_Predictor         0x013d
#define TIFF_ColourMap         0x0140

/* Photometric interpretations */
#define TIFF_photmet_grey_inverse  0x00
#define TIFF_photmet_grey_normal   0x01
#define TIFF_photmet_rgb           0x02
#define TIFF_photmet_paletted      0x03


/*
 * JPEG defines
 * ============
 * JPEG image file marker code definitions.
 *
JPEG_Marker_Codes(ForBrief)
 */

/* marker code prefix */
/* ------------------ */
#define  PREFIX   (ui8) 0xFF


/* Frame Signalling */
/* ---------------- */
#define  SOF0  (ui8) 0xC0   /* Baseline DCT   */
#define  SOF1  (ui8) 0xC1   /* Extended sequential DCT */
#define  SOF2  (ui8) 0xC2   /* Progessive DCT */
#define  SOF3  (ui8) 0xC3   /* Spatial DCT    */


/* Huffman table definition */
/* ------------------------ */
#define  DHT   (ui8) 0xC4   /* Define Huffman Table */


/* Other Markers */
/* ------------- */
#define  SOI   (ui8) 0xD8   /* Start of image */
#define  EOI   (ui8) 0xD9   /* End of image   */
#define  SOS   (ui8) 0xDA   /* Start of scan  */
#define  DQT   (ui8) 0xDB   /* Define quantization table */
#define  DNL   (ui8) 0xDC   /* Define number of lines  */
#define  DRI   (ui8) 0xDD   /* Define restart interval */
#define  DHP   (ui8) 0xDE   /* Define hierachical progession */
#define  EXP   (ui8) 0xDF   /* Expand reference image */

#define  COM   (ui8) 0xFE   /* Comment */


/* Restart Markers */
/* --------------- */
#define  RST0  (ui8) 0xD0
#define  RST1  (ui8) 0xD1
#define  RST2  (ui8) 0xD2
#define  RST3  (ui8) 0xD3
#define  RST4  (ui8) 0xD4
#define  RST5  (ui8) 0xD5
#define  RST6  (ui8) 0xD6
#define  RST7  (ui8) 0xD7


/* Application Signalling */
/* ---------------------- */
#define  APP0  (ui8) 0xE0   /* JFIF file format */
#define  APP1  (ui8) 0xE1
#define  APP2  (ui8) 0xE2
#define  APP3  (ui8) 0xE3
#define  APP4  (ui8) 0xE4
#define  APP5  (ui8) 0xE5
#define  APP6  (ui8) 0xE6
#define  APP7  (ui8) 0xE7
#define  APP8  (ui8) 0xE8
#define  APP9  (ui8) 0xE9
#define  APP10 (ui8) 0xEA
#define  APP11 (ui8) 0xEB
#define  APP12 (ui8) 0xEC
#define  APP13 (ui8) 0xED
#define  APP14 (ui8) 0xEE
#define  APP15 (ui8) 0xEF


/* JPEG Structure */
/* -------------- */
#define JPEG_MAX_COMPS 4   /* 0..3 */
#define TMG_JPEG_Q_FACTOR_SCALING      65536  /* Power(2, TMG_JPEG_Q_FACTOR_SCALING_BITS) */
#define TMG_JPEG_Q_FACTOR_SCALING_BITS    16  /* Matched to TMG_JPEG_Q_FACTOR_SCALING     */
#define TMG_JPEG_USER_COMMENT_LEN         32  /* 32 bytes for user info in comment field  */
#define TMG_JPEG_JFIF_LENGTH              18  /* See TMG_JPEG_JFIF_String[]               */

/* TMG JPEG Structure
 * ------------------
Struct_Tjpeg(ForBrief)
 */
struct Tjpeg
{
#ifndef _MVP_MP        /* So that it works on C80            */
  FILE *Pfile;         /* ptr to the data in the file        */
#endif
  ui32 start_offset;   /* offset from start of file to data  */
  IM_UI8 *Pdata;       /* actual compressed image data       */
  IM_UI8 *Pcurrent;    /* to record current pointer position */
  ui32 num_bytes_data; /* amount of compressed data - not including EOI (FF D9) */
                       /* (which isn't stored with the data anyway) */
                       /* sequence JPEG does include the RST markers though */
  ui16 Q_factor;

  ui8 *Pjfif;          /* ptr to JFIF info - if NULL - does not exist */

  char szUserComment[TMG_JPEG_USER_COMMENT_LEN];  /* For user info - pre-pended before ASL info */

  /* Frame Parameters - assumes only 1 frame! */
  ui16 frame_field_len;
  ui16 frame_precision;
  ui16 hsamples[JPEG_MAX_COMPS];
  ui16 vsamples[JPEG_MAX_COMPS];

  ui16 Q_field_length;
  ui16 QTable_index[JPEG_MAX_COMPS];  /* select which Q table */
  ui16 num_QTabs;
  ui8  QTable[JPEG_MAX_COMPS][64];    /* actual Q tables      */
  i32  nScaledQTableY[64];            /* Pre-scaled Q values for fast division */
  i32  nScaledQTableC[64];            /* Pre-scaled Q values for fast division */

  ui16 huffman_field_length;
  ui16 huffman_table_indices[5];
  ui8  Htable_lengths[4][16];
  ui8  Htable_symbols[4][256];

  ui16 scan_field_len;
  ui16 scan_comps;
  ui8 start_of_spectral_sel;
  ui8 end_of_spectral_sel;
  ui8 succ_approx;
};

/*
 * The structure for each Huffman codeword - TMG JPEG Software.
 */
struct Tcodetable
{
  ui16 length;
  ui32 code;
};

/* The structure decoding Huffman codes - TMG JPEG Software
 * --------------------------------------------------------
 */
struct Tdecodetable
{
   ui8  runlength;  /* only used by AC coeffs */
   ui8  category;
   ui16 codelength;
};


/*
 * Display Structure
 * =================
 */
/*
 * DOS VESA supported graphics modes:
 */
#define TMG_DOS_PROMPT            ((ui16) 1)
#define TMG_640x480x8_GRAYSCALE   ((ui16) 2)
#define TMG_640x480x8_RGB         ((ui16) 3)
#define TMG_640x480x16            ((ui16) 4)
#define TMG_640x480x24            ((ui16) 5)
#define TMG_800x600_VGA           ((ui16) 6)
#define TMG_800x600x8_GRAYSCALE   ((ui16) 7)
#define TMG_800x600x8_RGB         ((ui16) 8)
#define TMG_800x600x15            ((ui16) 9)
#define TMG_800x600x16           ((ui16) 10)
#define TMG_800x600x24           ((ui16) 11)
#define TMG_1024x768x8_GRAYSCALE ((ui16) 12)
#define TMG_1024x768x8_RGB       ((ui16) 13)
#define TMG_1280x1024x8_RGB      ((ui16) 14)

/*
 * Solaris X Graphics
 */
#define TMG_X_GRAYSCALE       ((ui16) 15)  /* for backwards compatibility */
#define TMG_X_WINDOWS         ((ui16) 15)  /* covers all X Windows modes  */

/*
 * Windows 3.1 Graphics
 */
#define TMG_WIN31                17

/*
 * Some useful #defines for determining display types.
 */
#define TMG_RED555    (0x01f << 10)
#define TMG_GREEN555  (0x01f << 5)
#define TMG_BLUE555   (0x01f)
#define TMG_RED565    (0x01f << 11)
#define TMG_GREEN565  (0x03f << 5)
#define TMG_BLUE565   (0x01f)
#define TMG_RED664    (0x03f << 10)
#define TMG_GREEN664  (0x03f << 5)
#define TMG_BLUE664   (0x0f)

/*
 * DOS FG Graphics
 */

/* Minimum definitions so that TMG library will work OK with Flash Graphics
 * but not need to be compiled with _TMG_FG_GRAPHICS - otherwise the linker 
 * fails to resolve fg_version_sync.
 */
#ifdef _TMG_DOS32
#ifndef _TMG_FG_GRAPHICS
#if __WATCOMC__ || _MSC_VER || __GNUC__ /* If Watcom, Microsoft or DJGPP */
 #define _FG_CDECL
#else
 #define _FG_CDECL __cdecl
#endif
typedef int _FG_CDECL fg_coord_t;
typedef fg_coord_t fg_box_t[4];
#endif  /* if Flash Graphics not defined */
#endif  /* if DOS (16 or 32) */


#if defined _TMG_WINDOWS
/* Overlay Display
 */
typedef struct ASL_Display* ASL_DisplayHandle;
struct ASL_DisplayFormat;
#endif  /* _TMG_WINDOWS */


/* TMG Display Structure
 * ---------------------
struct_Tdisplay(ForBrief)
 */
struct Tdisplay
{
  ui16 pixel_format; /* Colour organisation etc (or TMG_800x600x16 for DOS) */
                     /* TMG_RGB16 etc for Windows 3.1 */
  ui16 width;        /* screen size */
  ui16 height;       /* screen size */
#define TMG_AUTO_WIDTH  ((i16) 32767) /* auto means full height image */
#define TMG_AUTO_HEIGHT ((i16) 32767) /* bigger than any image - but still an i16 */
                                      /* also used for file reading and ROI */
  ui16 depth;      /* number of colours = 2^depth */
  ui32 flags;      /* various flags: */
#define TMG_DISPLAY_IS_COLOUR ((ui32) 0x01)
#define TMG_DISPLAY_DIRECT    ((ui32) 0x02)  /* bypass GDI layer   */
#define TMG_STRETCH           ((ui32) 0x04)  /* use StretchDIB/Blt */
#define TMG_KEEP_ASPECT       ((ui32) 0x08)  /* preserve aspect ratio */
  ui16 x_start;
  ui16 x_length;
#define TMG_AUTO_CENTRE ((ui16) 32766) /* auto-centre under DOS (only) */
  ui16 x_end;      /* used internally for DOS display */
  ui16 y_start;    /* used internally for DOS display */
  ui16 y_length;   /* ROI */
  ui16 y_end;      /* ROI */
#define TMG_ROI_INIT ((ui16) 0)  /* set the display ROI to display a full image, positioned top left */
#define TMG_ROI_SET  ((ui16) 1)  /* set an ROI */
  ui16 lines_so_far;
  ui16 ImageWidth;   /* width of last displayed image */
  ui16 ImageHeight;

  ui8  *pbVideoOverlayBuffer;
  ui32 dwVideoOverlayStride;


#ifdef _TMG_DOS32        /* So that libraries can be used with or without FG */
  fg_box_t b_box;    /* bounding box for image */
#endif
#ifdef _TMG_X_GRAPHICS   /* X Imaging */
  Display *dpy;
  int      screen;
  Visual  *visual;
  Colormap XCmap;                    /* colourmap installed at start */
  Colormap OurXCmap;                 /* our virtual colourmap */
  GC       gc;                       /* graphics context  */
  XColor   XCells[TMG_COLOURS];      /* Full X colourmap  */
  XColor   OurXCells[TMG_COLOURS];   /* our R/W colourmap */
  unsigned long XPixelLUT[TMG_COLOURS]; /* Mapping our 'colours' to pixel values   */
  unsigned long XPlaneMasks[8];
  int      XFree;                    /* number of colours we have */
  Window   XidFrame;                 /* X ID of XView frame  */
  Window   XidCanvas;                /* X ID of XView canvas */
  Window   XidDisplay;               /* X ID set by repaint callback */
  XImage   *OurXImage;               /* we use this for display and hide from user */
  ui16     OurXWidth;                /* to keep track of X Image size */
  ui16     OurXHeight;
#ifdef _XSHM
  XShmSegmentInfo ShmInfo;           /* For fast ROPs using XShm X extensions */
#endif
#endif  /* _TMG_X_GRAPHICS */

#if defined _TMG_OS2
   HWND      hWnd;            /* handle of associated display window */
   HBITMAP   hbm;
   PVOID     pPels;           /* pointer to display bitmap data */
   PBITMAPINFOHEADER2 pbmih;  /* bitmap (DIB) header */
   ui16      BitmapType;
   Tboolean  Paint;
   ui16      BitsPerPixel;
   ui16	    DisplayType;
   HDIVE     hDive;       /* handle to DIVE display library instance */
   ui32      ImageBuffer; /* for use by DIVE */
#endif

#if defined _MAC_LVR || defined _MACINTOSH
  GWorldPtr pgwDst;
  PixMapHandle hpmDst;	/* Handle to pixmap describing output */
  PixMapHandle hpmImg;	/* Handle to pixmap describing this image - for CopyBits */
  PixMapHandle hpmInterlace;	/* Handle to interlaced output pixmap  */
  RgnHandle hrgnMask;	/* Handle to mask region for CopyBits */
#endif

#if defined _TMG_MACOSX && !defined __MWERKS__
/* Use OpenGl for the display */
   CGLContextObj Context;        /* The Apple GL Context associated with this display */
   Thandle       hDisplayImage;  /* Temporary handle used if an image conversion id required */
#endif

#if defined _TMG_GL_GRAPHICS
/* Use OpenGl for the display */
   GLXContext    Context;        /* The X GL Context associated with this display */
   Thandle       hDisplayImage;  /* Temporary handle used if an image conversion id required */
#endif


/* _MAC signifies MS Mac cross-compiler - compiles under Wintel */
#if defined (_WINDOWS) && !defined (_MAC) /* && defined _WIN31  Windows 3.1/NT/95 - note not just _WIN31 so that pci_asm can compile */
   HDC  hDC;           /* Handle to device context */
   HWND hWnd;          /* Handle to device context */
   HDC  hMemoryDC;     /* Handle to memory device context - for BitBlt */
   HBITMAP hBitmap;    /* Handle to bitmap - for BitBlt */
   HBITMAP hOldBitmap; /* Handle to "replaced" bitmap - for BitBlt */
   ui16 bitspixel;
   ui16 planes;
   ui16 colorres;      /* represents max resolution of DACs */
#ifdef _TMG_WINDOWS
   LPDIRECTDRAW        lpDirectDraw;        /* DirectDraw object */
   LPDIRECTDRAWSURFACE lpDDPrimarySurface;  /* DirectDraw primary surface */
   LPDIRECTDRAWCLIPPER lpDDClipperList;     /* Clipper list for primary surface */
   LPDIRECTDRAWSURFACE lpDDImageSurface;    /* Client memory surface */
   /* Overlay Display */
   ASL_DisplayHandle hOverlay;
   struct ASL_DisplayFormat* pFormatTable;
#endif	/* end _TMG_WINDOWS */
#ifdef _WIN31
   HWINWATCH hWW;      /* DCI: Handle to WinWatch */
   HGLOBAL hClipList;  /* DCI: List of clipping rects */
   HDC hdcDCIProvider; /* DCI: Handle to DCI provider */
   DCIPRIMARY FAR *pSurface; /* DCI: Primary surface used by DCI */
#endif  /* _WIN31   */
#endif  /* _WINDOWS */
   ui32 frame_memory_offset;  /* offset from base address of PCI graphics */
                              /* card to start of frame memory.           */
   ui16 raster_op;
   ui32 fManagedByGraphicsCard;
};

/* Standard 16 colour VGA index */
#define TMG_BLACK    0
#define TMG_BLUE     1
#define TMG_GREEN    2
#define TMG_CYAN     3
#define TMG_RED      4
#define TMG_MAGENTA  5
#define TMG_YELLOW   6
#define TMG_WHITE    7
#define TMG_GRAY            8
#define TMG_LIGHT_BLUE      9
#define TMG_LIGHT_GREEN    10
#define TMG_LIGHT_CYAN     11
#define TMG_LIGHT_RED      12
#define TMG_LIGHT_MAGENTA  13
#define TMG_LIGHT_YELLOW   14
#define TMG_LIGHT_WHITE    15

/*
 * Direct PCI write raster operations:
 */
#define TMG_COPY       (0x8000)
#define TMG_LAT_INV   ((0x4000) | TMG_COPY)
#define TMG_VERT_INV  ((0x2000) | TMG_COPY)
#define TMG_ROTATE180 (TMG_LAT_INV | TMG_VERT_INV)
#define TMG_MASKBL    ((0x1000) | TMG_COPY)
#define TMG_CHECKER0  ((0x0800) | TMG_COPY)
#define TMG_CHECKER1  ((0x0400) | TMG_COPY)

/*
 * Duplicate of BITMAPINFOHEADER so that we do not need windows.h for
 * non-Windows applications.
 */
#if defined _TMG_OS2
typedef BITMAPINFOHEADER2 TBitmapInfoHeader;
#else
typedef struct TBitmapInfoHeader
{
   ui32 biSize;
   i32  biWidth;
   i32  biHeight;
   i16  biPlanes;
   i16  biBitCount;
   ui32 biCompression;
   ui32 biSizeImage;
   i32  biXPelsPerMeter;
   i32  biYPelsPerMeter;
   ui32 biClrUsed;
   ui32 biClrImportant;
} TBitmapInfoHeader;
#endif  /* end TBitmapInfoHeader for OS/2 */

#ifndef BI_RGB     /* if not compiled with Windows 3.1 include files */
#define BI_RGB     ((ui16)0)
#define BI_RLE8    ((ui16)1)
#define BI_RLE4    ((ui16)2)
#endif


typedef struct Trgbquad
{
   ui8  rgbBlue;
   ui8  rgbGreen;
   ui8  rgbRed;
   ui8  rgbReserved;
} Trgbquad;

/*
 * For Windows 3.1:
 */
#define TMG_HORZRES   1
#define TMG_VERTRES   2
#define TMG_BITSPIXEL 3
#define TMG_PLANES    4
#define TMG_ASPECTX   5
#define TMG_ASPECTY   6
#define TMG_COLORRES  7


/* TMG MJPEG Information
 * ---------------------
 */
#define TMG_MJPEG_MAX_STREAMS 8
#define TMG_MJPEG_NUM_BUFFERS 2

#define TMG_MJPEG_JPEG_USER_COMMENT_LEN 128  /* For user comments/spare, currently unused */
#define TMG_MJPEG_GPS_RMC_LEN 128            /* GPS RMC string */
#define TMG_MJPEG_COMMENT_LEN  (TMG_MJPEG_JPEG_USER_COMMENT_LEN+TMG_MJPEG_GPS_RMC_LEN+40)  /* Frame number, timestamp etc */
#define TMG_MJPEG_WRAPPER_OVERHEAD (2+2+TMG_MJPEG_COMMENT_LEN+2+2)  /* SETTING: COM+field_length, commentstring, SOS, EOI */
#define TMG_MJPEG_X1FWD_OFFSET  ((4 * 4) + 4)               /* SETTING: Offset to x1rev parameter */
#define TMG_MJPEG_X1REV_OFFSET  ((7 * 4) + 4)               /* SETTING: Offset to x1fwd parameter */
#define TMG_MJPEG_NUM_BYTES_JPEG_DATA_OFFSET ((1 * 4) + 4)  /* SETTING: Offset to dwNumBytesJpegData parameter */

#define TMG_MJPEG_LAST_IMAGE 1  /* Used by TMG_MJPEG_ImageRead() */


/* Local MJPEG Structure
 * ---------------------
 */
struct tTMG_MJPEG
{
   ui32 hHandle;            /* Handle to this MJPEG stream */
   struct tTMG_MJPEG *psIntMjpeg;  /* Forward pointer - can be used by user to write to */
   char szFilename[256];    /* Filename of MJPEG file      */
   i32  nQFactor;           /* JPEG Q factor for the whole stream */
   FILE *pFile;             /* Sequence file pointer */
   ui32 hJpegImage;         /* Used on MJPEG_Open() and for general use */
   i32  nSequenceLength;    /* Number of frames in the sequence (1..N) */

   ui8  szCommentString[TMG_MJPEG_COMMENT_LEN];   /* Contains frame number etc - see tmg_mjpg.c */

   /* Following section stored in MJPEG comment field */
   i32  nCurrentFrameNum;   /* Number actually written or read */
   ui32 dwNumBytesJpegData; /* Of current image */
   ui32 dwTimestampSecs;    /* This is always a 32 bit integer (not time_t) */
   ui32 dwCameraNumber;     /* Can be useful */
   ui32 dwOffsetFwdx1;      /* Fast index */
   ui32 dwOffsetFwdx10;     /* Fast index */
   ui32 dwOffsetFwdx100;    /* Fast index */
   ui32 dwOffsetRevx1;      /* Fast index */
   ui32 dwOffsetRevx10;     /* Fast index */
   ui32 dwOffsetRevx100;    /* Fast index */
   ui8  szGpsRmcString[TMG_MJPEG_GPS_RMC_LEN];
   ui8  szUserCommentString[TMG_MJPEG_JPEG_USER_COMMENT_LEN];

   i32  nImageWidth;        /* In pixels */
   i32  nImageHeight;       /* In pixels */
   i32  fColor;             /* Boolean, colour = True */
   i32  fTimestamp_is_UserSet;  /* Indicates that the application is filling in the dwTimestampSecs parameter */
   i32  fInUse;

   i32 nMemoryBufferIndex;
   i32 nJpegBufferSize;     /* Size of pbJpegBuffer */
   ui8 *pbJpegBuffer[TMG_MJPEG_NUM_BUFFERS];
   i32 naNumBytesJpegData[TMG_MJPEG_NUM_BUFFERS];
};


/* TMG MRAW Information
 * ---------------------
 */
#define TMG_MRAW_MAX_STREAMS 8
#define TMG_MRAW_HEADER_BUFFER_SIZE   512     /* SETTING */
#define TMG_MRAW_FILE_IDENTIFIER "MRAW www.activesilicon.com "
#define TMG_MRAW_HEADER_SIZE_PER_FILE     64  /* SETTING */
#define TMG_MRAW_HEADER_PADDING_PER_FILE   1  /* SETTING */
#define TMG_MRAW_HEADER_SIZE_PER_IMAGE   320  /* SETTING */
#define TMG_MRAW_HEADER_PADDING_PER_IMAGE 38  /* SETTING */


/* Local MRAW Structure
 * --------------------
 */
struct tTMG_MRAW
{
   ui32 hHandle;            /* Handle to this MJPEG stream */
   struct tTMG_MRAW *psIntMraw;  /* Forward pointer - can be used by user to write to */
   char szFilename[256];    /* Filename of MRAW file       */
   FILE *pFile;             /* Sequence file pointer */
   i32  nSequenceLength;    /* Number of frames in the sequence (1..N) */

   struct Timage *psImage;  /* Pointer to current image being recorded */
   ui32 hImage;             /* TMG image for use on replay */

   /* Following section stored in the Header per Image */
   i32  nCurrentFrameNum;   /* Number actually written or read */
   ui32 dwTimestampSecs;    /* This is always a 32 bit integer (not time_t) */
   ui32 dwOffsetx1;         /* Jump Index - to skip forwards/backwards in sequence file */
   ui32 dwCameraNumber;     /* Can be useful */
   ui32 dwIndexMarker;      /* Can be useful to mark events */
   ui32 dwFlagsPerImage;    /* Can be useful to mark events */
   ui8  szGpsRmcString[TMG_MJPEG_GPS_RMC_LEN];
   ui8  szUserCommentString[TMG_MJPEG_JPEG_USER_COMMENT_LEN];

   i32 nHeaderSizePerFile;
   i32 nHeaderSizePerImage;
   ui8 bHeaderBufferPerFile[TMG_MRAW_HEADER_BUFFER_SIZE];
   ui8 bHeaderBufferPerImage[TMG_MRAW_HEADER_BUFFER_SIZE];

   /* Some other useful information */
   i32  nImageWidth;        /* In pixels */
   i32  nImageHeight;       /* In pixels */
   i32  fColor;             /* Boolean, colour = True */

   i32  fTimestamp_is_UserSet;  /* Indicates that the application is filling in the dwTimestampSecs parameter */
   i32  fInUse;
};


/*
 * Macros
 * ------
MACROS(ForBrief)
_TMG_GetAndCheckpsImage(ForBrief)
_TMG_Limit_to_0(ForBrief)    
_TMG_Limit_to_255(ForBrief)
_TMG_Limit_0_to_255(ForBrief)
_TMG_Limit_0_to_359(ForBrief)
_TMG_Max(ForBrief)
_TMG_Min(ForBrief)
_TMG_Min3(ForBrief)
_TMG_IsEven(ForBrief)
_TMG_IsOdd(ForBrief)
_TMG_LengthOf()
*/
#define _TMG_GetAndCheckpsImage(s,Pi,Hi)   {if ((Pi = Pimage_array[Hi]) == NULL) TMG_Roe(TMG_ERROR_BAD_HANDLE,"TMG handle references null pointer",s)}

#define _TMG_Limit_to_0(x)     { if ((x) < 0) { (x) = 0; } }
#define _TMG_Limit_to_255(x)   { if ((x) > 255) { (x) = 255; } }
#define _TMG_Limit_0_to_255(x) { if ((x) < 0) { (x) = 0; } else if ((x) > 255) { (x) = 255; } }
#define _TMG_Limit_0_to_359(x) { if ((x) < 0) { (x) = 0; } else if ((x) > 359) { (x) = 359; } }

#define _TMG_Max(a,b)      (((a) > (b)) ? (a) : (b))
#define _TMG_Min(a,b)      (((a) < (b)) ? (a) : (b))
#define _TMG_Min3(a,b,c)   (_TMG_Min((a),_TMG_Min((b),(c))))

#define _TMG_IsOdd(a)  (((ui32)(a) & (ui32)0x01) ? TRUE : FALSE)
#define _TMG_IsEven(a)  (!_TMG_IsOdd(a))
#define _TMG_LengthOf(_a) ((int)( sizeof(_a)/sizeof(_a[0]) ))

#if defined _TMG_MACOSX && !defined __MWERKS__
#import <tmglib/tmg_pro.h>
#else 
#include <tmg_pro.h>
#endif

#endif  /* _TMG_API_H_ */

