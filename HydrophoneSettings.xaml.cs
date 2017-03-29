using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.ComponentModel;

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for HydrophoneSettings.xaml
    /// </summary>
    public partial class HydrophoneSettings : Window, INotifyPropertyChanged
    {
        public ATCore maincore;
        public bool dataOn = false;
        DispatcherTimer timer = new DispatcherTimer(DispatcherPriority.Render);

        public HydrophoneSettings()
        {
            InitializeComponent();
            maincore = MainWindow.maincore;
        }

        private void OK_Click(object sender, RoutedEventArgs e)
        {
            dataOn = false;
            //maincore.hydro1.acquisitionStop();
            this.Close();
        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            dataOn = false;
            //maincore.hydro1.acquisitionStop();
            this.Close();
        }

        private void btn_StartStop_Click(object sender, RoutedEventArgs e)
        {
            //if (dataOn == false)
            //{
            //    //OnLoaded();
            //    maincore.hydro1.acquisitionStart();

            //    dataOn = true;
            //}
            //else
            //{
            //    maincore.hydro1.acquisitionStop();
            //    dataOn = false;
            //}

            //Read();
        }

        //private void OnLoaded()
        //{
        //    // Create XyDataSeries to host data for our charts
        //    var lineData = new XyDataSeries<double, double>();

        //    for (int i = 0; i < 1000; i++)
        //    {
        //        lineData.Append(i, Math.Sin(i * 0.1));
        //    }
        //    // Assign dataseries to RenderSeries
        //    LineSeries.DataSeries = lineData;

        //    // Start a timer to update our data
        //    double phase = 0.0;

        //    timer.Interval = TimeSpan.FromMilliseconds(10);
        //    timer.Tick += (s, e) =>
        //    {
        //        // SuspendUpdates() ensures the chart is frozen
        //        // while you do updates. This ensures best performance
        //        using (lineData.SuspendUpdates())
        //        {
        //            for (int i = 0; i < 1000; i++)
        //            {
        //                lineData.Update(i, Math.Sin(i * 0.1 + phase));
        //            }
        //        }
        //        phase += 0.01;
        //    };
        //    timer.Start();
        //}

        private void WindowUnloaded(object sender, RoutedEventArgs e)
        {
            dataOn = false;
            //maincore.hydro1.acquisitionStop();
        }

        #region INotifyPropertyChanged implementation

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName = null)
        {
            if (PropertyChanged != null)
                PropertyChanged.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion
    }
}
