//------------------------------------------------------------------------------------------
//  Manatee Protection System Camera Object
//  Copyright Tyler Trahan - Aqua-tech Services/South Florida Water Management District 2017
//------------------------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows; 
using System.Windows.Controls; 
using System.Windows.Forms;
using System.Windows.Threading; 
using Emgu.CV;
using Emgu.CV.UI;
using Emgu.CV.CvEnum;
using Emgu.CV.Structure;
using Emgu.Util;
using System.Runtime.InteropServices; 

namespace ManateeConsole
{
    public class ATCamera
    {
            //----ATCAMERA FIELDS----\\
            private VideoCapture _capture = null;
            //private bool _captureInProgress;    -> I'm not using this yet -TMFT
            public Mat _frame; 
            public ImageBox imgBx;

            //----ATCAMERA CONSTRUCTOR----\\
            public ATCamera()
            {
                imgBx = new ImageBox();
                imgBx.FunctionalMode = ImageBox.FunctionalModeOption.Minimum;
                
                CvInvoke.UseOpenCL = false;
                try
                {
                    _capture = new VideoCapture();
                    //_capture.ImageGrabbed += ProcessFrame;
                    //_capture.Start(); 
                }
                catch (NullReferenceException excpt)
                {
                    System.Windows.Forms.MessageBox.Show(excpt.Message);
                }
                _frame = new Mat();
 
            }
            
            //----ATCAMERA MEMBERS----\\
            public void ProcessFrame()
            {
                if (_capture != null && _capture.Ptr != IntPtr.Zero)
                {
                    _capture.Retrieve(_frame, 0);

                    imgBx.Image = _frame;
                }
            }

            private void ReleaseData()
            {
                if (_capture != null)
                {
                    _capture.Dispose();
                    _frame.Dispose();
                }
            }
        
    }
}


