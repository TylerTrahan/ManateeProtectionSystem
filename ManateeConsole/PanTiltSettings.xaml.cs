using System;
using System.Collections.Generic;
using System.Linq;
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
using WindowsControlLibrary1;
using System.IO.Ports;
using System.Collections.ObjectModel;
using System.Diagnostics;

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for PanTiltSettings.xaml
    /// </summary>
    public partial class PanTiltSettings : Window
    {
        public ATCore maincore;
        public ObservableCollection<string> portList = new ObservableCollection<string>();

        public PanTiltSettings()
        {
            InitializeComponent();
        }

        private void OnClickOK(object sender, RoutedEventArgs e)
        {
           this.Close();
        }

        private void OnClickCancel(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void WindowLoaded(object sender, RoutedEventArgs e)
        {
            ExtendedWindowsFormsHost ptHost1 = new ExtendedWindowsFormsHost();
            ExtendedWindowsFormsHost ptHost2 = new ExtendedWindowsFormsHost();

            ptHost1.Child = maincore.pt1;
            ptHost2.Child = maincore.pt2;

            this.ptdock1.Children.Add(ptHost1);
            this.ptdock2.Children.Add(ptHost2);

            //Get a list of serial ports
            getPorts();

            //Bind portList to comboBoxes
            this.comboBox1SerialPort1.ItemsSource = portList;
            this.comboBox1SerialPort2.ItemsSource = portList;

            //Check if ports are open and initialize
            this.checkBoxAutoConnect1.IsChecked = maincore.port1open;
            this.checkBoxAutoConnect2.IsChecked = maincore.port2open;

            //Update Pan and Tilt Angles
            ////textBoxPanPosition1.Text = maincore.pt1.pAngle.ToString();
            ////textBoxTiltPosition1.Text = maincore.pt1.tAngle.ToString();

            ////textBoxPanPosition2.Text = maincore.pt2.pAngle.ToString();
            ////textBoxTiltPosition2.Text = maincore.pt2.tAngle.ToString();
        }

        private void getPorts()
        {
            string[] ports = SerialPort.GetPortNames();

            foreach (string port in ports)
            {
                portList.Add(port);
            }
        }

        private bool checkIfPortIsOpen(smPanTilt pt)
        {
            if(pt.cPort!="COM1")
            {
                //Does it exist?
                if(SerialPort.GetPortNames().Any(x=> x == pt.cPort.ToString()))
                {
                    return true;
                }
            }
            return false;
        }

        private void btn1Connect_Click(object sender, RoutedEventArgs e)
        {
                try
                {
                    maincore.pt1.cPort = this.comboBox1SerialPort1.Text;
                    maincore.port1open= maincore.pt1.cPortOpen();
                    this.checkBoxAutoConnect1.IsChecked = maincore.port1open;
                }
                catch (Exception ex)
                {
                    Trace.WriteLine("Error opening port!", ex.Message);
                }
        }

        private void btn2Connect_Click(object sender, RoutedEventArgs e)
        {
                try
                {
                    maincore.pt2.cPort = this.comboBox1SerialPort2.Text;
                    maincore.port2open= maincore.pt2.cPortOpen();
                    this.checkBoxAutoConnect2.IsChecked = maincore.port2open;
                }
                catch (Exception ex)
                {
                    Trace.WriteLine("Error opening port: {0}", ex.Message);
                }
        }

        private void btn2Disconnect_Click(object sender, RoutedEventArgs e)
        {
                try
                {
                    maincore.pt2.cPortClose();
                    this.checkBoxAutoConnect2.IsChecked = false;
                    maincore.port2open= false;
                }
                catch (Exception ex)
                {
                    Trace.WriteLine("Error closing port: {0}", ex.Message);
                }
        }

        private void btn1Disconnect_Click(object sender, RoutedEventArgs e)
        {
                try
                {
                    maincore.pt1.cPortClose();
                    this.checkBoxAutoConnect1.IsChecked = false;
                    maincore.port1open= false;
                }
                catch (Exception ex)
                {
                    Trace.WriteLine("Error closing port: {0}", ex.Message);
                }

        }

        //private void UpdatePTAngles(object sender, MouseButtonEventArgs e)
        //{
        //    textBoxPanPosition1.Text = maincore.pt1.pAngle.ToString();
        //    Trace.WriteLine("pAngle: " + maincore.pt1.pAngle.ToString());
        //    textBoxTiltPosition1.Text = maincore.pt1.tAngle.ToString();

        //    textBoxPanPosition2.Text = maincore.pt2.pAngle.ToString();
        //    textBoxTiltPosition2.Text = maincore.pt2.tAngle.ToString();
        //}
    }
}
