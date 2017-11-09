using System;
using System.Drawing; 
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms.Integration;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ArduinoDriver.SerialProtocol;
using ArduinoUploader.Hardware;
using WindowsControlLibrary1;
using BVTSDK;
using Emgu.CV;
using Emgu.Util;
using Emgu.CV.UI;
using Emgu.CV.Structure;
using MahApps.Metro.Controls;

namespace ManateeConsole
{
        /// <summary>
        /// Interaction logic for MainWindow.xaml for the Manatee Protection System main console
        /// </summary>
    public partial class MainWindow
    {
        private static readonly Lazy<ViewModel> lazyviewmodel = new Lazy<ViewModel>();
        public static ViewModel viewmodel { get { return lazyviewmodel.Value; } }
        public static ATCore maincore;
        public ArduinoDriver.ArduinoDriver driver = null;
        private System.Windows.Controls.Button backButton;
        private bool maximized = false;
        private const ArduinoModel AttachedArduino = ArduinoModel.UnoR3;
        //private System.Threading.Timer timer;
        public PinModeResponse pResponse;
        public BackgroundWorker visionWorker;
        public ImageBox test1, test2;

        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = viewmodel;

            //Functionality for the Back button when media window is maximized
            backButton = new System.Windows.Controls.Button();
            backButton.Content = "Back";
            backButton.Width = 100;
            backButton.Height = 50;
            backButton.Margin = new Thickness(5,5,5,5);
            backButton.VerticalAlignment = VerticalAlignment.Top;
            backButton.HorizontalAlignment = System.Windows.HorizontalAlignment.Right;
        }

        #region SONAR MOUSE DOWNS
        private void OnMouseDownSonar1(object sender, MouseButtonEventArgs e)
        {
            if (maximized) return;
            maximized = true;
            MainGrid.RowDefinitions.Clear();

            MiddleGrid.Visibility = Visibility.Hidden;
            BottomGrid.Visibility = Visibility.Hidden;

            TopGrid.ColumnDefinitions.Clear();

            sonar2.Visibility = Visibility.Hidden;
            sonar1.Margin = new Thickness(5, 5, 100, 5);
            //sonar3.Visibility = Visibility.Hidden;
            SonarControls.Visibility = Visibility.Hidden;

            backButton.Click -= new RoutedEventHandler(MinimizeTopGrid);
            backButton.Click += new RoutedEventHandler(MinimizeTopGrid);

            TopGrid.Children.Add(backButton);
        }

        private void OnMouseDownSonar2(object sender, MouseButtonEventArgs e)
        {
            if (maximized) return;
            maximized = true;
            MainGrid.RowDefinitions.Clear();

            MiddleGrid.Visibility = Visibility.Hidden;
            BottomGrid.Visibility = Visibility.Hidden;

            TopGrid.ColumnDefinitions.Clear();

            sonar1.Visibility = Visibility.Hidden;
            sonar2.Margin = new Thickness(5, 5, 100, 5);
            //sonar3.Visibility = Visibility.Hidden;
            SonarControls.Visibility = Visibility.Hidden;

            backButton.Click -= new RoutedEventHandler(MinimizeTopGrid);
            backButton.Click += new RoutedEventHandler(MinimizeTopGrid);
            TopGrid.Children.Add(backButton);
        }

        //private void OnMouseDownSonar3(object sender, MouseButtonEventArgs e)
        //{
        //    MainGrid.RowDefinitions.Clear();

        //    MiddleGrid.Visibility = Visibility.Hidden;
        //    BottomGrid.Visibility = Visibility.Hidden;

        //    TopGrid.ColumnDefinitions.Clear();

        //    sonar1.Visibility = Visibility.Hidden;
        //    sonar2.Visibility = Visibility.Hidden;

        //    backButton.Click += new RoutedEventHandler(MinimizeTopGrid);
        //    TopGrid.Children.Add(backButton);
        //}

        //Video Mouse Downs
        #endregion
        #region CAMERA MOUSE CLICKS
        private void OnMouseDownVideo1(object sender, MouseButtonEventArgs e)
        {
            if (maximized) return;
            maximized = true;
            MainGrid.RowDefinitions.Clear();

            TopGrid.Visibility = Visibility.Hidden;
            BottomGrid.Visibility = Visibility.Hidden;

            MiddleGrid.ColumnDefinitions.Clear();

            video1.Margin = new Thickness(5,5,100,5);

            video2.Visibility = Visibility.Hidden;
            videoHost2.Visibility = Visibility.Hidden;
            //video3.Visibility = Visibility.Hidden;
            //videoHost3.Visibility = Visibility.Hidden;
            SonarControls.Visibility = Visibility.Hidden;

            backButton.Click -= new RoutedEventHandler(MinimizeMiddleGrid);
            backButton.Click += new RoutedEventHandler(MinimizeMiddleGrid);

            if (MiddleGrid.Children.Count != 0)
            {
                MiddleGrid.Children.Add(backButton);
            }
            else
            {
                //How to dynamically add button to either ExtendedWinFormsHost control or ImageBox
                MiddleGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(4, GridUnitType.Star) });
                MiddleGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

                this.MiddleGrid.Children.Add(video1);
                this.video1.Children.Add(videoHost1);
                Grid.SetColumn(video1, 0);
                Grid.SetColumn(videoHost1, 0);
                this.MiddleGrid.Children.Add(backButton);
                Grid.SetColumn(backButton, 1);
            } 
        }

        private void OnMouseDownVideo2(object sender, MouseButtonEventArgs e)
        {
            if (maximized) return;
            maximized = true;
            MainGrid.RowDefinitions.Clear();

            TopGrid.Visibility = Visibility.Hidden;
            BottomGrid.Visibility = Visibility.Hidden;

            MiddleGrid.ColumnDefinitions.Clear();

            video2.Margin = new Thickness(5, 5, 100, 5);
            video1.Visibility = Visibility.Hidden;
            //video3.Visibility = Visibility.Hidden;
            SonarControls.Visibility = Visibility.Hidden;

            backButton.Click -= new RoutedEventHandler(MinimizeMiddleGrid);
            backButton.Click += new RoutedEventHandler(MinimizeMiddleGrid);
            MiddleGrid.Children.Add(backButton);
        }

        //private void OnMouseDownVideo3(object sender, MouseButtonEventArgs e)
        //{
        //    MainGrid.RowDefinitions.Clear();

        //    TopGrid.Visibility = Visibility.Hidden;
        //    BottomGrid.Visibility = Visibility.Hidden;

        //    MiddleGrid.ColumnDefinitions.Clear();

        //    video1.Visibility = Visibility.Hidden;
        //    video2.Visibility = Visibility.Hidden;

        //    backButton.Click += new RoutedEventHandler(MinimizeMiddleGrid);
        //    MiddleGrid.Children.Add(backButton);
        //}

        //These functions rearranges the grid after an element is maximized
        #endregion
        private void MinimizeTopGrid(object sender, RoutedEventArgs e)
        {
            if (!maximized) return;
            maximized = false;

            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(3, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(0.5, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(2.5, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(3, GridUnitType.Star) });
            TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(3, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(3, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            Grid.SetRow(TopGrid, 0);
            Grid.SetRow(SonarControls, 1);
            Grid.SetRow(MiddleGrid, 2);
            Grid.SetRow(BottomGrid, 3);

            Grid.SetColumn(sonar1, 1);
            Grid.SetColumn(sonar2, 2);
            //Grid.SetColumn(sonar3, 3);

            sonar1.Visibility = Visibility.Visible;
            sonar1.Margin = new Thickness(5);
            sonar2.Visibility = Visibility.Visible;
            sonar2.Margin = new Thickness(5);
            //sonar3.Visibility = Visibility.Visible;

            TopGrid.Visibility = Visibility.Visible;
            SonarControls.Visibility = Visibility.Visible;
            MiddleGrid.Visibility = Visibility.Visible;
            BottomGrid.Visibility = Visibility.Visible;

            backButton.Click -= new RoutedEventHandler(MinimizeTopGrid);
            TopGrid.Children.Remove(backButton);
        }

        private void MinimizeMiddleGrid(object sender, RoutedEventArgs e)
        {
            if (!maximized) return;
            maximized = false;

            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(3, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(0.5, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(2.5, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            MiddleGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            MiddleGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //MiddleGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            Grid.SetRow(TopGrid, 0);
            Grid.SetRow(SonarControls, 1);
            Grid.SetRow(MiddleGrid, 2);
            Grid.SetRow(BottomGrid, 3);

            Grid.SetColumn(video1, 0);
            Grid.SetColumn(video2, 1);
            //Grid.SetColumn(video3, 2);

            video1.Visibility = Visibility.Visible;
            video1.Margin = new Thickness(5);
            video2.Visibility = Visibility.Visible;
            video2.Margin = new Thickness(5);
            //video3.Visibility = Visibility.Visible;

            TopGrid.Visibility = Visibility.Visible;
            SonarControls.Visibility = Visibility.Visible;
            MiddleGrid.Visibility = Visibility.Visible;
            BottomGrid.Visibility = Visibility.Visible;

            backButton.Click -= new RoutedEventHandler(MinimizeMiddleGrid);
            MiddleGrid.Children.Remove(backButton);
        }

        #region BOTTOM MENU CLICKS
        private void OnClickSonarSettings(object sender, RoutedEventArgs e)
        {
            SonarSettings sonar;
            sonar = new SonarSettings();
            sonar.DataContext = viewmodel;
            sonar.maincore = maincore;
            sonar.Show();
            sonar.Activate();
        }

        private void OnClickCameraSettings(object sender, RoutedEventArgs e)
        {
            CameraSettings camera;
            camera = new CameraSettings();
            camera.DataContext = viewmodel;
            camera.maincore = maincore;
            camera.Show();
            camera.Activate();
        }

        private void OnClickPanTiltSettings(object sender, RoutedEventArgs e)
        {
            PanTiltSettings pantilt;
            pantilt = new PanTiltSettings();
            pantilt.DataContext = viewmodel;
            pantilt.maincore = maincore;
            pantilt.Show();
            pantilt.Activate();
            
        }

        private void OnClickSystemDiagnostics(object sender, RoutedEventArgs e)
        {
            SystemDiagnostics system;
            system = new SystemDiagnostics();
            system.DataContext = viewmodel;
            system.Show();
            system.Activate();
        }

        private void OnClickHydrophoneSettings(object sender, RoutedEventArgs e)
        {
            HydrophoneSettings hw;
            hw = new HydrophoneSettings();
            hw.DataContext = viewmodel;
            hw.maincore = maincore;
            hw.Show();
            hw.Activate();
        }

        private void OnClickIPCameraSettings(object sender, RoutedEventArgs e)
        {
            ipCameraSettings ip;
            ip = new ipCameraSettings();
            ip.DataContext = viewmodel;
            ip.maincore = maincore;
            ip.Show();
            ip.Activate();
        }

        private void OnClickPLCConnection(object sender, RoutedEventArgs e)
        {
            PLCSettings plc;
            plc = new PLCSettings();
            if (plc.driver == null) return;
            plc.DataContext = viewmodel;
            plc.Show();
            plc.Activate();
        }

        private void btn_FFT_Click(object sender, RoutedEventArgs e)
        {
            FFTWindow fft = new FFTWindow();
            fft.DataContext = viewmodel;
            fft.Show();
            fft.Activate();
        }

        private void Btn_CaptureHistory_OnClick(object sender, RoutedEventArgs e)
        {
            new MetroWindow()
            {
                Width = 900,
                Height = 750,
                WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen,
                Title = "CAPTURE HISTORY",
                Content = new CaptureHistoryView()
            }.ShowDialog();
        }
        #endregion

        private void bindProperty<Cont, Handler>(Cont control, Action<Cont, EventHandler> subscriber, Action<Cont> getter)
        {
            getter(control);
            subscriber(control, new EventHandler(delegate (object o, EventArgs a) { getter(control); }));
        }
        //This function is called after MainWindow loads - PUT INITIALIZATION FUNCTIONS HERE
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {

           //Temporary turned back on
            maincore = new ATCore();
            maincore.clock.Tick += new EventHandler(timer_Tick);
            maincore.clock.Start();

            maincore.sclock.Tick += new EventHandler(timer2_Tick);
            maincore.sclock.Start();

            //logclock = new DispatcherTimer();
            //logclock.Interval = new TimeSpan(0, 0, 0, 1);
            //logclock.Tick += new EventHandler(timer3_Tick);
            //logclock.Start();

            //-----Sonar EMGUCV ImageBoxes-----\\
            ExtendedWindowsFormsHost sonarhost1 = new ExtendedWindowsFormsHost();
            ExtendedWindowsFormsHost sonarhost2 = new ExtendedWindowsFormsHost();

            //Temporary turned off
            sonarhost1.Child = maincore.son1.imgBx;
            //sonarhost2.Child = maincore.son1.imgBx;

            this.sonar1.Children.Add(sonarhost1);
            //this.sonar2.Children.Add(sonarhost2);

            bindProperty<ToggleSwitch, EventHandler>(son1_pyt_switch, (c, h) => c.IsCheckedChanged += h, c => maincore.son1.MotionDetection = (bool)c.IsChecked);
            bindProperty<ToggleSwitch, EventHandler>(son1_vin_switch, (c, h) => c.IsCheckedChanged += h, c => maincore.son1.Identification = (bool)c.IsChecked);
            bindProperty<ToggleSwitch, EventHandler>(son1_trk_switch, (c, h) => c.IsCheckedChanged += h, c => maincore.son1.Tracking = (bool)c.IsChecked);

            //test1 = new ImageBox();
            //test1.SizeMode = PictureBoxSizeMode.StretchImage;
            //test1.FunctionalMode = ImageBox.FunctionalModeOption.Minimum;
            //sonarhost1.Child = test1;

            //test2 = new ImageBox();
            //test2.SizeMode = PictureBoxSizeMode.StretchImage;
            //test2.FunctionalMode = ImageBox.FunctionalModeOption.Minimum;
            //sonarhost2.Child = test2;

            //-----Video EMGUCV ImageBoxes-----\\
            ExtendedWindowsFormsHost videohost1 = new ExtendedWindowsFormsHost();
            ExtendedWindowsFormsHost videohost2 = new ExtendedWindowsFormsHost();
            videohost1.Child = maincore.cam1.imgBx;
            maincore.cam1.imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
            videohost2.Child = maincore.cam2.imgBx;
            maincore.cam2.imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
            this.video1.Children.Add(videohost1);
            this.video2.Children.Add(videohost2);

            //this.graph.DataSource = maincore.hydro1.chartCollection;
            //maincore.hydro1.acquisitionStart();
        }

        //    #region Load connection to Arduino
        //    if (viewmodel.isPLC_Connected == false)
        //    {
        //        try
        //        {
        //            driver = new ArduinoDriver.ArduinoDriver(ArduinoModel.UnoR3, "COM3", true);
        //            viewmodel.driver = driver;
        //            viewmodel.isPLC_Connected = true;
        //        }
        //        catch (System.IO.IOException ex)
        //        {
        //            System.Windows.MessageBox.Show("Arduino device is not connected");
        //            driver = null;
        //            return;
        //        }
        //    }
        //    else
        //    {
        //        driver = viewmodel.driver;
        //        //MessageBox.Show(viewmodel.isPLC_Connected.ToString());
        //    }
        //    // Set up pins to Arduino
        //    pResponse = driver.Send(new PinModeRequest(8, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(9, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(10, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(11, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(12, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(13, PinMode.Input));
        //    //Analog Pins
        //    pResponse = driver.Send(new PinModeRequest(14, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(15, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(16, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(17, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(18, PinMode.Input));
        //    pResponse = driver.Send(new PinModeRequest(19, PinMode.Input));
        //    #endregion
        //}


        private void RunLogAtTime()
        {
            DigitalReadResponse drResponse = null;
            AnalogReadResponse arResponse = null;
            string responseString = null; 

            //Read Digital Signals
            for(byte i=8; i<=13; i++)
            {
                drResponse = driver.Send(new DigitalReadRequest(i));
                responseString += @" " + drResponse.PinValue.ToString();
            }
            //Read Analog Signals
            for (byte j=14; j<=19; j++)
            {
                arResponse = driver.Send(new AnalogReadRequest(j));
                responseString += @" " + arResponse.PinValue.ToString();
            }
            
            // Write line to text file
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(@"C:\Users\VI-System2\Desktop\LogFile.txt", true))
            {
                file.WriteLine(DateTime.Now.ToShortDateString() + ' ' + DateTime.Now.ToLongTimeString() + responseString);
            }
        }

        private void Window_Closed(object sender, RoutedEventArgs e)
        {
            //maincore.hydro1.acquisitionStop();
            maincore.clock.Stop();
            maincore.sclock.Stop();
            try
            {
                _frameSaveTask.Wait();
            }
            catch (Exception exception)
            {
                Debug.WriteLine(exception);
            }
        }

        private void EnqueueCaptureTask(IImage sourceFrame, CaptureSource source)
        {
            var timestamp = DateTime.UtcNow;
            var frame = (IImage)sourceFrame.Clone();
            _frameSaveTask = _frameSaveTask.ContinueWith(t =>
            {
                CaptureStorage.Instance.AddCapture(frame, source, timestamp: timestamp);
                frame.Dispose();
            });
        }

        private int _currentlyQueued = 0;
        Task _frameSaveTask = Task.CompletedTask;
        private DispatcherTimer logclock;

        #region TIMER TICKS
        //Timer Event Handler - Event: Tick for videos
        static int matsize(IImage mat)
        {
            return (mat == null ? 0 : mat.Size.Width * mat.Size.Height * 3) + 10;
        }
        void timer_Tick(object sender, EventArgs e)
        {
            Mat cam1Frame = maincore.cam1.ProcessFrame();
            maincore.cam1.imgBx.Image = cam1Frame;
            Mat cam2Frame = maincore.cam2.ProcessFrame();
            maincore.cam2.imgBx.Image = cam2Frame;


            //Mat cam2Frame = maincore.cam2.ProcessFrame();
            //if (_currentlyQueued > 20)
            //{
            //    return;
            //}
            //var cam1Ts = DateTime.UtcNow;
            //Mat cam1FrameCopy = new Mat();
            //cam1Frame.CopyTo(cam1FrameCopy);
            //GC.AddMemoryPressure(matsize(cam1FrameCopy));
            //Interlocked.Increment(ref _currentlyQueued);
            //_frameSaveTask = _frameSaveTask.ContinueWith(t =>
            //{
            //    CaptureStorage.Instance.AddCapture(cam1FrameCopy, CaptureSource.Camera1, timestamp: cam1Ts);
            //    GC.RemoveMemoryPressure(matsize(cam1FrameCopy));
            //    cam1FrameCopy.Dispose();
            //    Interlocked.Decrement(ref _currentlyQueued);
            //});
            //var cam2Ts = DateTime.UtcNow;
            //var cam2FrameCopy = new Mat();
            //cam2Frame.CopyTo(cam2FrameCopy);
            //GC.AddMemoryPressure(matsize(cam2FrameCopy));
            //Interlocked.Increment(ref _currentlyQueued);
            //_frameSaveTask = _frameSaveTask.ContinueWith(t =>
            //{
            //    CaptureStorage.Instance.AddCapture(cam2FrameCopy, CaptureSource.Camera2, timestamp: cam2Ts);
            //    GC.RemoveMemoryPressure(matsize(cam2FrameCopy));
            //    cam2FrameCopy.Dispose();
            //    Interlocked.Decrement(ref _currentlyQueued);
            //});

            //maincore.ip1.ProcessFrame();
        }
        //Timer Event Handler - Event: Tick for sonar
        //void timer2_Tick(object sender, EventArgs e)
        //{
        //    if (maincore.son1 != null && maincore.son1.isConnected)
        //    {
        //        var cam1Ts = DateTime.UtcNow;
        //        var son1Frame = maincore.son1.ProcessFrame();
        //        _frameSaveTask = _frameSaveTask.ContinueWith(t => CaptureStorage.Instance.AddCapture(son1Frame, CaptureSource.Sonar1, timestamp: cam1Ts));
        //        maincore.son1.ProcessFrame();
        //    }
        //    if (maincore.son2 != null && maincore.son2.isConnected)
        //    {
        //        var cam2Ts = DateTime.UtcNow;
        //        var son2Frame = maincore.son2.ProcessFrame();
        //        _frameSaveTask = _frameSaveTask.ContinueWith(t => CaptureStorage.Instance.AddCapture(son2Frame, CaptureSource.Sonar2, timestamp: cam2Ts));
        //        maincore.son2.ProcessFrame();
        //    }
        //}
        //#endregion

        //#region Timer ticks
        ////Timer Event Handler - Event: Tick for videos
        //void timer_Tick(object sender, EventArgs e)
        //{
        //    EnqueueCaptureTask(maincore.cam1.ProcessFrame(), CaptureSource.Camera1);
        //    EnqueueCaptureTask(maincore.cam2.ProcessFrame(), CaptureSource.Camera2);

        //    //maincore.ip1.ProcessFrame();
        //}
        //Timer Event Handler - Event: Tick for sonar
        void timer2_Tick(object sender, EventArgs e)
        {
            if (maincore.counter == 10)
            {
                //Turn on saving
                    //maincore.saveAtThisTick = true;
                //Turn off saving
                maincore.saveAtThisTick = false;
            }

            if (maincore.counter >= 10)
            {
                maincore.counter = 0;
            }

            if (maincore.son1 != null && maincore.son1.isConnected)
            {
                if (maincore.saveAtThisTick)
                {
                    EnqueueCaptureTask(maincore.son1.ProcessFrame(), CaptureSource.Sonar1);
                }
                else
                {
                    maincore.son1.ProcessFrame();
                }
            }
            if (maincore.son2 != null && maincore.son2.isConnected)
            {
                if (maincore.saveAtThisTick)
                {
                    EnqueueCaptureTask(maincore.son2.ProcessFrame(), CaptureSource.Sonar2);
                }
                else
                {
                    maincore.son2.ProcessFrame();
                }
            }

            maincore.counter++;

            if (maincore.saveAtThisTick == true)
            {
                maincore.saveAtThisTick = false;
            }
        }

        //void timer3_Tick(object sender, EventArgs e)
        //{
        //    RunLogAtTime();
        //}
        #endregion

        #region BUTTON CLICKS
        private void btn_son1_head0_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (maincore.son1.getHeadCount() > 0)
                {
                    maincore.son1.connectHead(0);
                    btn_son1_head0.Background = new SolidColorBrush(Colors.Silver);
                    btn_son1_head1.Background = new SolidColorBrush(Colors.Black);
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("There is only one head on this sonar...");
                    System.Windows.Forms.MessageBox.Show(maincore.son1.getHeadCount().ToString());
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Unable to connect to 2nd sonar head...Connecting to head 1");
                maincore.son1.connectHead(0);
            }
        }

        private void btn_son1_head1_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (maincore.son1.getHeadCount() > 1)
                {
                    maincore.son1.connectHead(1);
                    btn_son1_head0.Background = new SolidColorBrush(Colors.Black);
                    btn_son1_head1.Background = new SolidColorBrush(Colors.Silver);   
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("There is only one head on this sonar...");
                    System.Windows.Forms.MessageBox.Show(maincore.son1.getHeadCount().ToString());
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Unable to connect to 2nd sonar head...Connecting to head 1");
                maincore.son1.connectHead(0);
            }
        }

        private void btn_son2_head0_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (maincore.son2.getHeadCount() > 0)
                {
                    maincore.son2.connectHead(0);
                    btn_son2_head0.Background = new SolidColorBrush(Colors.Silver);
                    btn_son2_head1.Background = new SolidColorBrush(Colors.Black);  
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("There is only one head on this sonar...");
                    System.Windows.Forms.MessageBox.Show(maincore.son1.getHeadCount().ToString());
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Unable to connect to 1st sonar head");
                maincore.son1.connectHead(0);
            }
        }

        private void btn_son2_head1_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (maincore.son2.getHeadCount() > 1)
                {
                    maincore.son2.connectHead(1);
                    btn_son2_head0.Background = new SolidColorBrush(Colors.Black);
                    btn_son2_head1.Background = new SolidColorBrush(Colors.Silver); 
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("There is only one head on this sonar...");
                    System.Windows.Forms.MessageBox.Show(maincore.son1.getHeadCount().ToString());
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Unable to connect to 2nd sonar head...Connecting to head 1");
                maincore.son1.connectHead(0);
            }

        }

        private void btn_SetRangeSon1_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (maincore.son1.isConnected == true)
                {
                    maincore.son1.setRange(viewmodel.startRange_s1, viewmodel.stopRange_s1);
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("Unable to set range in sonar 1!");
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.ToString());
            }
        }

        private void btn_SetRangeSon2_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (maincore.son2.isConnected == true)
                {
                    maincore.son2.setRange(viewmodel.startRange_s2, viewmodel.stopRange_s2);
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("Unable to set range in sonar 2!");
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.ToString());
            }
        }
        #endregion
    }
    public class ExtendedWindowsFormsHost : WindowsFormsHost
    {
        public ExtendedWindowsFormsHost()
        {
            ChildChanged += OnChildChanged;
        }

        private void OnChildChanged(object sender, ChildChangedEventArgs childChangedEventArgs)
        {
            var previousChild = childChangedEventArgs.PreviousChild as System.Windows.Forms.Control;
            if (previousChild != null)
            {
                previousChild.MouseDown -= OnMouseDown;
            }
            if (Child != null)
            {
                Child.MouseDown += OnMouseDown;
            }
        }

        private void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs mouseEventArgs)
        {
            MouseButton? wpfButton = ConvertToWpf(mouseEventArgs.Button);
            if (!wpfButton.HasValue)
                return;

            RaiseEvent(new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, wpfButton.Value)
            {
                RoutedEvent = Mouse.MouseDownEvent,
                Source = this,
            });
        }

        private MouseButton? ConvertToWpf(MouseButtons winformButton)
        {
            switch (winformButton)
            {
                case MouseButtons.Left:
                    return MouseButton.Left;
                case MouseButtons.None:
                    return null;
                case MouseButtons.Right:
                    return MouseButton.Right;
                case MouseButtons.Middle:
                    return MouseButton.Middle;
                case MouseButtons.XButton1:
                    return MouseButton.XButton1;
                case MouseButtons.XButton2:
                    return MouseButton.XButton2;
                default:
                    throw new ArgumentOutOfRangeException("winformButton");
            }
        }
    }
}
