using System;
using System.Drawing;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Forms.Integration;
using System.Windows.Forms;
using Emgu.CV;
using Emgu.Util;
using Emgu.CV.UI;
using Emgu.CV.Structure;

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for ipCameraSettings.xaml
    /// </summary>
    public partial class ipCameraSettings : Window
    {
        public ATCore maincore;

        public ipCameraSettings()
        {
            InitializeComponent();
        }

        private void OK_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void WindowLoaded(object sender, RoutedEventArgs e)
        {
            //-----IP Camera EMGUCV ImageBoxes-----\\
            WindowsFormsHost ipHost1 = new WindowsFormsHost();
            WindowsFormsHost ipHost2 = new WindowsFormsHost();
            //ipHost1.Child = maincore.ip1.imgBx;
            maincore.ip1.imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
            //ipHost2.Child = maincore.ip1.imgBx;
            //ipImgBox1 = maincore.ip1.imgBx;
            //maincore.cam1.imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
            //this.ipImgBox2 = maincore.cam2.imgBx;
            //maincore.cam2.imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
            this.ip1.Children.Add(ipHost1);
            this.ip2.Children.Add(ipHost2);
            //Testing
            //maincore.clock.Tick += timer_Tick;
            maincore.ip1._capture.Start();
            maincore.ip1._capture.ImageGrabbed += maincore.ip1.ProcessFrame;
            ipHost1.Child = maincore.ip1.imgBx;
        }

        private void WindowUnloaded(object sender, RoutedEventArgs e)
        {
            ipHost1.Child = null;
            ipHost2.Child = null;
            maincore.ip1._capture.Stop();
        }

        //void timer_Tick(object sender, EventArgs e)
        //{
        //    maincore.ip1.ProcessFrame();
        //    ipHost1.Child = maincore.ip1.imgBx;
        //}
    }
}
