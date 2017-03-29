using System;
using System.Drawing; 
using System.Collections.Generic;
using System.Text;
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
    public partial class MainWindow : MahApps.Metro.Controls.MetroWindow
    {
        private static readonly Lazy<ViewModel> lazyviewmodel = new Lazy<ViewModel>();
        public static ViewModel viewmodel { get { return lazyviewmodel.Value; } }
        public static ATCore maincore;
        private System.Windows.Controls.Button backButton;

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

        //Sonar Mouse Downs
        private void OnMouseDownSonar1(object sender, MouseButtonEventArgs e)
        {
            MainGrid.RowDefinitions.Clear();

            MiddleGrid.Visibility = Visibility.Hidden;
            BottomGrid.Visibility = Visibility.Hidden;

            TopGrid.ColumnDefinitions.Clear();

            sonar2.Visibility = Visibility.Hidden;
            //sonar3.Visibility = Visibility.Hidden;

            backButton.Click += new RoutedEventHandler(MinimizeTopGrid);

            TopGrid.Children.Add(backButton);
        }

        private void OnMouseDownSonar2(object sender, MouseButtonEventArgs e)
        {
            MainGrid.RowDefinitions.Clear();

            MiddleGrid.Visibility = Visibility.Hidden;
            BottomGrid.Visibility = Visibility.Hidden;

            TopGrid.ColumnDefinitions.Clear();

            sonar1.Visibility = Visibility.Hidden;
            //sonar3.Visibility = Visibility.Hidden;

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

        private void OnMouseDownVideo1(object sender, MouseButtonEventArgs e)
        {
            MainGrid.RowDefinitions.Clear();

            TopGrid.Visibility = Visibility.Hidden;
            BottomGrid.Visibility = Visibility.Hidden;

            MiddleGrid.ColumnDefinitions.Clear();

            video1.Margin = new Thickness(5,5,100,5);

            video2.Visibility = Visibility.Hidden;
            videoHost2.Visibility = Visibility.Hidden;
            //video3.Visibility = Visibility.Hidden;
            //videoHost3.Visibility = Visibility.Hidden;           

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
            MainGrid.RowDefinitions.Clear();

            TopGrid.Visibility = Visibility.Hidden;
            BottomGrid.Visibility = Visibility.Hidden;

            MiddleGrid.ColumnDefinitions.Clear();

            video1.Visibility = Visibility.Hidden;
            //video3.Visibility = Visibility.Hidden;

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

        private void MinimizeTopGrid(object sender, RoutedEventArgs e)
        {
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(2, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(2, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(3, GridUnitType.Star) });
            TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(3, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(3, GridUnitType.Star) });
            //TopGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            Grid.SetRow(TopGrid, 0);
            Grid.SetRow(MiddleGrid, 1);
            Grid.SetRow(BottomGrid, 2);

            Grid.SetColumn(sonar1, 1);
            Grid.SetColumn(sonar2, 2);
            //Grid.SetColumn(sonar3, 3);

            sonar1.Visibility = Visibility.Visible;
            sonar1.Margin = new Thickness(5);
            sonar2.Visibility = Visibility.Visible;
            sonar2.Margin = new Thickness(5);
            //sonar3.Visibility = Visibility.Visible;

            TopGrid.Visibility = Visibility.Visible;
            MiddleGrid.Visibility = Visibility.Visible;
            BottomGrid.Visibility = Visibility.Visible;

            backButton.Click -= new RoutedEventHandler(MinimizeTopGrid);
            TopGrid.Children.Remove(backButton);
        }

        private void MinimizeMiddleGrid(object sender, RoutedEventArgs e)
        {
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(2, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(2, GridUnitType.Star) });
            MainGrid.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            MiddleGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            MiddleGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
            //MiddleGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            Grid.SetRow(TopGrid, 0);
            Grid.SetRow(MiddleGrid, 1);
            Grid.SetRow(BottomGrid, 2);

            Grid.SetColumn(video1, 0);
            Grid.SetColumn(video2, 1);
            //Grid.SetColumn(video3, 2);

            video1.Visibility = Visibility.Visible;
            video1.Margin = new Thickness(5);
            video2.Visibility = Visibility.Visible;
            video2.Margin = new Thickness(5);
            //video3.Visibility = Visibility.Visible;

            TopGrid.Visibility = Visibility.Visible;
            MiddleGrid.Visibility = Visibility.Visible;
            BottomGrid.Visibility = Visibility.Visible;

            backButton.Click -= new RoutedEventHandler(MinimizeMiddleGrid);
            MiddleGrid.Children.Remove(backButton);
        }

        //Bottom Menu Clicks
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
            plc.DataContext = viewmodel;
            plc.Show();
            plc.Activate();
        }

        private void btn_FFT_Click(object sender, RoutedEventArgs e)
        {
            FFTWindow fft = new FFTWindow();
            fft.Show();
        }

        //This function is called after MainWindow loads - PUT INITIALIZATION FUNCTIONS HERE
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            maincore = new ATCore();
            maincore.clock.Tick += new EventHandler(timer_Tick);
            maincore.clock.Start();

            maincore.sclock.Tick += new EventHandler(timer2_Tick);
            maincore.sclock.Start();

            //-----Sonar EMGUCV ImageBoxes-----\\
            ExtendedWindowsFormsHost sonarhost1 = new ExtendedWindowsFormsHost();
            ExtendedWindowsFormsHost sonarhost2 = new ExtendedWindowsFormsHost();
            //ExtendedWindowsFormsHost sonarhost3 = new ExtendedWindowsFormsHost();
            sonarhost1.Child = maincore.son1.imgBx;
            sonarhost2.Child = maincore.son2.imgBx;
            //sonarhost3.Child = maincore.son3.imgBx;
            this.sonar1.Children.Add(sonarhost1);
            this.sonar2.Children.Add(sonarhost2);
            //this.sonar3.Children.Add(sonarhost3);

            //-----Video EMGUCV ImageBoxes-----\\
            ExtendedWindowsFormsHost videohost1 = new ExtendedWindowsFormsHost();
            ExtendedWindowsFormsHost videohost2 = new ExtendedWindowsFormsHost();
            //ExtendedWindowsFormsHost videohost3 = new ExtendedWindowsFormsHost();
            videohost1.Child = maincore.cam1.imgBx;
            maincore.cam1.imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
            videohost2.Child = maincore.cam2.imgBx;
            maincore.cam2.imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
            //videohost3.Child = maincore.cam3.imgBx;
            //maincore.cam3.imgBx.SizeMode = PictureBoxSizeMode.StretchImage;
            this.video1.Children.Add(videohost1);
            this.video2.Children.Add(videohost2);
            //this.video3.Children.Add(videohost3);

            //this.graph.DataSource = maincore.hydro1.chartCollection;
            //maincore.hydro1.acquisitionStart();
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

        Task _frameSaveTask = Task.CompletedTask;

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

        #region Timer ticks
        //Timer Event Handler - Event: Tick for videos
        void timer_Tick(object sender, EventArgs e)
        {
            EnqueueCaptureTask(maincore.cam1.ProcessFrame(), CaptureSource.Camera1);
            EnqueueCaptureTask(maincore.cam2.ProcessFrame(), CaptureSource.Camera2);

            //maincore.ip1.ProcessFrame();
        }
        //Timer Event Handler - Event: Tick for sonar
        void timer2_Tick(object sender, EventArgs e)
        {
            if (maincore.son1 != null && maincore.son1.isConnected)
            {
                EnqueueCaptureTask(maincore.son1.ProcessFrame(), CaptureSource.Sonar1);
            }
            if (maincore.son2 != null && maincore.son2.isConnected)
            {
                EnqueueCaptureTask(maincore.son2.ProcessFrame(), CaptureSource.Sonar2);
            }
        }
        #endregion

        private void Btn_CaptureHistory_OnClick(object sender, RoutedEventArgs e)
        {
            new MetroWindow() 
            { 
                Width = 1000, Height = 750, 
                WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen, 
                Title = "CAPTURE HISTORY",
                Content = new CaptureHistoryView() 
            }.ShowDialog();
        }

        private void btn_son1_head1_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (maincore.son1.headCount > 1)
                {
                    maincore.son1.connectHead(1);
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("There is only one head on this sonar...");
                }
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Unable to connect to 2nd sonar head...Connecting to head 1");
                maincore.son1.connectHead(0);
            }
        }

        private void btn_son2_head1_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (maincore.son2.headCount > 1)
                {
                    maincore.son2.connectHead(1);
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("There is only one head on this sonar...");
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
                if (maincore.son1.isConnected == true)
                {
                    maincore.son1.setRange(viewmodel.startRange_s1, viewmodel.stopRange_s1);
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
