// LfgCapture.h

#pragma once

namespace Lfg {
	struct CamApp;

    public ref class Capture : public Emgu::CV::VideoCapture, public System::IDisposable
	{
        typedef Emgu::CV::VideoCapture Base;
		CamApp * cam;
    public:
        // auto selection of available device
        Capture();
        // camNo - set to 0 to automatically select the first available card, or else 1, 2, 3…etc is used to select a specific PCI
        // device number. (Note that often the physical layout of PCI slots does not follow an ascending pattern - for
        // example a four slot PCI motherboard may have the physical slots laid out as 2134.)
		Capture(int camNo);
        // run in test mode
        Capture(System::String^ testImage);
		~Capture();
		!Capture();

        bool IsValid();

        virtual bool Grab() override;
        virtual bool Retrieve(Emgu::CV::IOutputArray^ image, int channel) override;

    private:
		void finalize();
        bool init(int camNo);
        bool init(System::String^ testImage);
		static Emgu::CV::Mat^ hToMat(unsigned long h, bool* destroy);
		static unsigned long bmpToH(System::Drawing::Bitmap^ bmp);
	};
}
