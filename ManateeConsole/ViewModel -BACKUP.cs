using System;
using System.ComponentModel;

namespace ManateeConsole
{
    using SciChart.Charting.Model.DataSeries;
    using SciChart.Charting.ViewportManagers;
    using System.ComponentModel;
    using System.Timers;

    public class ViewModel : INotifyPropertyChanged
    {
        private IDataSeries<double, double> voltageDataSeries;
        
        private IDataSeries<double, double> amplitudeDataSeries;

        private double dt = 0.02;

        private int FifoSize = 1000;

        // Timer to process updates
        private Timer _timerNewDataUpdate;

        // The current time
        private double t;

        Random _random = new Random();

        #region OnPropertyChanged Members
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        #region ViewModel Constructor
        public ViewModel()
        {
            _timerNewDataUpdate = new Timer(dt * 1000);
            _timerNewDataUpdate.AutoReset = true;
            _timerNewDataUpdate.Elapsed += OnNewData;
           
            ViewportManager = new DefaultViewportManager();

            // Create a DataSeriesSet
            VoltageDataSeries = new XyDataSeries<double, double>();
            AmplitudeDataSeries = new XyDataSeries<double, double>();

            VoltageDataSeries.FifoCapacity = FifoSize;
            AmplitudeDataSeries.FifoCapacity = FifoSize;

            _timerNewDataUpdate.Start();
        }
        #endregion

        #region System Variable Declarations
        //--Sonar Variables--//
        private string _ipaddr_s1 = @"C:\Manatees.son";
        private string _ipaddr_s2 = @"C:\Manatees.son";
        private string _ipaddr_s3 = @"C:\Manatees.son";
        private bool? _isConnected_s1;
        private bool? _isConnected_s2;
        private bool? _isConnected_s3;
        private float _startRange_s1 = 1;
        private float _stopRange_s1 = 50;
        private float _startRange_s2 = 1;
        private float _stopRange_s2 = 50;
        private float _startRange_s3 = 1;
        private float _stopRange_s3 = 50;
        private bool? _isPaired_s1 = false;
        private bool? _isPaired_s2 = false;
        private bool? _isPaired_s3 = false;
        private string _spouse_s1 = "sonar2";
        private string _spouse_s2 = "sonar1";
        private string _spouse_s3 = null;
        private int _head_s1 = 0;
        private bool? _alternateHead_s1=false;
        private int _head_s2 = 0;
        private bool? _alternateHead_s2=false;
        private int _head_s3 = 0;
        private bool? _alternateHead_s3=true;
        //--Camera Variables--//
        private string _ipaddr_c1;
        private string _ipaddr_c2;
        private string _ipaddr_c3;
        private bool? _isConnected_c1;
        private bool? _isConnected_c2;
        private bool? _isConnected_c3;
        //--PanTilt Variables--//
        private string _serialPort_pt1 = "COM3";
        private string _partner_pt1 = null;
        private float _panHome_pt1 = 0;
        private float _tiltHome_pt1 = 210;
        private string _serialPort_pt2 = "COM4";
        private string _partner_pt2 = null;
        private float _panHome_pt2 = 0;
        private float _tiltHome_pt2 = 210;
        private string _serialPort_pt3 = "COM5";
        private string _partner_pt3 = null;
        private float _panHome_pt3 = 0;
        private float _tiltHome_pt3 = 210;
        //--Diagnostics Variables--//
        private float _rangeThres_s1 = 0; 
        private float _soundSpeed_s1 = 1500; 
        private bool? _isUpright_s1 = false; 
        private float _rangeThres_s2 = 0; 
        private float _soundSpeed_s2 = 1500; 
        private bool? _isUpright_s2 = false; 
        private float _rangeThres_s3 = 0; 
        private float _soundSpeed_s3 = 1500; 
        private bool? _isUpright_s3 = false; 
        private int _networkProtocol = 0; 
        private string _networkConnection = @"192.168.1.1"; 
        private string _port = @"80"; 
        private string _filepath = @"C:\Data\"; 
        private string _magImageFilePath = @"C:\Data\magImages\"; 
        private string _colorImageFilePath = @"C:\Data\colorImages\"; 
        private string _pngFilePath = @"C:\Data\Images\";
        //--Hydrophone Variables--//
        private double _hydroValue;
        #endregion

        //Need to reformat many of these functions -TMFT
        #region ViewModel Property Changers
        public string ipaddr_s1
        {
            get { return _ipaddr_s1; }
            set
            {
                if (value != this._ipaddr_s1)
                {
                    this._ipaddr_s1 = value;
                    OnPropertyChanged("ipaddr_s1");
                }
            }
        }
        public string ipaddr_s2
        {
            get { return _ipaddr_s2; }
            set
            {
                if (value != this._ipaddr_s2)
                {
                    this._ipaddr_s2 = value;
                    OnPropertyChanged("ipaddr_s2");
                }
            }
        }
        public string ipaddr_s3
        {
            get { return _ipaddr_s3; }
            set
            {
                if (value != this._ipaddr_s3)
                {
                    this._ipaddr_s3 = value;
                    OnPropertyChanged("ipaddr_s3");
                }
            }
        }
        public bool? isConnected_s1
        {
            get { return (_isConnected_s1 != null) ? _isConnected_s1 : false; }
            set
            {
                if (value != this._isConnected_s1)
                {
                    this._isConnected_s1 = value;
                    OnPropertyChanged("isConnected_s1");
                }
            }
        }
        public bool? isConnected_s2
        {
            get { return (_isConnected_s2 != null) ? _isConnected_s2 : false; }
            set
            {
                if (value != this._isConnected_s2)
                {
                    this._isConnected_s2 = value;
                    OnPropertyChanged("isConnected_s2");
                }
            }
        }
        public bool? isConnected_s3
        {
            get { return (_isConnected_s3 != null) ? _isConnected_s3 : false; }
            set
            {
                if (value != this._isConnected_s3)
                {
                    this._isConnected_s3 = value;
                    OnPropertyChanged("isConnected_s3");
                }
            }
        }
        public float startRange_s1
        { 
            get { return _startRange_s1; } 
            set 
            { 
                if (value != this._startRange_s1) 
                { 
                    this._startRange_s1 = value; 
                    OnPropertyChanged("startRange_s1"); 
                } 
            } 
        }
        public float stopRange_s1
        {
            get { return _stopRange_s1; }
            set 
            { 
                if (value != this._stopRange_s1) 
                { 
                    this._stopRange_s1 = value; 
                    OnPropertyChanged("stopRange_s1"); 
                }
            }
        }
        public float startRange_s2
        {
            get { return _startRange_s2; }
            set 
            { 
                if (value != this._startRange_s2) 
                { 
                    this._startRange_s2 = value; 
                    OnPropertyChanged("startRange_s2"); 
                } 
            }
        }
        public float stopRange_s2
        {
            get { return _stopRange_s2; }
            set 
            { 
                if (value != this._stopRange_s2) 
                { 
                    this._stopRange_s2 = value; 
                    OnPropertyChanged("stopRange_s2"); 
                } 
            }
        }
        public float startRange_s3
        {
            get { return _startRange_s3; }
            set 
            { 
                if (value != this._startRange_s3) 
                { 
                    this._startRange_s3 = value; 
                    OnPropertyChanged("startRange_s3"); 
                } 
            }
        }
        public float stopRange_s3
        {
            get { return _stopRange_s3; }
            set 
            { 
                if (value != this._stopRange_s3) 
                { 
                    this._stopRange_s3 = value; 
                    OnPropertyChanged("stopRange_s3"); 
                } 
            }
        }
        public bool? isPaired_s1
        {
            get { return _isPaired_s1; }
            set 
            { 
                if (value != this._isPaired_s1) 
                { 
                    this._isPaired_s1 = value; 
                    OnPropertyChanged("isPaired_s1"); 
                } 
            }
        }
        public bool? isPaired_s2
        {
            get { return _isPaired_s2; }
            set 
            { 
                if (value != this._isPaired_s2) 
                { 
                    this._isPaired_s2 = value; 
                    OnPropertyChanged("isPaired_s2"); 
                } 
            }
        }
        public bool? isPaired_s3
        {
            get { return _isPaired_s3; }
            set 
            { 
                if (value != this._isPaired_s3) 
                { 
                    this._isPaired_s3 = value; 
                    OnPropertyChanged("isPaired_s3"); 
                } 
            }
        }
        public string spouse_s1
        {
            get { return _spouse_s1; }
            set 
            { 
                if (value != this._spouse_s1) 
                { 
                    this._spouse_s1 = value; 
                    OnPropertyChanged("spouse_s1"); 
                } 
            }
        }
        public string spouse_s2
        {
            get { return _spouse_s2; }
            set 
            { 
                if (value != this._spouse_s2) 
                { 
                    this._spouse_s2 = value; 
                    OnPropertyChanged("spouse_s2"); 
                } 
            }
        }
        public string spouse_s3
        {
            get { return _spouse_s3; }
            set 
            { 
                if (value != this._spouse_s3) 
                { 
                    this._spouse_s3 = value; 
                    OnPropertyChanged("spouse_s3"); 
                } 
            }
        }
        public int head_s1
        {
            get { return _head_s1; }
            set 
            { 
                if (value != this._head_s1) 
                { 
                    this._head_s1 = value; 
                    OnPropertyChanged("head_s1"); 
                } 
            }
        }
        public int head_s2
        {
            get { return _head_s2; }
            set 
            { 
                if (value != this._head_s2) 
                { 
                    this._head_s2 = value; 
                    OnPropertyChanged("head_s2"); 
                } 
            }
        }
        public int head_s3
        {
            get { return _head_s3; }
            set 
            { 
                if (value != this._head_s3) 
                { 
                    this._head_s3 = value; 
                    OnPropertyChanged("head_s3"); 
                } 
            }
        }
        public bool? alternateHead_s1
        {
            get { return (_isConnected_s1 != null) ? _alternateHead_s1 : false; }
            set
            {
                if (value != this._alternateHead_s1)
                {
                    this._alternateHead_s1 = value;
                    OnPropertyChanged("alternateHead_s1");
                }
            }
        }
        public bool? alternateHead_s2
        {
            get { return (_isConnected_s2 != null) ? _alternateHead_s2 : false; }
            set
            {
                if (value != this._alternateHead_s2)
                {
                    this._alternateHead_s2 = value;
                    OnPropertyChanged("alternateHead_s2");
                }
            }
        }
        public bool? alternateHead_s3
        {
            get { return (_isConnected_s3 != null) ? _alternateHead_s3 : null; }
            set
            {
                if (value != this._alternateHead_s3)
                {
                    this._alternateHead_s3 = value;
                    OnPropertyChanged("alternateHead_s3");
                }
            }
        }
        public string ipaddr_c1
        {
            get { return _ipaddr_c1; }
            set
            {
                if (value != this._ipaddr_c1)
                {
                    this._ipaddr_c1 = value;
                    OnPropertyChanged("ipaddr_c1");
                }
            }
        }
        public string ipaddr_c2
        {
            get { return _ipaddr_c2; }
            set
            {
                if (value != this._ipaddr_c2)
                {
                    this._ipaddr_c2 = value;
                    OnPropertyChanged("ipaddr_c2");
                }
            }
        }
        public string ipaddr_c3
        {
            get { return _ipaddr_c3; }
            set
            {
                if (value != this._ipaddr_c3)
                {
                    this._ipaddr_c3 = value;
                    OnPropertyChanged("ipaddr_c3");
                }
            }
        }
        public bool? isConnected_c1
        {
            get { return (_isConnected_c1 != null) ? _isConnected_c1 : false; }
            set
            {
                if (value != this._isConnected_c1)
                {
                    this._isConnected_c1 = value;
                    OnPropertyChanged("isConnected_c1");
                }
            }
        }
        public bool? isConnected_c2
        {
            get { return (_isConnected_c2 != null) ? _isConnected_c2 : false; }
            set
            {
                if (value != this._isConnected_c2)
                {
                    this._isConnected_c2 = value;
                    OnPropertyChanged("isConnected_c2");
                }
            }
        }
        public bool? isConnected_c3
        {
            get { return (_isConnected_c3 != null) ? _isConnected_c3 : false; }
            set
            {
                if (value != this._isConnected_s1)
                {
                    this._isConnected_c3 = value;
                    OnPropertyChanged("isConnected_c3");
                }
            }
        }
        public string serialPort_pt1
        { 
            get { return _serialPort_pt1; } 
            set 
            { 
                if (value != this._serialPort_pt1) 
                { 
                    this._serialPort_pt1 = value; 
                    OnPropertyChanged("serialPort_pt1"); 
                } 
            } 
        }
        public string partner_pt1
        { 
            get { return _partner_pt1; } 
            set 
            { 
                if (value != this._partner_pt1) 
                { 
                    this._partner_pt1 = value; 
                    OnPropertyChanged("partner_pt1"); 
                } 
            } 
        }
        public float panHome_pt1
        { 
            get { return _panHome_pt1; } 
            set 
            { 
                if (value != this._panHome_pt1) 
                { 
                    this._panHome_pt1 = value; 
                    OnPropertyChanged("panHome_pt1"); 
                } 
            } 
        }
        public float tiltHome_pt1
        { 
            get { return _tiltHome_pt1; } 
            set 
            { 
                if (value != this._tiltHome_pt1) 
                { 
                    this._tiltHome_pt1 = value; 
                    OnPropertyChanged("tiltHome_pt1"); 
                } 
            } 
        }
        public string serialPort_pt2
        { 
            get { return _serialPort_pt2; } 
            set 
            { 
                if (value != this._serialPort_pt2) 
                { 
                    this._serialPort_pt2 = value; 
                    OnPropertyChanged("serialPort_pt2"); 
                } 
            } 
        }
        public string partner_pt2
         { 
            get { return _partner_pt2; } 
            set 
            { 
                if (value != this._partner_pt2) 
                { 
                    this._partner_pt2 = value; 
                    OnPropertyChanged("partner_pt2"); 
                } 
            } 
        }
        public float panHome_pt2
         { 
            get { return _panHome_pt2; } 
            set 
            { 
                if (value != this._panHome_pt2) 
                { 
                    this._panHome_pt2 = value; 
                    OnPropertyChanged("panHome_pt2"); 
                } 
            } 
        }
        public float tiltHome_pt2
         { 
            get { return _tiltHome_pt2; } 
            set 
            { 
                if (value != this._tiltHome_pt2) 
                { 
                    this._tiltHome_pt2 = value; 
                    OnPropertyChanged("tiltHome_pt2"); 
                } 
            } 
        }
        public string serialPort_pt3
         { 
            get { return _serialPort_pt3; } 
            set
            { 
                if (value != this._serialPort_pt3) 
                { 
                    this._serialPort_pt3 = value; 
                    OnPropertyChanged("serialPort_pt3"); 
                } 
            } 
        }
        public string partner_pt3
         { 
            get { return _partner_pt3; } 
            set 
            { 
                if (value != this._partner_pt3) 
                { 
                    this._partner_pt3 = value; 
                    OnPropertyChanged("partner_pt3"); 
                } 
            } 
        }
        public float panHome_pt3
         { 
            get { return _panHome_pt3; } 
            set 
            { 
                if (value != this._panHome_pt3) 
                { 
                    this._panHome_pt3 = value; 
                    OnPropertyChanged("panHome_pt3"); 
                } 
            } 
        }
        public float tiltHome_pt3
         { 
            get { return _tiltHome_pt3; } 
            set 
            { 
                if (value != this._tiltHome_pt3) 
                { 
                    this._tiltHome_pt3 = value; 
                    OnPropertyChanged("tiltHome_pt3"); 
                } 
            } 
        }
        public float rangeThres_s1
         { 
            get { return _rangeThres_s1; } 
            set 
            { 
                if (value != this._rangeThres_s1) 
                { 
                    this._rangeThres_s1 = value; 
                    OnPropertyChanged("rangeThres_s1"); 
                } 
            } 
        }
        public float soundSpeed_s1
         { 
            get { return _soundSpeed_s1; } 
            set
            { 
                if (value != this._soundSpeed_s1) 
                { 
                    this._soundSpeed_s1 = value; 
                    OnPropertyChanged("soundSpeed_s1"); 
                } 
            } 
        }
        public bool? isUpright_s1
         { 
            get { return _isUpright_s1; } 
            set 
            { 
                if (value != this._isUpright_s1) 
                { 
                    this._isUpright_s1 = value; 
                    OnPropertyChanged("isUpright_s1"); 
                } 
            } 
        }
        public float rangeThres_s2
         { 
            get { return _rangeThres_s2; } 
            set 
            { 
                if (value != this._rangeThres_s2) 
                { 
                    this._rangeThres_s2 = value; 
                    OnPropertyChanged("rangeThres_s2"); 
                } 
            } 
        }
        public float soundSpeed_s2
         { 
            get { return _soundSpeed_s2; } 
            set 
            { 
                if (value != this._soundSpeed_s2) 
                { 
                    this._soundSpeed_s2 = value; 
                    OnPropertyChanged("soundSpeed_s2"); 
                } 
            } 
        }
        public bool? isUpright_s2
         { 
            get { return _isUpright_s2; } 
            set 
            { 
                if (value != this._isUpright_s2) 
                { 
                    this._isUpright_s2 = value; 
                    OnPropertyChanged("isUpright_s2"); 
                } 
            } 
        }
        public float rangeThres_s3
         { 
            get { return _rangeThres_s3; } 
            set 
            { 
                if (value != this._rangeThres_s3) 
                { 
                    this._rangeThres_s3 = value; 
                    OnPropertyChanged("rangeThres_s3"); 
                } 
            } 
        }
        public float soundSpeed_s3
         { 
            get { return _soundSpeed_s3; } 
            set 
            { 
                if (value != this._soundSpeed_s3) 
                { 
                    this._soundSpeed_s3 = value; 
                    OnPropertyChanged("soundSpeed_s3"); 
                } 
            } 
        }
        public bool? isUpright_s3
         { 
            get { return _isUpright_s3; } 
            set 
            { 
                if (value != this._isUpright_s3) 
                { 
                    this._isUpright_s3 = value; 
                    OnPropertyChanged("isUpright_s3"); 
                } 
            } 
        }
        public double hydroValue
        {
            get { return _hydroValue; }
            set
            {
                if (value != this._hydroValue)
                {
                    this._hydroValue = value;
                    OnPropertyChanged("hydroValue");
                }
            }
        }
        public int networkProtocol
         { 
            get { return _networkProtocol; } 
            set 
            { 
                if (value != this._networkProtocol) 
                { 
                    this._networkProtocol = value; 
                    OnPropertyChanged("networkProtocol"); 
                } 
            } 
        }
        public string networkConnection
         { 
            get { return _networkConnection; } 
            set 
            { 
                if (value != this._networkConnection) 
                { 
                    this._networkConnection = value; 
                    OnPropertyChanged("networkConnection"); 
                } 
            } 
        }
        public string port
         { 
            get { return _port; } 
            set 
            { 
                if (value != this._port) 
                { 
                    this._port = value; 
                    OnPropertyChanged("port"); 
                } 
            } 
        }
        public string filepath
         { 
            get { return _filepath; } 
            set 
            { 
                if (value != this._filepath) 
                { 
                    this._filepath = value; 
                    OnPropertyChanged("filepath"); 
                } 
            } 
        }
        public string magImageFilePath
         { 
            get { return _magImageFilePath; } 
            set 
            { 
                if (value != this._magImageFilePath) 
                { 
                    this._magImageFilePath = value; 
                    OnPropertyChanged("magImageFilePath"); 
                } 
            } 
        }
        public string colorImageFilePath
         { 
            get { return _colorImageFilePath; } 
            set 
            { 
                if (value != this._colorImageFilePath) 
                { 
                    this._colorImageFilePath = value; 
                    OnPropertyChanged("colorImageFilePath"); 
                } 
            } 
        }
        public string pngFilePath
         { 
            get { return _pngFilePath; } 
            set 
            { 
                if (value != this._pngFilePath) 
                { 
                    this._pngFilePath = value; 
                    OnPropertyChanged("pngFilePath"); 
                } 
            } 
        }
        #endregion

        private void OnNewData(object sender, ElapsedEventArgs e)
        {
            //var data = _dataSource.GetVoltageSeries(1000);

            // Append data to series.
            // _dataSeries0.Append(data.XData, data.YData);

            double y1 = 3.0 * Math.Sin(((2 * Math.PI) * 1.4) * t) + _random.NextDouble() * 0.5;
            //AtHydrphone ath = new AtHydrphone();

            //for (int i = 0; i < VoltageValues.Count; i++)
            //{
            //    data.Samples[i].Value

            //    VoltageDataSeries.Append(t, VoltageValues[i]);

            //    t += dt;
            //}

            VoltageDataSeries.Append(t, y1); // This plots the voltage graoh you need to pass voltage values to y1
            var db = 20 * Math.Log10(y1) - 120;
            AmplitudeDataSeries.Append(t, db);
           
            // Increment current time
            t += dt;
        }


        public IDataSeries<double, double> VoltageDataSeries
        {
            get { return voltageDataSeries; }
            set
            {
                voltageDataSeries = value;
                OnPropertyChanged("VoltageDataSeries");
            }
        }
        public IDataSeries<double, double> AmplitudeDataSeries
        {
            get { return amplitudeDataSeries; }
            set
            {
                amplitudeDataSeries = value;
                OnPropertyChanged("AmplitudeDataSeries");
            }
        }

        public IViewportManager ViewportManager { get; set; }
    }
}
