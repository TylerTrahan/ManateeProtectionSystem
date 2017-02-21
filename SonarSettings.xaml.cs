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
using System.ComponentModel;

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for SonarSettings.xaml
    /// </summary>
    public partial class SonarSettings : Window
    {
        public ATCore maincore;
        //----SONAR SETTINGS MEMBERS----\\
        public SonarSettings()
        {
            InitializeComponent();
            this.DataContext = MainWindow.viewmodel;
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            //Check if sonar is connected, if so then disconnect
            if (MainWindow.viewmodel.isConnected_s1.HasValue)
            {
                if ((bool)MainWindow.viewmodel.isConnected_s1)
                {
                    maincore.son1.DisconnectSonar();
                    maincore.viewmodel.isConnected_s1 = false;
                }
            }

            //Check if head checkbox is clicked
            if (MainWindow.viewmodel.alternateHead_s1.HasValue)
            {
                if ((bool)MainWindow.viewmodel.alternateHead_s1)
                {
                    //try to connect to head 1      
                    if (maincore.son1.getHeadCount() > 1)
                    {
                        maincore.son1 = new BVTSDK.ATSonar(MainWindow.viewmodel.ipaddr_s1, 1, 1, 40);
                    }
                    else
                    {
                        //if fail try to connect to head 0
                        maincore.son1 = new BVTSDK.ATSonar(MainWindow.viewmodel.ipaddr_s1, 0, 1, 40);
                    }
                }
            }
            else
            {
                maincore.son1 = new BVTSDK.ATSonar(MainWindow.viewmodel.ipaddr_s1, 0, 1, 40);
                MessageBox.Show(maincore.son1.isConnected.ToString());
                if (maincore.son1.isConnected)
                {
                    maincore.viewmodel.isConnected_s1 = true;
                }
            }
            //Pass sonar object to MainWindow
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button3_Click(object sender, RoutedEventArgs e)
        {

        }        

        private void OnClickOK(object sender, RoutedEventArgs e)
        {
            //ApplicationState.SetValue("sonarIpaddr1", ipaddr1.Text);
            //ApplicationState.SetValue("sonarIpaddr2", ipaddr2.Text);
            //ApplicationState.SetValue("sonarIpaddr3", ipaddr3.Text);
            //ApplicationState.SetValue("sonarIpaddr4", ipaddr4.Text);
            ApplicationState.SetValue("sonarChecked1", checkBox1.IsChecked);
            ApplicationState.SetValue("sonarChecked2", checkBox2.IsChecked);
            ApplicationState.SetValue("sonarChecked3", checkBox3.IsChecked);
            ApplicationState.SetValue("sonarChecked4", checkBox4.IsChecked);
            //ApplicationState.SetValue("sonarHost1", sonarHost1);
            //ApplicationState.SetValue("sonarHost2", sonarHost2);
            //ApplicationState.SetValue("sonarHost3", sonarHost3); 

            this.Close();
        }
        private void OnClickCancel(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

    }


}
