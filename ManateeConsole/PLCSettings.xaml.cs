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
using System.Windows.Shapes;
using ArduinoDriver.SerialProtocol;
using ArduinoUploader.Hardware;

namespace ManateeConsole
{
    /// <summary>
    /// Interaction logic for PLCSettings.xaml
    /// </summary>
    public partial class PLCSettings : Window
    {
        ViewModel viewmodel = MainWindow.maincore.viewmodel;

        private const ArduinoModel AttachedArduino = ArduinoModel.UnoR3;

        public ArduinoDriver.ArduinoDriver driver = null;

        public PLCSettings()
        {
            if (viewmodel.isPLC_Connected == false)
            {
                try
                {
                    driver = new ArduinoDriver.ArduinoDriver(ArduinoModel.UnoR3, "COM9", true);
                    viewmodel.driver = driver;
                    viewmodel.isPLC_Connected = true;
                }
                catch (System.IO.IOException ex)
                {
                    System.Windows.MessageBox.Show("Arduino device is not connected");
                    driver = null;
                    return;
                }
            }
            else
            {
                driver = viewmodel.driver;
                //MessageBox.Show(viewmodel.isPLC_Connected.ToString());
            }
            InitializeComponent();
        }

        private void InitializeStates()
        {
            DigitalReadResponse drResponse;
            PinModeResponse pResponse;

            for (int i = 1; i < 9; i++)
            {
                byte pin = (byte)i;

                pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));

                drResponse = driver.Send(new DigitalReadRequest(pin));

                if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
                {
                    switch (pin)
                    {
                        case 2:
                            this.Signal1.Fill = new SolidColorBrush(Colors.Red);
                            break;
                        case 3:
                            this.Signal2.Fill = new SolidColorBrush(Colors.Red);
                            break;
                        case 4:
                            this.Signal3.Fill = new SolidColorBrush(Colors.Red);
                            break;
                        case 5:
                            this.Signal4.Fill = new SolidColorBrush(Colors.Red);
                            break;
                        case 6:
                            this.Signal5.Fill = new SolidColorBrush(Colors.Red);
                            break;
                        case 7:
                            this.Signal6.Fill = new SolidColorBrush(Colors.Red);
                            break;
                        case 8:
                            this.Signal7.Fill = new SolidColorBrush(Colors.Red);
                            break;
                        //case 9:
                        //    this.Signal8.Fill = new SolidColorBrush(Colors.Red);
                        //    break;
                        default:
                            break;
                    }
                }
                if(drResponse.PinValue==ArduinoDriver.DigitalValue.High)
                {
                    switch(pin)
                    {
                        case 2:
                            this.Signal1.Fill = new SolidColorBrush(Colors.WhiteSmoke);
                            break;
                        case 3:
                            this.Signal2.Fill = new SolidColorBrush(Colors.WhiteSmoke);
                            break;
                        case 4:
                            this.Signal3.Fill = new SolidColorBrush(Colors.WhiteSmoke);
                            break;
                        case 5:
                            this.Signal4.Fill = new SolidColorBrush(Colors.WhiteSmoke);
                            break;
                        case 6:
                            this.Signal5.Fill = new SolidColorBrush(Colors.WhiteSmoke);
                            break;
                        case 7:
                            this.Signal6.Fill = new SolidColorBrush(Colors.WhiteSmoke);
                            break;
                        case 8:
                            this.Signal7.Fill = new SolidColorBrush(Colors.WhiteSmoke);
                            break;
                        //case 9:
                        //    this.Signal8.Fill = new SolidColorBrush(Colors.WhiteSmoke);
                        //    break;
                        default:
                            break;
                    }
                }
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            byte pin = 2;
            PinModeResponse pResponse;
            DigitalReadResponse drResponse;
            DigitalWriteReponse dwResponse;


            //Set the pin number above to output a value
            //pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));
            //MessageBox.Show(pResponse.Pin.ToString() + " | " + pResponse.Mode.ToString());

            //Read the value on the pin currently
            drResponse = driver.Send(new DigitalReadRequest(pin));

            //If value is high, switch to low. If the value is low, switch to high
            if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.High));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());

                this.Signal1.Fill = new SolidColorBrush(Colors.WhiteSmoke);

            }
            else if(drResponse.PinValue ==ArduinoDriver.DigitalValue.High)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.Low));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());

                this.Signal1.Fill = new SolidColorBrush(Colors.Red);
            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            byte pin = 3;
            PinModeResponse pResponse;
            DigitalReadResponse drResponse;
            DigitalWriteReponse dwResponse;


            //Set the pin number above to output a value
            //pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));
            //MessageBox.Show(pResponse.Pin.ToString() + " | " + pResponse.Mode.ToString());

            //Read the value on the pin currently
            drResponse = driver.Send(new DigitalReadRequest(pin));

            //If value is high, switch to low. If the value is low, switch to high
            if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.High));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal2.Fill = new SolidColorBrush(Colors.WhiteSmoke);
            }
            else if (drResponse.PinValue == ArduinoDriver.DigitalValue.High)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.Low));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal2.Fill = new SolidColorBrush(Colors.Red);
            }
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            byte pin = 4;
            PinModeResponse pResponse;
            DigitalReadResponse drResponse;
            DigitalWriteReponse dwResponse;

            //Set the pin number above to output a value
            //pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));
            //MessageBox.Show(pResponse.Pin.ToString() + " | " + pResponse.Mode.ToString());

            //Read the value on the pin currently
            drResponse = driver.Send(new DigitalReadRequest(pin));

            //If value is high, switch to low. If the value is low, switch to high
            if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.High));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal3.Fill = new SolidColorBrush(Colors.WhiteSmoke);
            }
            else
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.Low));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal3.Fill = new SolidColorBrush(Colors.Red);
            }
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            byte pin = 5;
            PinModeResponse pResponse;
            DigitalReadResponse drResponse;
            DigitalWriteReponse dwResponse;

            //Set the pin number above to output a value
            //pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));
            //MessageBox.Show(pResponse.Pin.ToString() + " | " + pResponse.Mode.ToString());

            //Read the value on the pin currently
            drResponse = driver.Send(new DigitalReadRequest(pin));

            //If value is high, switch to low. If the value is low, switch to high
            if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.High));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal4.Fill = new SolidColorBrush(Colors.WhiteSmoke);
            }
            else
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.Low));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal4.Fill = new SolidColorBrush(Colors.Red);
            }
        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {
            byte pin = 6;
            PinModeResponse pResponse;
            DigitalReadResponse drResponse;
            DigitalWriteReponse dwResponse;

            //Set the pin number above to output a value
            //pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));
            //MessageBox.Show(pResponse.Pin.ToString() + " | " + pResponse.Mode.ToString());

            //Read the value on the pin currently
            drResponse = driver.Send(new DigitalReadRequest(pin));

            //If value is high, switch to low. If the value is low, switch to high
            if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.High));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal5.Fill = new SolidColorBrush(Colors.WhiteSmoke);
            }
            else
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.Low));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal5.Fill = new SolidColorBrush(Colors.Red);
            }
        }

        private void Button_Click_6(object sender, RoutedEventArgs e)
        {
            byte pin = 7;
            PinModeResponse pResponse;
            DigitalReadResponse drResponse;
            DigitalWriteReponse dwResponse;

            //Set the pin number above to output a value
            //pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));
            //MessageBox.Show(pResponse.Pin.ToString() + " | " + pResponse.Mode.ToString());

            //Read the value on the pin currently
            drResponse = driver.Send(new DigitalReadRequest(pin));

            //If value is high, switch to low. If the value is low, switch to high
            if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.High));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal6.Fill = new SolidColorBrush(Colors.WhiteSmoke);
            }
            else
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.Low));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal6.Fill = new SolidColorBrush(Colors.Red);
            }
        }

        private void Button_Click_7(object sender, RoutedEventArgs e)
        {
            byte pin = 8;
            PinModeResponse pResponse;
            DigitalReadResponse drResponse;
            DigitalWriteReponse dwResponse;

            //Set the pin number above to output a value
            //pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));
            //MessageBox.Show(pResponse.Pin.ToString() + " | " + pResponse.Mode.ToString());

            //Read the value on the pin currently
            drResponse = driver.Send(new DigitalReadRequest(pin));

            //If value is high, switch to low. If the value is low, switch to high
            if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.High));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal7.Fill = new SolidColorBrush(Colors.WhiteSmoke);
            }
            else
            {
                dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.Low));
                drResponse = driver.Send(new DigitalReadRequest(pin));
                //MessageBox.Show(drResponse.PinValue.ToString());
                this.Signal7.Fill = new SolidColorBrush(Colors.Red);
            }
        }

        private void Button_Click_8(object sender, RoutedEventArgs e)
        {
            byte pin = 9;
            PinModeResponse pResponse;
            DigitalReadResponse drResponse;
            DigitalWriteReponse dwResponse;

            //Set the pin number above to output a value
            //pResponse = driver.Send(new PinModeRequest(pin, PinMode.Output));
            //MessageBox.Show(pResponse.Pin.ToString() + " | " + pResponse.Mode.ToString());

            //Read the value on the pin currently
            drResponse = driver.Send(new DigitalReadRequest(pin));

            ////If value is high, switch to low. If the value is low, switch to high
            //if (drResponse.PinValue == ArduinoDriver.DigitalValue.Low)
            //{
            //    dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.High));
            //    drResponse = driver.Send(new DigitalReadRequest(pin));
            //    //MessageBox.Show(drResponse.PinValue.ToString());
            //    this.Signal8.Fill = new SolidColorBrush(Colors.WhiteSmoke);
            //}
            //else
            //{
            //    dwResponse = driver.Send(new DigitalWriteRequest(pin, ArduinoDriver.DigitalValue.Low));
            //    drResponse = driver.Send(new DigitalReadRequest(pin));
            //    //MessageBox.Show(drResponse.PinValue.ToString());
            //    this.Signal8.Fill = new SolidColorBrush(Colors.Red);
            //}
        }

        private void OnWindowLoaded(object sender, RoutedEventArgs e)
        {
            InitializeStates();
        }
    }
}
