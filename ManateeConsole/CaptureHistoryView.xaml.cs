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
            Loaded += CaptureHistoryView_Loaded;
        }

        void CaptureHistoryView_Loaded(object sender, RoutedEventArgs e)
        {
            var now = DateTime.Now;
            DatePicker.SelectedDate = now.Date;
            HourTextBox.NumValue = now.Hour;
            MinTextBox.NumValue = now.Minute;
            SecTextBox.NumValue = now.Second;
        }

        private void FindButton_OnClick(object sender, RoutedEventArgs e)
        {
            if (!DatePicker.SelectedDate.HasValue) return;
            var dateTime = DatePicker.SelectedDate.Value.Add(new TimeSpan(HourTextBox.NumValue, MinTextBox.NumValue, SecTextBox.NumValue));
            CapturesControl.ItemsSource = CaptureStorage.Instance.GetCaptures(dateTime, 1000).OrderBy(x => x.Source);
        }
    }
}
