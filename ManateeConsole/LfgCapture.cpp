// This is the main DLL file.


#include "stdafx.h"
#include "LfgCapture.h"

#include <lfg_api.h>
#include <tmg_api.h>


#undef GetTempPath

namespace Lfg {

struct CamApp
{
    tLFG gsLFG;
    ui32 hCard;
    ui32 hSrcImage;
    ui32 hBuffer;
    bool testMode;
    bool captured;;
	bool error;
    CamApp()
    {
        memset(this, 0, sizeof(CamApp));
    }
};

static void newImageEventHandler(ui32 hCard, ui32 dwEvent, ui32 dwIntStatus, void* pv)
{
    struct CamApp * psApp = (struct CamApp*)pv;
    if ( dwEvent & LFG_EVENT_NEW_IMAGE )
    {
		if (!psApp->captured)
		{
			psApp->error = TMG_image_copy(psApp->hBuffer, psApp->hSrcImage) != TMG_OK;
		}
        /* Make sure we know which image to process from our continuous
        * (circular) live acquisition sequence of 2 images.
        */
        if ( dwEvent & LFG_EVENT_END_SEQUENCE )
        {
            /* Process image 2 here */
        }
        else
        {
            /* Process image 1 here */
        }
    }
    psApp->captured = true;
}

Capture::Capture()
    : Base(), cam(nullptr)
{
    init(0);
}
Capture::Capture(int camNo)
    : Base(), cam(nullptr)
{
    init(camNo);
}
Capture::Capture(System::String^ testImage)
    : Base(), cam(nullptr)
{
    init(testImage);
}
Capture::~Capture()
{
	finalize();
}
Capture::!Capture()
{
	finalize();
}
void Capture::finalize()
{
	if (!cam) return;
    try
    {
        if (!cam->testMode)
        {
			LFG_AcquisitionStop(cam->hCard);
            TMG_image_set_ptr(cam->hSrcImage, TMG_IMAGE_DATA, NULL);
			TMG_image_set_ptr(cam->hBuffer, TMG_IMAGE_DATA, NULL);
			TMG_image_destroy(cam->hBuffer);
        }
        TMG_image_destroy(cam->hSrcImage);
        cam->hSrcImage = 0;
    }
    catch (...)
    {
    }
	delete cam;
    cam = 0;
}
bool Capture::init(int camNo)
{
    finalize();
    try
    {
        cam = new CamApp;
        cam->testMode = false;

		//cam->gsLFG.dwDeviceAddr = 0;

        if (LFG_Open(&cam->hCard, &cam->gsLFG, LFG_ErrHandlerDefault) != LFG_OK) throw gcnew System::Exception("Card is not available");

		switch (camNo)
		{
		case 0: cam->gsLFG.sLog.dwVideoSrc = LFG_VID_SRC_CM0;
			break;
		case 1: cam->gsLFG.sLog.dwVideoSrc = LFG_VID_SRC_CM1;
			break;
		case 2: cam->gsLFG.sLog.dwVideoSrc = LFG_VID_SRC_CM2;
			break;
		case 3: cam->gsLFG.sLog.dwVideoSrc = LFG_VID_SRC_CM3;
			break;
		default:
			cam->gsLFG.sLog.dwVideoSrc = LFG_VID_SRC_CM0;
			break;
		}

        //cam->gsLFG.sLog.dwVideoMode = LFG_50_PAL_384x288_F1;
		cam->gsLFG.sLog.dwVideoMode = LFG_60_NTSC_640x240_F1;

        LFG_SetAndGet(cam->hCard, &cam->gsLFG, LFG_WRITE);

        LFG_TMG_ImageCreate(&cam->hSrcImage);
		LFG_TMG_ImageCreate(&cam->hBuffer);
		if (LFG_TMG_ImageSet(cam->hCard, &cam->gsLFG, cam->hBuffer, 1) != LFG_OK) throw gcnew System::Exception("Can not create buffer");

        cam->gsLFG.sLog.dwEvents = LFG_EVENT_NEW_IMAGE;
        cam->gsLFG.sLog.fPolledDrivenCallback = TRUE; /* For clarity here */

        LFG_SetAndGet(cam->hCard, &cam->gsLFG, LFG_WRITE);

        LFG_EventHandlerInstall(cam->hCard, &newImageEventHandler, cam);

		LFG_AcquisitionStart(cam->hCard);
    }
    catch (...)
    {
        finalize();
        return false;
    }
    return true;
}
bool Capture::init(System::String^ testImage)
{
    finalize();
    try
    {
        System::Drawing::Bitmap^ bmp = gcnew System::Drawing::Bitmap(testImage);

        cam = new CamApp;
        cam->testMode = true;
        cam->hCard = 0;
        cam->hSrcImage = bmpToH(bmp);
    }
    catch (...)
    {
        finalize();
        return false;
    }
    return true;
}

bool Capture::IsValid()
{
    return cam != nullptr;
}

Emgu::CV::Mat^ Capture::hToMat(unsigned long h, bool* destroy)
{
    ui32 x = TMG_image_get_parameter(h, TMG_WIDTH);
	if (x == 0) return nullptr;
    x = TMG_image_get_parameter(h, TMG_HEIGHT);
    if (TMG_image_check(h) != TMG_OK) return nullptr;

	if (!TMG_image_get_ptr(h, TMG_IMAGE_DATA)) return nullptr;

	void *data = nullptr;

	bool makenull = false;
	ui32 curFormat = TMG_image_get_parameter(h, TMG_PIXEL_FORMAT);

	if (curFormat != TMG_BGR24)
	{
		Thandle dst = TMG_image_create();
		if (TMG_image_convert(h, dst, TMG_BGR24, 0, TMG_RUN) != TMG_OK) return nullptr;
		h = dst;
		if (destroy) *destroy = true;
		makenull = true;
	}
	else
	{
		//int size = TMG_image_get_parameter(h, TMG_HEIGHT) * TMG_image_get_parameter(h, TMG_BYTES_PER_LINE);
		//data = malloc(size);
		//memcpy(data, TMG_image_get_ptr(h, TMG_IMAGE_DATA), size);
		if (destroy) *destroy = false;
	}

	data = TMG_image_get_ptr(h, TMG_IMAGE_DATA);
	if (!data) return nullptr;

	Emgu::CV::Mat^ mat = gcnew Emgu::CV::Mat(
		TMG_image_get_parameter(h, TMG_HEIGHT),
		TMG_image_get_parameter(h, TMG_WIDTH),
		Emgu::CV::CvEnum::DepthType::Cv8U,
		3,
		System::IntPtr(data),
		TMG_image_get_parameter(h, TMG_BYTES_PER_LINE));

	data = mat->DataPointer.ToPointer();
	//mat->Save("C:\\qq.png");

	if (makenull)
	{
		TMG_image_set_ptr(h, TMG_IMAGE_DATA, nullptr);
		TMG_image_destroy(h);
	}
	return mat;
}

unsigned long Capture::bmpToH(System::Drawing::Bitmap^ bmp)
{
	ui32 bpc = TMG_BGR24;
	if (bmp->PixelFormat != System::Drawing::Imaging::PixelFormat::Format24bppRgb)
    {
        bmp = bmp->Clone(System::Drawing::Rectangle(0, 0, bmp->Width, bmp->Height), System::Drawing::Imaging::PixelFormat::Format24bppRgb);
	}
	Thandle h = TMG_image_create();
	TMG_image_set_parameter(h, TMG_WIDTH, bmp->Width);
	TMG_image_set_parameter(h, TMG_HEIGHT, bmp->Height);
	TMG_image_set_parameter(h, TMG_PIXEL_FORMAT, bpc);
	TMG_image_set_parameter(h, TMG_LINES_THIS_STRIP, bmp->Height);

	if (TMG_image_check(h) != TMG_OK) return 0;
	if (TMG_image_malloc_a_strip(h) != TMG_OK) return 0;

	ui8* data = (ui8*)TMG_image_get_ptr(h, TMG_IMAGE_DATA);

	System::Drawing::Rectangle rect = System::Drawing::Rectangle(0, 0, bmp->Width, bmp->Height);
	System::Drawing::Imaging::BitmapData^ bmpData = bmp->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, bmp->PixelFormat);
	int bytes = System::Math::Abs(bmpData->Stride) * bmp->Height;

	memcpy(data, bmpData->Scan0.ToPointer(), bytes);

	bmp->UnlockBits(bmpData);

	return h;
}

bool Capture::Grab()
{
	if (!cam || cam->error) return false;

    if (!cam->testMode)
    {
		cam->captured = false;
        for (int i = 0; !cam->captured && i < 100; ++i) /* Loop until finished or 100 attempts ~ 2 sec */
        {
            /* The next line drives the event driven callback in polled mode */
            LFG_SetAndGet(cam->hCard, &cam->gsLFG, LFG_READ);
        }
    }

    return cam->hSrcImage != 0;
}

bool Capture::Retrieve(Emgu::CV::IOutputArray^ image, int channel)
{
    if (!cam || !cam->hSrcImage || cam->error) return false;
    Emgu::CV::Mat^ mat = nullptr;
	try
	{
		bool destroy = false;
		mat = hToMat(cam->hSrcImage, &destroy);
		if (!cam->testMode && cam->captured)
		{
			//LFG_AcquisitionStop(cam->hCard);
			//cam->captured = false;
		}
		if (mat)
		{
			mat->CopyTo(image, nullptr);
			void * data = mat->DataPointer.ToPointer();
			delete mat;
			if (destroy && data)
			{
				TMG_DebugFree(data);
			}
		}
	}
	catch (...)
	{
		mat = nullptr;
	}
    return mat != nullptr;
}

}