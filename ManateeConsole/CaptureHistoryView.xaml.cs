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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for CaptureHistoryView.xaml
    /// </summary>
    public partial class CaptureHistoryView : UserControl
    {
        public CaptureHistoryView()
        {
            InitializeComponent();
        }

        private void FindButton_OnClick(object sender, RoutedEventArgs e)
        {
            if (!DateTimePicker.Value.HasValue) return;
            CapturesControl.ItemsSource = CaptureStorage.Instance.GetCaptures(DateTimePicker.Value.Value, 500);
        }
    }
}
