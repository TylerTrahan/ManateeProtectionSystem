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
           // ApplicationState.SetValue("serialPort1", comboBox1SerialPort1.Text);
           // ApplicationState.SetValue("serialPort2", comboBox1SerialPort2.Text);
           // //ApplicationState.SetValue("serialPort3", comboBox1SerialPort3.Text);
           // //ApplicationState.SetValue("serialPort4", comboBox1SerialPort4.Text);
           // ApplicationState.SetValue("partnerSonar1", comboBoxPartnerSonar1.Text);
           // ApplicationState.SetValue("partnerSonar2", comboBoxPartnerSonar2.Text);
           // //ApplicationState.SetValue("partnerSonar3", comboBoxPartnerSonar3.Text);
           // //ApplicationState.SetValue("partnerSonar4", comboBoxPartnerSonar4.Text);
           // ApplicationState.SetValue("autoConnect1", checkBoxAutoConnect1.IsChecked);
           // ApplicationState.SetValue("autoConnect2", checkBoxAutoConnect2.IsChecked);
           // //ApplicationState.SetValue("autoConnect3", checkBoxAutoConnect3.IsChecked);
           // //ApplicationState.SetValue("autoConnect4", checkBoxAutoConnect4.IsChecked);
           // ApplicationState.SetValue("panPosition1", textBoxPanPosition1.Text);
           // ApplicationState.SetValue("panPosition2", textBoxPanPosition2.Text);
           // //ApplicationState.SetValue("panPosition3", textBoxPanPosition3.Text);
           // //ApplicationState.SetValue("panPosition4", textBoxPanPosition4.Text);
           // ApplicationState.SetValue("tiltPosition1", textBoxTiltPosition1.Text);
           // ApplicationState.SetValue("tiltPosition2", textBoxTiltPosition2.Text);
           //// ApplicationState.SetValue("tiltPosition3", textBoxTiltPosition3.Text);
           // //ApplicationState.SetValue("tiltPosition4", textBoxTiltPosition4.Text);
           // ApplicationState.SetValue("offset1", checkBoxOffset1.IsChecked);
           // ApplicationState.SetValue("offset2", checkBoxOffset2.IsChecked);
           // //ApplicationState.SetValue("offset3", checkBoxOffset3.IsChecked);
           // //ApplicationState.SetValue("offset4", checkBoxOffset4.IsChecked);
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
            maincore.pt1.cPort = "COM6";
            port1Open = maincore.pt1.cPortOpen();
            this.checkBoxAutoConnect1.IsChecked = port1Open;
        }

        private void btn2Connect_Click(object sender, RoutedEventArgs e)
        {
            maincore.pt2.cPort = "COM7";
            port2Open = maincore.pt2.cPortOpen();
            this.checkBoxAutoConnect2.IsChecked = port1Open;
        }
    }
}
