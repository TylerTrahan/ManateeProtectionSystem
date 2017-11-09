/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : LFG TMG Library
 * Description : TMG library interface.
 * Updated     : 04-Jun-2015
 *
 * Copyright (c) 2015 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * This LFG interface file provides an interface to the TMG Imaging Library
 * so that TMG functions for display, file saving, compression etc may be
 * used.
 * This file is not compiled into the library (DLL/.so) so that it 
 * remains independent and able to operate with any third party imaging 
 * library.
 * This file acts as an example of how to extract the necessary information
 * from the LFG library in order to interface to typical imaging libraries.
 *
 * Note this file is 100% portable between LFG supported operating systems.
 *
 ****************************************************************************
 */

#if defined _LFG_MACOSX && !defined __MWERKS__
#include <lfglib/lfg_api.h>
#import <tmglib/tmg_api.h>
#else
#include <lfg_api.h>
#include <tmg_api.h>
#endif

/*
 * Provide a level of abstraction for the TMG create function.
 * We create a JPEG image type which is a superset of the standard image
 * in case the user wants to perform JPEG compression/decompression on the
 * images.
 */
ui32 LFG_EXPORT_FN LFG_TMG_ImageCreate( ui32 *phImage)
{
   *phImage = TMG_JPEG_image_create();
   return(LFG_OK);
}


/*
 * Provide a level of abstraction for the TMG image destroy function, but
 * also, importantly, the internal image pointer is set to NULL as this
 * should not be destroyed by the TMG library but by the LFG library,
 * using the function LFG_BufferDestroy().
 */
ui32 LFG_EXPORT_FN LFG_TMG_ImageDestroy( ui32 hImage)
{
   if ( hImage != 0 )
   {
      TMG_image_set_ptr(hImage, TMG_IMAGE_DATA, NULL);
      TMG_image_destroy(hImage);
   }
   return(LFG_OK);
}


/*
 * Setup TMG images to represent the DMA'ed video data.
 * nImageNum is the number of each image in the "round-robin" of 
 * acquired images.  See the example applications for details.
 */
ui32 LFG_EXPORT_FN LFG_TMG_ImageSet(ui32 hCard, struct tLFG *psUsrLFG, ui32 hImage, i32 nImageNum)
{
   static const char *szFnName = "LFG_TMG_ImageSet";
   struct tLFG *psIntLFG;  /* Internal LFG structure */


   /* Get the internal psLFG pointer - this contains the video DMA info */
   LFG_GetPointer(hCard, LFG_PTR_LFG_STRUCTURE, (void**) &psIntLFG); 

   _LFG_MagicCheck(psUsrLFG->dwLibIdMagic, LFG_LIB_ID);
   _LFG_MagicCheck(psIntLFG->dwLibIdMagic, LFG_LIB_ID);

   TMG_image_set_parameter(hImage, TMG_WIDTH,  (ui32) psUsrLFG->sLog.nImageWidth);
   TMG_image_set_parameter(hImage, TMG_HEIGHT, (ui32) psUsrLFG->sLog.nImageHeight);
   TMG_image_set_parameter(hImage, TMG_LINES_THIS_STRIP, psUsrLFG->sLog.nImageHeight);

   switch ( psUsrLFG->sLog.dwPixelFormatActual )
   {
      case LFG_PIXEL_FORMAT_Y8:
         TMG_image_set_parameter(hImage, TMG_PIXEL_FORMAT, TMG_Y8);
         break;

      case LFG_PIXEL_FORMAT_YUV422:
         TMG_image_set_parameter(hImage, TMG_PIXEL_FORMAT, TMG_YUV422);
         break;

      case LFG_PIXEL_FORMAT_RGB15:
      case LFG_PIXEL_FORMAT_RGB15_D:  /* Dithered version */
         TMG_image_set_parameter(hImage, TMG_PIXEL_FORMAT, TMG_RGB15);
         break;

      case LFG_PIXEL_FORMAT_RGB16:
      case LFG_PIXEL_FORMAT_RGB16_D:  /* Dithered version */
         TMG_image_set_parameter(hImage, TMG_PIXEL_FORMAT, TMG_RGB16);
         break;

      case LFG_PIXEL_FORMAT_RGB24:
         TMG_image_set_parameter(hImage, TMG_PIXEL_FORMAT, TMG_BGR24);
         break;

      case LFG_PIXEL_FORMAT_RGB32:
         TMG_image_set_parameter(hImage, TMG_PIXEL_FORMAT, TMG_BGRX32);
         break;

      default:
         _LFG_ErrRet(LFG_ERROR_BAD_PARAM, "", 0);
         break;
   }


  /* Set the pointer to the Nth image to the start of the DMA buffer plus
   * an offset, such that the offset is N x size of image. This means that
   * each image points to the correct data.
   */
   nImageNum -= 1;
   _LFG_Assert(nImageNum >= 0);

   TMG_image_set_ptr(hImage, TMG_IMAGE_DATA,
      (void*)((ui64)(psIntLFG->sCdaBufferVid.pvData) +
          (nImageNum * (psUsrLFG->sLog.nBytesPerLine *
                        psUsrLFG->sLog.nImageHeight)) ) );

   if ( TMG_image_check(hImage) != TMG_OK )
   {
      _LFG_ErrRet(LFG_ERROR_NOT_LFG, "Corrupt TMG image", 0);
   }


  /* Clear down image
   * ----------------
   * This can be useful for test purposes but more importantly, it will
   * make sure the memory exists (i.e. is paged in) prior to starting
   * acquisition.
   */
   {
      i32  nLine, nPixel;
      ui8  *pbData  = (ui8*) TMG_image_get_ptr(hImage, TMG_IMAGE_DATA);
      ui32 *pdwData = (ui32*) TMG_image_get_ptr(hImage, TMG_IMAGE_DATA);

      if ( pbData != NULL )
      {
         if ( psUsrLFG->sLog.dwPixelFormatActual == LFG_PIXEL_FORMAT_YUV422 )
         {
            /*printf("Colour = %08x\n", TMG_CK_get_YUV_values(TMG_BLACK) );*/
            /* 0x80008000 = Black, 0xcc2d662d = Red */

            for (nLine = 0; nLine < psUsrLFG->sLog.nImageHeight; nLine++)
            {
               for (nPixel = 0; nPixel < psUsrLFG->sLog.nImageWidth/2; nPixel++)
               {
                  /* *pdwData++ = (ui8) nPixel;  Ramp type test pattern */
                  *pdwData++ = (ui32) 0x80008000;  /* VYUY -> written little endian = YUYV */
               }
            }
         }
         else
         {
            for (nLine = 0; nLine < psUsrLFG->sLog.nImageHeight; nLine++)
            {
               for (nPixel = 0; nPixel < psUsrLFG->sLog.nBytesPerLine; nPixel++)
               {
                  /* *pbData++ = (ui8) nPixel;  Ramp type test pattern */
                  *pbData++ = (ui8) 0;
               }
            }
         }
      }
   }

   return(LFG_OK);
}

