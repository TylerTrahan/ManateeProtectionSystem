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
    /// Interaction logic for UpDown.xaml
    /// </summary>
    public partial class UpDown : UserControl
    {
        public UpDown()
        {
            InitializeComponent();
            Format = "D";
            txtNum.Text = Text;
        }

        private int _numValue = 0;

        public int NumValue
        {
            get {  return _numValue; }
            set
            {
                if (Min < Max) 
                { 
                    value = Math.Min(Math.Max(value, Min), Max - 1); 
                }
                _numValue = value;
                txtNum.Text = Text;
            }
        }

        private string Text { get { return NumValue.ToString(Format); } }

        public int Min { get; set; }
        public int Max { get; set; }
        public string Format { get; set; }

        private void cmdUp_Click(object sender, RoutedEventArgs e)
        {
            NumValue++;
        }
        
        private void cmdDown_Click(object sender, RoutedEventArgs e)
        {
            NumValue--;
        }
        
        private void txtNum_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.Equals(Text, txtNum.Text, StringComparison.Ordinal))
            {
                return;
            }

            int value;
            if (int.TryParse(txtNum.Text, out value))
            {
                NumValue = value;
            }
            else
            {
                txtNum.Text = Text;
            }
        }
    }
}
