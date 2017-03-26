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

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for PanTiltSettings.xaml
    /// </summary>
    public partial class PanTiltSettings : Window
    {
        public ATCore maincore;
        public ObservableCollection<string> portList = new ObservableCollection<string>();
        public bool port1Open = false;
        public bool port2Open = false;

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
            //ExtendedWindowsFormsHost ptHost3 = new ExtendedWindowsFormsHost();
            ptHost1.Child = maincore.pt1;
            ptHost2.Child = maincore.pt2;
            //ptHost3.Child = maincore.pt3;
            this.ptdock1.Children.Add(ptHost1);
            this.ptdock2.Children.Add(ptHost2);
            //this.ptdock3.Children.Add(ptHost3);
            getPorts();
            //Bind portList to comboBoxes
            this.comboBox1SerialPort1.ItemsSource = portList;
            maincore.pt1.PerformAutoScale();
            this.comboBox1SerialPort2.ItemsSource = portList;

            this.checkBoxAutoConnect1.IsChecked = port1Open;
            this.checkBoxAutoConnect2.IsChecked = port2Open; 
        }

        private void getPorts()
        {
            string[] ports = SerialPort.GetPortNames();

            foreach (string port in ports)
            {
                portList.Add(port);
            }

        }

        private void btn1Connect_Click(object sender, RoutedEventArgs e)
        {
            maincore.pt1.cPort = this.comboBox1SerialPort1.Text;
            port1Open = maincore.pt1.cPortOpen();
            this.checkBoxAutoConnect1.IsChecked = port1Open;
        }

        private void btn2Connect_Click(object sender, RoutedEventArgs e)
        {
            maincore.pt2.cPort = this.comboBox1SerialPort2.Text;
            port2Open = maincore.pt2.cPortOpen();
            this.checkBoxAutoConnect2.IsChecked = port1Open;
        }
    }
}
