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
        public ATCamera ip1;
        public smPanTilt pt1, pt2, pt3;
        public ATHydrophone hydro1;
        public DispatcherTimer clock, sclock;
        public ViewModel viewmodel;

        //----ATCORE CONSTRUCTOR----\\
        public ATCore()
        {
            viewmodel = MainWindow.viewmodel;
            clock = new DispatcherTimer(DispatcherPriority.Render);
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

            //son2.connectHead(1);    //-USE THIS TO CONNECT TO THE 2250 HEAD
            //son2.setRange(1, 15);

            cam1 = new ATCamera(2, 0);
            viewmodel.isConnected_c1 = true;   
            viewmodel.ipaddr_c1 = "CAMERA 1";    

            cam2 = new ATCamera(2, 0);
            viewmodel.isConnected_c2 = true;   
            viewmodel.ipaddr_c2 = "CAMERA 2";    

            pt1 = new smPanTilt();
            pt1.AutoScaleMode = AutoScaleMode.Inherit;
            pt1.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            //Initialize McBay's class X 2
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

            hydro1 = new ATHydrophone();

            //ip1 = new ATCamera(3);
            
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
