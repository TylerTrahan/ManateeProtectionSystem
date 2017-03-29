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
        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
            "Value", typeof (int), typeof (UpDown), new FrameworkPropertyMetadata(default(int), FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, PropertyChangedCallback));

        private static void PropertyChangedCallback(DependencyObject dependencyObject, DependencyPropertyChangedEventArgs dependencyPropertyChangedEventArgs)
        {
            var ud = dependencyObject as UpDown;
            if (ud != null)
            {
                ud.OnValuePropertyChanged();
            }
        }

        public int Value
        {
            get { return (int) GetValue(ValueProperty); }
            set { SetValue(ValueProperty, value); }
        }

        public UpDown()
        {
            InitializeComponent();
            Format = "D";
            txtNum.Text = Text;
        }

        private void OnValuePropertyChanged()
        {
            if (Min < Max)
            {
                var value = Math.Min(Math.Max(Value, Min), Max - 1);
                if (value != Value)
                {
                    Value = value;
                    return;
                }
            }
            txtNum.Text = Text;
        }

        private string Text { get { return Value.ToString(Format); } }

        public int Min { get; set; }
        public int Max { get; set; }
        public string Format { get; set; }

        private void cmdUp_Click(object sender, RoutedEventArgs e)
        {
            Value++;
        }
        
        private void cmdDown_Click(object sender, RoutedEventArgs e)
        {
            Value--;
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
                Value = value;
            }
            else
            {
                txtNum.Text = Text;
            }
        }
    }
}
