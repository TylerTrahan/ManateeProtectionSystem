using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.CommandWpf;

namespace ManateeConsole
{
    class CaptureHistoryViewModel : ViewModelBase
    {
        private int _seconds;
        private int _minutes;
        private int _hours;
        private DateTime _date;
        private bool _isSearching;

        public bool IsEnabled
        {
            get { return CaptureStorage.Instance.IsEnabled; }
            set { CaptureStorage.Instance.IsEnabled = value; RaisePropertyChanged(() => IsEnabled); }
        }

        public DateTime DateTime
        {
            get { return Date.Date.Add(new TimeSpan(Hours, Minutes, Seconds)); }
            set
            {
                Date = value.Date;
                Hours = value.Hour;
                Minutes = value.Minute;
                Seconds = value.Second;
            }
        }

        public int Seconds
        {
            get { return _seconds; }
            set { _seconds = value; RaisePropertyChanged(() => Seconds); RaisePropertyChanged(() => DateTime); }
        }

        public int Minutes
        {
            get { return _minutes; }
            set { _minutes = value; RaisePropertyChanged(() => Minutes); RaisePropertyChanged(() => DateTime); }
        }

        public int Hours
        {
            get { return _hours; }
            set { _hours = value; RaisePropertyChanged(() => Hours); RaisePropertyChanged(() => DateTime); }
        }

        public int FpsLimit
        {
            get { return CaptureStorage.Instance.FpsLimit; }
            set { CaptureStorage.Instance.FpsLimit = value; RaisePropertyChanged(() => FpsLimit); }
        }

        public DateTime Date
        {
            get { return _date; }
            set { _date = value; RaisePropertyChanged(() => Date); RaisePropertyChanged(() => DateTime); }
        }

        public bool IsSearching
        {
            get { return _isSearching; }
            set { _isSearching = value; RaisePropertyChanged(() => IsSearching); }
        }

        public ICommand SearchCommand { get; private set; }
        public ICommand SwitchCommand { get; private set; }

        public ObservableCollection<CaptureEntry> Captures { get; private set; } 

        public CaptureHistoryViewModel()
        {
            DateTime = DateTime.Now;
            SearchCommand = new RelayCommand(async () => await SearchAsync());
            SwitchCommand = new RelayCommand(Switch);
            Captures = new ObservableCollection<CaptureEntry>();
        }

        private void Switch()
        {
            IsEnabled = !IsEnabled;
        }

        private async Task SearchAsync()
        {
            var dateTime = DateTime;
            Captures.Clear();
            IsSearching = true;
            try
            {
                var captures = await Task.Run(() => CaptureStorage.Instance.GetCaptures(dateTime, 1000).OrderBy(x => x.Source).ToArray());
                foreach (var capture in captures)
                {
                    Captures.Add(capture);
                }
            }
            finally
            {
                IsSearching = false;
            }
        }
    }
}
