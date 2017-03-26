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
            public VideoCapture _capture = null;
            //private bool _captureInProgress;    -> I'm not using this yet -TMFT
            public Mat _frame; 
            public ImageBox imgBx;
            public int captureType = 1;

            //----ATCAMERA CONSTRUCTOR----\\
            public ATCamera()
            {
                imgBx = new ImageBox();
                imgBx.FunctionalMode = ImageBox.FunctionalModeOption.Minimum;
                
                CvInvoke.UseOpenCL = true;

                //Default capture type
                try
                {
                    _capture = new VideoCapture();

                    //The following functions were used in testing
                        //_capture.ImageGrabbed += ProcessFrame;
                        //_capture.Start(); 
                }
                catch (NullReferenceException excpt)
                {
                    System.Windows.Forms.MessageBox.Show(excpt.Message);
                }
                _frame = new Mat();
            }
            
            public ATCamera(int captureType)
            {

                imgBx = new ImageBox();
                imgBx.FunctionalMode = ImageBox.FunctionalModeOption.Minimum;

                CvInvoke.UseOpenCL = true;
                if (captureType == 1)
                {
                    //Default capture type
                    try
                    {
                        _capture = new VideoCapture();

                    }
                    catch (NullReferenceException excpt)
                    {
                        System.Windows.Forms.MessageBox.Show(excpt.Message);
                    }
                }
                else if (captureType == 2)
                {
                    throw new NotSupportedException(nameof(captureType));
                    //Camera capture from card
                    /*
                    try
                    {
                        _capture = createLfgCapture(0);
                    }
                    catch (Exception excpt)
                    {
                        System.Windows.Forms.MessageBox.Show(excpt.Message);
                    }
                    */
                }
                else if (captureType == 3)
                {
                    //IP Camera capture
                    try
                    {
                        _capture = new VideoCapture(@"rtsp://admin:12345@192.168.1.64:554");
                    }
                    catch (NullReferenceException excpt)
                    {
                        System.Windows.Forms.MessageBox.Show(excpt.Message);
                    }
                }
                _frame = new Mat();
            }

            public ATCamera(int captureType, int camNo)
            {
                imgBx = new ImageBox();
                imgBx.FunctionalMode = ImageBox.FunctionalModeOption.Minimum;

                CvInvoke.UseOpenCL = false;
                if (captureType == 1)
                {
                    //Default capture type
                    try
                    {
                        _capture = new VideoCapture();

                    }
                    catch (NullReferenceException excpt)
                    {
                        System.Windows.Forms.MessageBox.Show(excpt.Message);
                    }
                }
                else if (captureType == 2)
                {
                    throw new NotSupportedException(nameof(captureType));
                    //Camera capture from card
                    /*
                    try
                    {
                        _capture = createLfgCapture(camNo);
                    }
                    catch (Exception excpt)
                    {
                        System.Windows.Forms.MessageBox.Show(excpt.Message);
                    }
                    */
                }
                else if (captureType == 3)
                {
                    //IP Camera capture
                    try
                    {
                        _capture = new VideoCapture(@"rtsp://admin:12345@192.168.1.64:554");
                    }
                    catch (NullReferenceException excpt)
                    {
                        System.Windows.Forms.MessageBox.Show(excpt.Message);
                    }
                }
                _frame = new Mat();
            }

            //private static Lfg.Capture createLfgCapture(int camNo)
            //{
            //    return new Lfg.Capture(camNo);
            //}
            
            //----ATCAMERA MEMBERS----\\
            public IImage ProcessFrame()
            {
                if (_capture != null && _capture.Ptr != IntPtr.Zero)
                {
                    //_capture.Retrieve(_frame, 0);
                    _frame = _capture.QueryFrame();

                    imgBx.Image = _frame;
                    return _frame;
                }
                return null;
            }

            public void ProcessFrame(object sender, EventArgs arg)
            {
                if(_capture != null)
                {
                    try
                    {
                        _capture.Retrieve(_frame, 0);
                        //_frame = _capture.QuerySmallFrame();
                        imgBx.Image = _frame;
                        //System.Windows.MessageBox.Show(_frame.ToString());
                    }
                    catch (Exception ex)
                    {
                        System.Windows.Forms.MessageBox.Show(ex.ToString());
                    }
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


