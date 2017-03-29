using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for SystemDiagnostics.xaml
    /// </summary>
    public partial class SystemDiagnostics : Window
    {
        public SystemDiagnostics()
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

        private void OnClickOpenFileDialog(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            if (folderBrowserDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                textBoxFilePath.Text = folderBrowserDialog.SelectedPath;
        }

        private void OnClickYES(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void OnClickNO(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

    }
}
