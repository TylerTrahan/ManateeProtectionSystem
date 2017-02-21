//-------------------------------------------------
//  Manatee Protection System Core System
//  Copyright Tyler Trahan - Aqua-tech Services 2017
//-------------------------------------------------

using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Windows.Forms;
using System.Windows.Threading;
using System.Collections.Generic;
using System.Text;
using System.Threading; 
using System.Runtime.InteropServices;
using System.IO;
using System.Drawing.Imaging;
using System.Windows.Forms.Integration; 
using WindowsControlLibrary1;
using System.ComponentModel; 

//Include BlueView SDK Libraries
using BVTSDK;

//Include OpenCV Libraries
using Emgu.CV;
using Emgu.CV.UI;
using Emgu.Util;
using Emgu.CV.CvEnum;
using Emgu.CV.Structure;

namespace ManateeConsole
{
    public class ATCore
    {
        //-----ATCORE FIELDS-----\\
        public ATSonar son1, son2;
        public ATCamera cam1, cam2;
        public smPanTilt pt1, pt2, pt3;
        public ATHydrophone hydro1;
        public DispatcherTimer clock, sclock;
        public ViewModel viewmodel;

        //----ATCORE CONSTRUCTOR----\\
        public ATCore()
        {
            viewmodel = MainWindow.viewmodel;
            clock = new DispatcherTimer();
            clock.Interval = new TimeSpan(0, 0, 0, 0, 50);

            sclock = new DispatcherTimer();
            sclock.Interval = new TimeSpan(0, 0, 0, 0, 50);

            son1 = new ATSonar(viewmodel.ipaddr_s1, 0, 1, 50);
            if (son1.isConnected)
            {
                viewmodel.isConnected_s1 = true;
            }
            son2 = new ATSonar(viewmodel.ipaddr_s2, 0, 1, 50);
            if (son2.isConnected)
            {
                viewmodel.isConnected_s2 = true;
            }

            //son2.connectHead(1);

            //son3 = new ATSonar(viewmodel.ipaddr_s3, 1, 100);
            //if (son3.isConnected)
            //{
            //    viewmodel.isConnected_s3 = true;
            //}

            cam1 = new ATCamera();
            cam2 = new ATCamera();
            //cam3 = new ATCamera();

            pt1 = new smPanTilt();
            pt1.AutoScaleMode = AutoScaleMode.Inherit;
            pt1.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            //Initialize McBay's class X 3
            pt1.cBaudRate = 9600;
            pt1.cDataBits = 8;
            pt1.cParity = 0;
            pt1.cStopBits = 1;
            pt2 = new smPanTilt();
            pt2.AutoScaleMode = AutoScaleMode.Inherit;
            pt2.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            pt2.cBaudRate = 9600;
            pt2.cDataBits = 8;
            pt2.cStopBits = 1;
            //pt3 = new smPanTilt();
            //pt3.cBaudRate = 9600;
            //pt3.cDataBits = 8;
            //pt3.cParity = 0;
            //pt3.cStopBits = 1;

            //hydro1 = new ATHydrophone();
        }

        //-----ATCORE MEMBERS-----\\
        // This all needs to happen in this function
        //      loadSettingsFromConfigFile()
        //      runDiagnostics()
        //      runNetworkSettings()
        //
        //      findAllSonars()
        //      connectSonars()
        //      pairSonars()
        //
        //      findCameras()
        //      connectCameras()
        //      findPantilts()
        //      connectPantilts()

    }
}
